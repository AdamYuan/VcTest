#version 450 core

layout (location = 0) out vec4 HalfGPosition;
layout (location = 1) out vec3 HalfGNormal;

layout (binding = 0) uniform sampler2D uGPosition;
layout (binding = 1) uniform sampler2D uGNormal;

in vec2 vTexcoords;

void main()
{
	ivec2 texel = ivec2(gl_FragCoord.xy) * 2;
	HalfGPosition = texelFetch(uGPosition, texel, 0);
	HalfGPosition += texelFetch(uGPosition, texel + ivec2(1, 0), 0);
	HalfGPosition += texelFetch(uGPosition, texel + ivec2(0, 1), 0);
	HalfGPosition += texelFetch(uGPosition, texel + ivec2(1, 1), 0);
	HalfGPosition *= 0.25f;
	HalfGNormal = texelFetch(uGNormal, texel, 0).rgb;
	HalfGNormal += texelFetch(uGNormal, texel + ivec2(1, 0), 0).rgb;
	HalfGNormal += texelFetch(uGNormal, texel + ivec2(0, 1), 0).rgb;
	HalfGNormal += texelFetch(uGNormal, texel + ivec2(1, 1), 0).rgb;
	HalfGNormal *= 0.25f;
}
