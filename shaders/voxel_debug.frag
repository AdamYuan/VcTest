#version 450 core

out vec4 FragColor;

in vec3 vTexcoordsFront, vTexcoordsBack;

layout (binding = 0) uniform sampler3D uVoxelAlbedo;

void main()
{
	FragColor = texture(uVoxelAlbedo, gl_FrontFacing ? vTexcoordsFront : vTexcoordsBack);
	if(FragColor.a < 0.1f)
		discard;
}
