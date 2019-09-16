#version 430

layout(location=4) uniform sampler2D texMap;
layout(location=5) uniform int isFont;
layout(location=6) uniform vec4 subtexCoords;
layout(location=7) uniform vec4 scissor;

in vec4 ex_color;
smooth in vec2 uvInter;

layout(location = 0) out vec4 color;

void main() {
	if (scissor.z < 1 || (
		gl_FragCoord.x > scissor.x &&
		gl_FragCoord.y > scissor.y &&
		gl_FragCoord.x < scissor.x + scissor.z &&
		gl_FragCoord.y < scissor.y + scissor.w)) {
		if (isFont == 1) {
			color = ex_color * vec4(1.0, 1.0, 1.0, texture(texMap, uvInter).r);
		}
		else if(textureSize(texMap, 0).x > 1) {
			vec2 uvSub = mod(uvInter, 1.f);
			uvSub = uvSub * vec2(subtexCoords.z - subtexCoords.x, subtexCoords.w - subtexCoords.y);
			uvSub = uvSub + subtexCoords.xy;
			color = ex_color * texture(texMap, uvSub);
		}
		else {
			color = ex_color;
		}
	}
}
