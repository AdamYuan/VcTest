#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoords;

out vec2 vTexcoords;
out vec3 vNormal;

void main()
{
	vTexcoords = aTexcoords;
	vNormal = aNormal;
	gl_Position = vec4(aPosition, 1.0f);
}
