#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aTexcoordsFront;
layout (location = 2) in vec3 aTexcoordsBack;

out vec3 vTexcoordsFront, vTexcoordsBack;

uniform mat4 uProjection, uView;

void main()
{
	vTexcoordsFront = aTexcoordsFront;
	vTexcoordsBack = aTexcoordsBack;
	gl_Position = uProjection * uView * vec4(aPosition, 1.0f);
}
