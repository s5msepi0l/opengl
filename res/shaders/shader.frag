#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

out vec2 TexCoord; // output to fragment shader

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    TexCoord = texcoord;  // pass texture coordinates to fragment shader
}
