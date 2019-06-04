#version 430

layout(location=4) uniform sampler2D texMap;

in vec4 ex_color;
smooth in vec2 texCoordsInterpolated;

layout(location = 0) out vec4 FragColor;

void main(){
		FragColor = ex_color;
}
