#version 430

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
layout(location=0) uniform mat4 Projection;
layout(location=1) uniform mat4 View;
layout(location=2) uniform mat4 Model;

void main(){
	gl_Position =  Projection * View * Model * vec4(vertexPosition_modelspace,1);
}
