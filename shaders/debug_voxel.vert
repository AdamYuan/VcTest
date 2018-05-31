#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aTexcoords1;
layout (location = 2) in vec3 aTexcoords2;

uniform mat4 uProjection, uView;

out vec3 vTexcoords1, vTexcoords2;

void main()
{
	vTexcoords1 = aTexcoords1;
	vTexcoords2 = aTexcoords2;
	gl_Position = uProjection * uView * vec4(aPosition, 1.0f);
}
