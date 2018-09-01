#version 430

layout(location=4) uniform sampler2D texMap;
layout(location=5) uniform int isFont;

in vec4 ex_color;
smooth in vec2 uvInter;

layout(location = 0) out vec4 color;

void main() {
	if (isFont == 1) {
		color = ex_color * vec4(1.0, 1.0, 1.0, texture(texMap, uvInter).r);
	}
	else if(textureSize(texMap, 0).x > 1) {
		color = ex_color * texture(texMap, uvInter);
	}
	else {
		color = ex_color;
	}
}
