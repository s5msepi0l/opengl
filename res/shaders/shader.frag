#version 330 core

in vec2 TexCoord; // texture coordinates from vertex shader
out vec4 color;

uniform sampler2D ourTexture;

void main() {
    color = texture(ourTexture, TexCoord);  // sample the texture
}
