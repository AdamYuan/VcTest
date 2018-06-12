#version 450 core

#define PI 3.14159265f

out vec4 FragColor;

in vec2 vTexcoords;
in vec3 vViewDir;

layout (binding = 0) uniform sampler2D uGPosition;
layout (binding = 1) uniform sampler2D uGNormal;
layout (binding = 2) uniform sampler2D uGAlbedo;
layout (binding = 3) uniform sampler2D uHalfTraceResult;
layout (binding = 4) uniform samplerCube uSkyboxTexture;
layout (binding = 5) uniform sampler2DShadow uShadowMap;
layout (binding = 6) uniform sampler3D uVoxelRadiance;
layout (binding = 9) uniform sampler3D uVoxelRadianceMipmaps[6];

uniform mat4 uLightMatrix;
uniform vec3 uLightDir;

uniform ivec3 uVoxelDimension;
uniform vec3 uVoxelGridRangeMin, uVoxelGridRangeMax;
uniform float uVoxelWorldSize;
uniform ivec2 uResolution;

uniform bool uEnableIndirectTrace, uShowAlbedo, uShowEdge;

uniform vec3 uCamPosition;

const vec2 kShadowTexelSize = vec2(1.0f / 2048.0f);
const vec3 kLightColor = vec3(2.2f, 2.0f, 1.8f);

const vec3 kConeDirections[6] = 
{
	vec3(0, 0, 1),
	vec3(0, 0.866025, 0.5),
	vec3(0.823639, 0.267617, 0.5),
	vec3(0.509037, -0.700629, 0.5),
	vec3(-0.509037, -0.700629, 0.5),
	vec3(-0.823639, 0.267617, 0.5)
};
const float kConeWeights[6] = {0.25f, 0.15f, 0.15f, 0.15f, 0.15f, 0.15f};
const ivec2 kEdgeTests[8] = {{0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1}};

mat3 GetTBN(in vec3 normal)
{
	vec3 tangent;
	vec3 v1 = cross(normal, vec3(0.0f, 0.0f, 1.0f)), v2 = cross(normal, vec3(0.0f, 1.0f, 0.0f));
	if(dot(v1, v1) > dot(v2, v2))
		tangent = v1;
	else
		tangent = v2;
	
	return mat3(tangent, cross(tangent, normal), normal);
}

vec4 SampleVoxel(in vec3 world_pos, in float lod, in ivec3 indices, in vec3 weights)
{
	vec3 voxel_uv = ((world_pos - uVoxelGridRangeMin) / uVoxelWorldSize) / vec3(uVoxelDimension);
	voxel_uv.xy += 1.0f / vec2(uVoxelDimension.xy);

	float mipmap_lod = max(0.0f, lod - 1.0f);
	vec4 mipmap_color = vec4(0.0f);
	if(weights.x > 0.0f)
		mipmap_color += textureLod(uVoxelRadianceMipmaps[indices.x], voxel_uv, mipmap_lod) * weights.x;
	if(weights.y > 0.0f)
		mipmap_color += textureLod(uVoxelRadianceMipmaps[indices.y], voxel_uv, mipmap_lod) * weights.y;
	if(weights.z > 0.0f)
		mipmap_color += textureLod(uVoxelRadianceMipmaps[indices.z], voxel_uv, mipmap_lod) * weights.z;
	if(lod < 1.0f)
		return mix(texture(uVoxelRadiance, voxel_uv), mipmap_color, lod);
	else
		return mipmap_color;
}

vec3 ConeTrace(in vec3 start_pos, in vec3 direction, in float tan_half_angle)
{
	vec4 color = vec4(0.0f);

	float dist = 0.5f;

	ivec3 load_indices = ivec3(
			direction.x < 0.0f ? 0 : 1, 
			direction.y < 0.0f ? 2 : 3, 
			direction.z < 0.0f ? 4 : 5);
	vec3 weights = direction * direction;

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

bool DetectEdge(in float depth, in vec3 normal)
{
	float sample_depth;
	vec3 sample_normal;
	for(int i = 0; i < 8; ++i)
	{
		sample_depth = texture(uGPosition, vec2(gl_FragCoord.xy + vec2(kEdgeTests[i])) / vec2(uResolution)).a;
		if(abs(depth - sample_depth) > 0.7f)
			return true;
		sample_normal = texture(uGNormal, vec2(gl_FragCoord.xy + vec2(kEdgeTests[i])) / vec2(uResolution)).rgb * 2.0f - 1.0f;
		if(dot(normal, sample_normal) < 0.8f)
			return true;
	}
	return false;
}

vec3 IndirectLight(in vec3 start_pos, in mat3 matrix)
{
	vec3 color = vec3(0.0f);
	for(int i = 0; i < 6; i++)
		color += kConeWeights[i] * ConeTrace(start_pos, normalize(matrix * kConeDirections[i]), 0.57735f);
	return color * PI;
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
		vec4 pos4 = texture(uGPosition, vTexcoords);
		vec3 position = pos4.rgb;
		float depth = pos4.a;
		vec3 normal = texture(uGNormal, vTexcoords).rgb * 2.0f - 1.0f;
		vec3 trace_result = texture(uHalfTraceResult, vTexcoords).rgb;

		final_color = DirectLight(normal) * CalculateShadow(position);

		bool edge = DetectEdge(depth, normal);
		if(edge && uShowEdge)
		{
			final_color = vec3(1.0f, 0.0f, 0.0f);
		}
		else if(uEnableIndirectTrace)
		{
			if(edge)
				final_color += IndirectLight(position + normal * 0.2f, GetTBN(normal));
			else
				final_color += texture(uHalfTraceResult, vTexcoords).rgb;
		}
		if(uShowAlbedo) 
			final_color *= albedo.rgb;
		final_color *= kLightColor;
	}
	else
		final_color = texture(uSkyboxTexture, vViewDir).rgb;

	vec3 mapped = vec3(1.0f) - exp(-final_color * 1.5f);
	mapped = pow(mapped, vec3(1.0f / 2.2f));
	FragColor = vec4(mapped, 1.0f);
}
