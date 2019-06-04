#version 430

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform vec4 colorScalar;
uniform sampler2D texMap;
uniform vec3 light;
uniform bool useLight;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;
layout(location=2) in vec2 uv;
layout(location=3) in vec3 normal;

out vec4 ex_color;
smooth out vec2 texCoordsInterpolated;

void main(){
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	ex_color = colorScalar;

	texCoordsInterpolated = uv;
}
