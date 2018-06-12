#version 450 core

out vec4 FragColor;

in vec3 vTexcoordsFront, vTexcoordsBack;

layout (binding = 0) uniform sampler3D uVoxelAlbedo;

void main()
{
	FragColor = texture(uVoxelAlbedo, gl_FrontFacing ? vTexcoordsFront : vTexcoordsBack);
	if(FragColor.a < 0.1f)
		discard;
	vec3 mapped = vec3(1.0f) - exp(-FragColor.rgb * 1.5f);
	mapped = pow(mapped, vec3(1.0f / 2.2f));
	FragColor = vec4(mapped, 1.0f);
}
