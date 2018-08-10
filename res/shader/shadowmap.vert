#version 430


layout(location=0) uniform mat4 Projection;
layout(location=1) uniform mat4 View;
layout(location=2) uniform mat4 Model;
layout(location=3) uniform vec4 colorScalar;
layout(location=4) uniform sampler2D texMap;
layout(location=5) uniform vec3 light;
layout(location=6) uniform bool useLight;

layout(location=0) in vec4 vertexPosition;
layout(location=1) in vec4 vertexColor;
layout(location=2) in vec2 vertexUV;
layout(location=3) in vec3 vertexNormal;

// Output data ; will be interpolated for each fragment.
out vec4 ShadowCoord;
out vec4 ex_color;
out float cosThetaSun;
out float cosThetaNorm;

// Values that stay constant for the whole mesh.
layout(location=10) uniform mat4 DepthBiasMVP;
layout(location=11) uniform vec3 sunlight;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  Projection * View * Model * vertexPosition;
	
	ShadowCoord = DepthBiasMVP * Model * vertexPosition;
	
	if(useLight) {
		cosThetaSun = clamp(dot(vertexNormal, sunlight), 0, 1);
		cosThetaNorm = clamp(dot(vertexNormal, light), 0, 1);
	}
	else {
		cosThetaSun = 1;
		cosThetaNorm = 1;
	}
	ex_color = vertexColor * colorScalar;
}
