#version 430

out vec4 FragColor;

in vec2 TexCoords;

uniform int aaSamples;
uniform sampler2D screenTex;

vec4 ssaa(sampler2D screenTex, int samples, ivec2 uv) {
	vec4 avg = vec4(0);
	for (int x = 0; x < samples; x++) {
		for (int y = 0; y < samples; y++) {
			avg += texelFetch(screenTex, uv * samples + ivec2(x, y), 0);
		}
	}
	return avg / pow(samples, 2);
}

void main() {
	FragColor = ssaa(screenTex, aaSamples, ivec2(gl_FragCoord.x, gl_FragCoord.y));
}
