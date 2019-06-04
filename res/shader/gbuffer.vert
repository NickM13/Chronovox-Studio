#version 430

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform vec4 colorScalar;

out vec3 FragPos;
out vec4 Color;
out vec2 TexCoords;
out vec3 Normal;

layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec2 vTexCoords;
layout(location=3) in vec3 vNormal;

void main() {
	vec4 worldPos = Model * vec4(vPosition, 1.0);
	FragPos = worldPos.xyz;
	TexCoords = vTexCoords;
	Color = vColor * colorScalar;
	
	mat3 normalMatrix = transpose(inverse(mat3(Model)));
	Normal = normalMatrix * vNormal;
	
	gl_Position = Projection * View * worldPos;
}
