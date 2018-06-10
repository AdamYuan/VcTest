#version 450 core

layout (location = 0) out vec4 GPosition;
layout (location = 1) out vec3 GNormal;
layout (location = 2) out vec4 GAlbedo;

in vec2 vTexcoords;
in vec3 vFragPos, vNormal;

layout (binding = 0) uniform sampler2D uDiffuseTexture;

const float kNear = 0.1f, kFar = 100.0f; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * kNear * kFar) / (kFar + kNear - z * (kFar - kNear));
}

void main()
{
	vec4 color4 = texture(uDiffuseTexture, vTexcoords);
	if(color4.a < 0.1f)
		discard;

	GPosition = vec4(vFragPos, LinearizeDepth(gl_FragCoord.z));
	GNormal = normalize(vNormal) * 0.5f + 0.5f;
	GAlbedo = vec4(color4.rgb, 1.0f);
}
