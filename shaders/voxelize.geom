#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec2 vTexcoords[];
in vec4 vLightspaceFragPos[];

out vec2 gTexcoords;
out vec3 gNormal;
out vec4 gLightspaceFragPos;
out vec3 gWorldPos;

uniform vec3 uVoxelGridRangeMin, uVoxelGridRangeMax;
uniform float uVoxelWorldSize;

vec3 WorldToScreen(in vec3 v)
{
	return ((v - uVoxelGridRangeMin) / uVoxelWorldSize) / 500.0f - 1.0f;
}

vec2 WorldToScreen(in vec3 v, in int axis)
{
	vec3 vs = WorldToScreen(v);
	if(axis == 0) return vs.yz;
	else if(axis == 1) return vs.xz;
	else return vs.xy;
}

void AddVertex(in vec3 world_mid, in int vert_index, in int axis)
{
	gTexcoords = vTexcoords[vert_index];
	gLightspaceFragPos = vLightspaceFragPos[vert_index];
	gNormal = normalize(vNormal[vert_index]);

	gWorldPos = gl_in[vert_index].gl_Position.xyz;

	vec2 screen_pos = WorldToScreen(gWorldPos, axis), screen_mid_pos = WorldToScreen(world_mid, axis);

	//screen_pos += normalize(screen_pos - screen_mid_pos) * 0.05f;

	gl_Position = vec4(screen_pos, 1.0f, 1.0f);
	EmitVertex();
}

void main()
{
    vec3 p1 = normalize(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz);
    vec3 p2 = normalize(gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
	vec3 axis_weight = abs(cross(p1, p2));

	int axis;
	if(axis_weight.x >= axis_weight.y && axis_weight.x > axis_weight.z) axis = 0;
	else if(axis_weight.y >= axis_weight.z && axis_weight.y > axis_weight.x) axis = 1;
	else axis = 2;

	vec3 world_mid = (gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz) / 3.0f;

	AddVertex(world_mid, 0, axis);
	AddVertex(world_mid, 1, axis);
	AddVertex(world_mid, 2, axis);
	EndPrimitive();
}
