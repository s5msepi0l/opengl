#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 MVP;

void main() {
    TexCoords = aPos; // Use the position as direction for cubemap
    gl_Position = MVP * vec4(aPos, 1.0);
}