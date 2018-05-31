#version 450 core
layout (location = 0) in vec3 aPos;
uniform mat4 uProjection, uView;
out vec3 vTexcoords;
void main()
{
	vTexcoords = aPos;
	vec4 pos = uProjection * uView * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
}
