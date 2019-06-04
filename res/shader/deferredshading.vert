#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 TexCoords;

void main()
{
    TexCoords = uv;
    gl_Position = vec4(position, 1.0);
}