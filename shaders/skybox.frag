#version 450 core
out vec4 FragColor;
in vec3 vTexcoords;
layout (binding = 0) uniform samplerCube uSkyboxTexture;
void main()
{
	FragColor = texture(uSkyboxTexture, vTexcoords);
}
