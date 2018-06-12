#version 450 core

in vec2 gTexcoords;
in vec3 gNormal;
in vec4 gLightspaceFragPos;
in vec3 gWorldPos;

layout (binding = 0) uniform sampler2D uDiffuseTexture;
layout (binding = 1) uniform sampler2DShadow uShadowMap;
layout (rgba8, binding = 5) uniform writeonly image3D uVoxelAlbedo;
layout (rgba8, binding = 6) uniform writeonly image3D uVoxelNormal;
layout (rgba8, binding = 7) uniform writeonly image3D uVoxelRadiance;

uniform vec3 uVoxelGridRangeMin, uVoxelGridRangeMax;
uniform float uVoxelWorldSize;

uniform vec3 uLightDir;

float CalculateShadow()
{
	vec3 proj_coords = gLightspaceFragPos.xyz / gLightspaceFragPos.w;
	proj_coords = proj_coords * 0.5f + 0.5f;
	if(proj_coords.z > 1.0)
		return 0.0;
	return texture(uShadowMap, vec3(proj_coords.xy, proj_coords.z - 0.002f));
}

float DirectLight()
{
	//return 1.0f;
	//return max(dot(-uLightDir, normalize(gNormal)), 0.0f);
	return dot(-uLightDir, normalize(gNormal)) > 0 ? 1.0f : 0.0f;
}

void main()
{
	if(
			gWorldPos.x < uVoxelGridRangeMin.x || gWorldPos.x > uVoxelGridRangeMax.x ||
			gWorldPos.y < uVoxelGridRangeMin.y || gWorldPos.y > uVoxelGridRangeMax.y ||
			gWorldPos.z < uVoxelGridRangeMin.z || gWorldPos.z > uVoxelGridRangeMax.z)
		discard;
	ivec3 voxel_pos = ivec3((gWorldPos - uVoxelGridRangeMin) / vec3(uVoxelWorldSize));

	vec4 color = texture(uDiffuseTexture, gTexcoords);
	imageStore(uVoxelAlbedo, voxel_pos, color);

	color.rgb *= CalculateShadow() * DirectLight();
	imageStore(uVoxelRadiance, voxel_pos, color);

	vec4 normal = vec4(normalize(gNormal) * 0.5f + 0.5f, 1.0f);
	imageStore(uVoxelNormal, voxel_pos, normal);
}
