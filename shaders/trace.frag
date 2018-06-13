#version 450 core
#define PI 3.14159265f

out vec4 FragColor;

in vec2 vTexcoords;
in vec3 vViewDir;

layout (binding = 0) uniform sampler2D uHalfGPosition;
layout (binding = 1) uniform sampler2D uHalfGNormal;
layout (binding = 6) uniform sampler3D uVoxelRadiance;
layout (binding = 7) uniform sampler3D uVoxelRadianceMipmaps[6];

uniform ivec3 uVoxelDimension;
uniform vec3 uVoxelGridRangeMin, uVoxelGridRangeMax;
uniform float uVoxelWorldSize;

uniform vec3 uCamPosition;

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
	//voxel_uv.xy += 1.0f / vec2(uVoxelDimension.xy);

	float mipmap_lod = max(0.0f, lod - 1.0f);
	vec4 mipmap_color = vec4(0.0f);
	if(lod > 0.0f)
	{
		if(weights.x > 0.0f)
			mipmap_color += textureLod(uVoxelRadianceMipmaps[indices.x], voxel_uv, mipmap_lod) * weights.x;
		if(weights.y > 0.0f)
			mipmap_color += textureLod(uVoxelRadianceMipmaps[indices.y], voxel_uv, mipmap_lod) * weights.y;
		if(weights.z > 0.0f)
			mipmap_color += textureLod(uVoxelRadianceMipmaps[indices.z], voxel_uv, mipmap_lod) * weights.z;
	}
	if(lod < 1.0f)
		return mix(texture(uVoxelRadiance, voxel_uv), mipmap_color, max(lod, 0.0f));
	else
		return mipmap_color;
}

vec3 ConeTrace(in vec3 start_pos, in vec3 direction, in float tan_half_angle)
{
	vec4 color = vec4(0.0f);

	float dist = 0.3f;

	ivec3 load_indices = ivec3(
			direction.x < 0.0f ? 0 : 1, 
			direction.y < 0.0f ? 2 : 3, 
			direction.z < 0.0f ? 4 : 5);
	vec3 weights = direction * direction;

	while(dist < 32.0f && color.a < 1.0f)
	{
		vec3 pos = start_pos + dist * direction;

		float diameter = 2.0f * tan_half_angle * dist;
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

	for(int i = 0; i < 6; i++)
		color += kConeWeights[i] * ConeTrace(start_pos, normalize(matrix * kConeDirections[i]), 0.57735f);

	return color * PI;
}

void main()
{
	vec3 color, normal = texture(uHalfGNormal, vTexcoords).rgb * 2.0f - 1.0f;

	if(length(normal) > 0.5f)
	{
		vec3 position = texture(uHalfGPosition, vTexcoords).rgb;
		color = IndirectLight(position + normal * 0.3f, GetTBN(normal));
	}
	else
		color = vec3(0.0f);

	FragColor = vec4(color, 1.0f);
}
