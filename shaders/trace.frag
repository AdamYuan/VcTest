#version 450 core

out vec4 FragColor;

in vec2 vTexcoords;
in vec3 vViewDir;

layout (binding = 0) uniform sampler2D uGPosition;
layout (binding = 1) uniform sampler2D uGNormal;
layout (binding = 2) uniform sampler2D uGAlbedo;
layout (binding = 4) uniform samplerCube uSkyboxTexture;
layout (binding = 5) uniform sampler2DShadow uShadowMap;
layout (binding = 6) uniform sampler3D uVoxelTexture;

layout (binding = 7) uniform sampler3D uVoxelTextureMipmaps[6];

uniform mat4 uLightMatrix;
uniform vec3 uLightDir;

uniform ivec3 uVoxelDimension;
uniform vec3 uVoxelGridRangeMin, uVoxelGridRangeMax;
uniform float uVoxelWorldSize;

uniform bool uEnableIndirectTrace, uShowAlbedo, uDebugVoxel;

uniform vec3 uCamPosition;

const vec2 kShadowTexelSize = vec2(1.0f / 2048.0f);
const vec3 kLightColor = vec3(2.2f, 2.0f, 1.8f);

const int kDiffuseConeNum = 6;
const vec3 kConeDirections[6] = 
{
	vec3(0, 1, 0),
	vec3(0, 0.5, 0.866025),
	vec3(0.823639, 0.5, 0.267617),
	vec3(0.509037, 0.5, -0.700629),
	vec3(-0.509037, 0.5, -0.700629),
	vec3(-0.823639, 0.5, 0.267617)
};
const float kConeWeights[6] = {0.25f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f};

void ons(in vec3 v1, inout vec3 v2, out vec3 v3)
{
	if (abs(v1.x) > abs(v1.y)) 
	{
		float inv_len = inversesqrt(v1.x * v1.x + v1.z * v1.z);
		v2 = vec3(-v1.z * inv_len, 0.0f, v1.x * inv_len);
	} 
	else 
	{
		float inv_len = inversesqrt(v1.y * v1.y + v1.z * v1.z);
		v2 = vec3(0.0f, v1.z * inv_len, -v1.y * inv_len);
	}
	v3 = cross(v1, v2);
}

vec4 SampleVoxel(in vec3 world_pos, in float lod, in ivec3 indices, in vec3 weights)
{
	vec3 voxel_uv = ((world_pos - uVoxelGridRangeMin) / uVoxelWorldSize) / vec3(uVoxelDimension);
	voxel_uv.xy += 1.0f / vec2(uVoxelDimension.xy);

	float mipmap_lod = max(0.0f, lod - 1.0f);
	vec4 mipmap_color = vec4(0.0f);
	if(weights.x > 0.05f)
		mipmap_color += textureLod(uVoxelTextureMipmaps[indices.x], voxel_uv, mipmap_lod) * weights.x;
	if(weights.y > 0.05f)
		mipmap_color += textureLod(uVoxelTextureMipmaps[indices.y], voxel_uv, mipmap_lod) * weights.y;
	if(weights.z > 0.05f)
		mipmap_color += textureLod(uVoxelTextureMipmaps[indices.z], voxel_uv, mipmap_lod) * weights.z;
	//vec4 mipmap_color = textureLod(uVoxelTextureMipmaps[5], voxel_uv, mipmap_lod);
	if(lod < 1.0f)
		return mix(texture(uVoxelTexture, voxel_uv), mipmap_color, lod);
	else
		return mipmap_color;
}

/*vec4 SampleVoxel(in vec3 world_pos, in float lod, in int dir)
{
	vec3 voxel_uv = ((world_pos - uVoxelGridRangeMin) / uVoxelWorldSize) / vec3(uVoxelDimension);
	float mipmap_lod = max(0.0f, lod - 1.0f);
	vec4 mipmap_color = textureLod(uVoxelTextureMipmaps[dir], voxel_uv, mipmap_lod);
	if(lod < 1.0f)
	{
		return mix(texture(uVoxelTexture, voxel_uv), mipmap_color, lod);
	}
	else
		return mipmap_color;
}*/

vec3 ConeTrace(in vec3 start_pos, in vec3 direction, in float tan_half_angle)
{
	vec4 color = vec4(0.0f);

	float dist = 0.5f;

	ivec3 load_indices = ivec3(
			direction.x < 0.0f ? 0 : 1, 
			direction.y < 0.0f ? 2 : 3, 
			direction.z < 0.0f ? 4 : 5);
	vec3 weights = direction * direction;

	/*vec3 weight = abs(direction);
	int load_dir;
	if(weight.x > weight.y && weight.x > weight.z)
		load_dir = int(weight.x > 0);
	else if(weight.y > weight.x && weight.y > weight.z)
		load_dir = int(weight.y > 0) + 2;
	else
		load_dir = int(weight.z > 0) + 4;*/

	while(dist < 32.0f && color.a < 1.0f)
	{
		vec3 pos = start_pos + dist * direction;

		float diameter = max(uVoxelWorldSize, 2.0f * tan_half_angle * dist);
		float lod = log2(diameter / uVoxelWorldSize);
		vec4 voxel_color = SampleVoxel(pos, lod, load_indices, weights);
		color += voxel_color * (1.0f - color.a);

		dist += diameter * 0.5f;
	}

	return color.rgb;
}

vec3 IndirectLight(in vec3 start_pos, in mat3 matrix)
{
	vec3 color = vec3(0.0f);

	for(int i = 0; i < kDiffuseConeNum; i++)
		color += kConeWeights[i] * ConeTrace(start_pos, normalize(matrix * kConeDirections[i]), 0.577f);

	return color;// * 3.14159f;
}

float CalculateShadow(in vec3 position)
{
	vec4 lightspace_frag_pos = uLightMatrix * vec4(position, 1.0f);
	vec3 proj_coords = lightspace_frag_pos.xyz / lightspace_frag_pos.w;
	proj_coords = proj_coords * 0.5f + 0.5f;

	float sum = 0.0f;
	for(int i = -2; i <= 2; ++i)
		for(int j = -2; j <= 2; ++j)
			sum += texture(uShadowMap, vec3(proj_coords.xy + vec2(float(i), float(j)) * kShadowTexelSize, proj_coords.z - 0.001f));

	return sum / 25.0f;
}

vec3 DirectLight(in vec3 normal)
{
	//diffuse
	return vec3(max(dot(-uLightDir, normal), 0.0));
}


void main()
{
	vec4 albedo = texture(uGAlbedo, vTexcoords);
	vec3 final_color;

	if(albedo.a > 0.5f)
	{
		if(uDebugVoxel)
		{
			final_color = ConeTrace(uCamPosition, vViewDir, 0.0f);
		}
		else
		{
			vec3 position = texture(uGPosition, vTexcoords).rgb;
			vec3 normal = texture(uGNormal, vTexcoords).rgb * 2.0f - 1.0f;

			vec3 tangent, bitangent;
			ons(normal, tangent, bitangent);

			final_color = DirectLight(normal) * CalculateShadow(position);
			if(uEnableIndirectTrace)
				final_color += IndirectLight(position, mat3(tangent, normal, bitangent));
			//final_color += ConeTrace(position, reflect(vViewDir, normal), 0.2f);
			if(uShowAlbedo) 
				final_color *= albedo.rgb;
			final_color *= kLightColor;

			//debug mipmap
			//final_color = SampleVoxel(position, 4.0f, ivec3(0), vec3(0.0f)).rgb;
		}
	}
	else
		final_color = texture(uSkyboxTexture, vViewDir).rgb;

	vec3 mapped = vec3(1.0f) - exp(-final_color * 5.0f);
	mapped = pow(mapped, vec3(1.0f / 2.2f));
	FragColor = vec4(mapped, 1.0f);
}
