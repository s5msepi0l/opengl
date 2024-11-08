#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}
/*#version 330 core

// Input from the vertex buffer
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

// Uniform for the Model-View-Projection matrix
uniform mat4 MVP;

// Output to the fragment shader
out vec3 fragmentColor;

void main() {
    // Transform the vertex position using the MVP matrix
    gl_Position = MVP * vec4(vertexPosition, 1.0);

    // Pass the vertex color to the fragment shader
    fragmentColor = vertexColor;
}
*/