#version 430

layout(location=0) uniform mat4 Projection;
layout(location=1) uniform mat4 View;
layout(location=2) uniform mat4 Model;
layout(location=3) uniform vec4 colorScalar;

layout(location=0) in vec2 position;
layout(location=1) in vec4 color;
layout(location=2) in vec2 uv;

out vec4 ex_color;
smooth out vec2 uvInter;

void main() {
	gl_Position = Projection * View * Model * vec4(position, 0.0, 1.0);

	ex_color = color * colorScalar;
	
	uvInter = uv;
}
