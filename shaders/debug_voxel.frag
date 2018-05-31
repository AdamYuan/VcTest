#version 450 core

out vec4 FragColor;
in vec3 vTexcoords1, vTexcoords2;

layout (binding = 6) uniform sampler3D uVoxelTexture;

void main()
{
	vec4 color4 = textureLod(uVoxelTexture, gl_FrontFacing ? vTexcoords2 : vTexcoords1, 0.0f);
	if(color4.a == 0.0f)
		discard;
	FragColor = vec4(pow(color4.rgb, vec3(1.0f / 2.2f)), 1.0f);
}
