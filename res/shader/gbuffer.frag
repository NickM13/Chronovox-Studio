#version 430

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec4 Color;
in vec2 TexCoords;
in vec3 Normal;

uniform bool useLight;
uniform vec3 light;
uniform bool wireframe;

void main(){
	// Store the fragment position vector in the first gbuffer texture
	gPosition = FragPos;
	// Also store the per-fragment normals into the gbuffer
	gNormal = normalize(Normal);
	if (useLight && !wireframe) {
		float cosThetaLight = clamp(dot(gNormal, light), 0, 1) * 0.5 + 0.5;
		gAlbedoSpec.rgba = vec4(Color.rgb * cosThetaLight, Color.a);
	}
	else {
		gAlbedoSpec.rgba = Color.rgba;
	}
}
