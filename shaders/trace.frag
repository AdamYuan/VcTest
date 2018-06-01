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

vec4 SampleVoxel(in vec3 world_pos, in float lod)
{
	vec3 voxel_uv = ((world_pos - uVoxelGridRangeMin) / uVoxelWorldSize) / vec3(uVoxelDimension);
	voxel_uv.xy += 1.0f / vec2(uVoxelDimension.xy);
	return textureLod(uVoxelTexture, voxel_uv, lod);
}

vec3 ConeTrace(in vec3 start_pos, in vec3 direction, in float tan_half_angle)
{
	vec4 color = vec4(0.0f);

	float dist = uVoxelWorldSize * 2.5f;

	while(dist < 32.0f && color.a < 1.0f)
	{
		vec3 pos = start_pos + dist * direction;

		if(
				pos.x < uVoxelGridRangeMin.x || pos.x > uVoxelGridRangeMax.x ||
				pos.y < uVoxelGridRangeMin.y || pos.y > uVoxelGridRangeMax.y ||
				pos.z < uVoxelGridRangeMin.z || pos.z > uVoxelGridRangeMax.z)
			break;

		float diameter = max(uVoxelWorldSize, 2.0f * tan_half_angle * dist);
		float lod = log2(diameter / uVoxelWorldSize);
		vec4 voxel_color = SampleVoxel(pos, lod);
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
			//final_color += ConeTrace(position, reflect(vViewDir, normal), 0.3f);
			if(uShowAlbedo) 
				final_color *= albedo.rgb;
			final_color *= kLightColor;
		}
		//debug mipmap
		//final_color += SampleVoxel(position, 7.0f).rgb;
	}
	else
		final_color = texture(uSkyboxTexture, vViewDir).rgb;

	final_color = pow(final_color, vec3(1.0f / 2.2f));
	FragColor = vec4(final_color, 1.0f);
}
