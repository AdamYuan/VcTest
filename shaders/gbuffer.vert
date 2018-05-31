#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aTexcoords;

out vec2 vTexcoords;
out vec3 vFragPos, vNormal;
out mat3 vTBN;

uniform mat4 uProjection, uView;

void main()
{
	vFragPos = aPosition;
	vTexcoords = aTexcoords;
	vNormal = aNormal;
	vTBN = mat3(aTangent, aBitangent, aNormal);
	gl_Position = uProjection * uView * vec4(aPosition, 1.0f);
}
