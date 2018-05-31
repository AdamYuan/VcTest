#version 450 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aTexcoords;

out vec2 vTexcoords;
out vec4 vLightspaceFragPos;
out vec3 vNormal;

uniform mat4 uLightMatrix;

void main()
{
	vLightspaceFragPos = uLightMatrix * vec4(aPosition, 1.0f);
	vTexcoords = aTexcoords;
	vNormal = aNormal;
	gl_Position = vec4(aPosition, 1.0f);
}
