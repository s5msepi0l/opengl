#version 330 core

layout(location = 0) in vec3 vertexPosition; // Position from g_vertex_buffer_data
layout(location = 1) in vec3 vertexColor;    // Color from g_color_buffer_data

out vec3 fragmentColor; // Pass color to the fragment shader
uniform mat4 MVP;       // Model-View-Projection matrix

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1.0);
    fragmentColor = vertexColor;
}
