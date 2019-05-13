#version 430

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 ShadowCoord;
in vec4 ex_color;
in float cosThetaSun;
in float cosThetaNorm;
in vec4 gl_FragCoord;

// Ouput data
layout(location = 0) out vec4 color;
out float gl_FragDepth;

// Values that stay constant for the whole mesh.
//layout(location=4) uniform sampler2D myTextureSampler;
layout(location=6) uniform bool useLight;
layout(location=7) uniform sampler2D shadowMap;
layout(location=8) uniform float depthLayer;
layout(location=20) uniform bool useShadows;

float random(in vec3 seed4, in int i);
float random(in vec3 seed4, in int i) {
	float dot_product = dot(vec4(seed4, i), vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

void main(){
	vec3 LightColor = vec3(1.f,1.f,1.f);
	
	vec3 MaterialDiffuseColor = ex_color.rgb;
	vec3 _color = MaterialDiffuseColor;
	float visibility = 1.0f;
	if(useLight) {
		visibility = visibility * (cosThetaNorm * 0.5f + 0.5f);
	}
	if(useShadows) {
		if(visibility > 0) {
			float currentDepth = ShadowCoord.z;
			vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
			int shadowPixels = 0;
			for(int x = -1; x <= 1; ++x) {
				for(int y = -1; y <= 1; ++y) {
					float pcfDepth = texture(shadowMap, ShadowCoord.xy + vec2(x, y) * texelSize).r;
					if(currentDepth > pcfDepth) {
						shadowPixels++;
					}
				}
			}
			visibility -= clamp(shadowPixels, 0, 6) / 6.0;
		}
		// Make shadows non-solid
		visibility = visibility * 0.25f + 0.75f;
		visibility = visibility * (cosThetaNorm * 0.5f + 0.5f);
	}
	_color *= (LightColor * visibility);
	color = vec4(_color, ex_color.a);
	gl_FragDepth = gl_FragCoord.z * depthLayer;
}
