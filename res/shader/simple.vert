layout(location=0) uniform mat4 Projection;
layout(location=1) uniform mat4 View;
layout(location=2) uniform mat4 Model;
layout(location=3) uniform vec4 colorScalar;
layout(location=4) uniform sampler2D texMap;
layout(location=5) uniform vec3 light;
layout(location=6) uniform bool useLight;

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;
layout(location=2) in vec2 uv;
layout(location=3) in vec3 normal;

out vec4 ex_color;
smooth out vec2 texCoordsInterpolated;

void main(){
	gl_Position = Projection * View * Model * position;
	ex_color = color * colorScalar;
	if(useLight) {
		float cosTheta = clamp(dot(normal, light), -1, 1) * 0.75f + 0.5f;
		ex_color *= vec4(cosTheta, cosTheta, cosTheta, 1.f);
	}

	texCoordsInterpolated = uv;
}
