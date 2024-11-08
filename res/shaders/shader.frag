#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main(){

    // Output color = color of the texture at the specified UV
    color = texture( myTextureSampler, UV ).rgb;
}


/*#version 330 core

// Input from the vertex shader
in vec3 fragmentColor;

// Output color
out vec4 color;

void main() {
    // Set the fragment color
    color = vec4(fragmentColor, 1.0);
}
*/