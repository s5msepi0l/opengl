#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/std_image.h"

#define WIDTH 1920
#define HEIGHT 1080

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    fprintf(stderr, "OpenGL Debug: %s\n", message);
}

using namespace glm;

// Vertex data with positions and texture coordinates
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    1.0f,  1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    1.0f,  1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    1.0f, -1.0f,  1.0f,  0.0f, 0.0f
};

class DeltaTime {
public:
    DeltaTime() : lastTime(0.0), deltaTime(0.0) {}

    // Call this every frame to update delta time
    void Update() {
        double currentTime = glfwGetTime();  // Get the current time
        deltaTime = currentTime - lastTime;  // Calculate the delta time
        lastTime = currentTime;              // Update lastTime for the next frame
    }

    // Get the delta time (time between frames)
    double GetDeltaTime() const {
        return deltaTime;
    }

private:
    double lastTime;   // Time of the previous frame
    double deltaTime;  // Time difference between current and previous frame
};

GLuint load_cube_map_tx() {
    //texture sides

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    std::vector<std::string> texture_faces {
        "res/textures/grass_side.png",    // right
        "res/textures/grass_side.png",    // left
        "res/textures/grass_top.png",     // top
        "res/textures/dirt.png",  // bottom
        "res/textures/grass_side.png",    // back
        "res/textures/grass_side.png"     // front
    };
    int width, height, nr_channels;
    unsigned char *data;


    for (unsigned int i = 0; i<texture_faces.size(); i++) {

        data = stbi_load(texture_faces[i].c_str(), &width, &height, &nr_channels, 0);
        if (!data) {
            std::cout << "Unable to load texture_faces[" << i << "] = " << texture_faces[i] << std::endl;  
            return -1;
        }
     
        std::cout << "Succesfully loaded texture_faces[ +" << i << "] = " << texture_faces[i] << std::endl;
        std::cout << "Width = " << width << "\nHeight = " << height << "\nnr_channels = " << nr_channels << std::endl;
        
        GLenum fmt = (nr_channels == 4) ? GL_RGBA : GL_RGB;
        GLenum ifmt = (nr_channels == 4) ? GL_RGBA8 : GL_RGB8;
        
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
        );
        
        
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

    return texture;
}

int main() {
    glewExperimental = true;
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Tutorial", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "GLEW initialization failed\n");
        return -1;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
/*
    // Load texture
    int width, height, channels;
    unsigned char* image = stbi_load("res/textures/grass.jpg", &width, &height, &channels, 0);
    if (!image) {
        fprintf(stderr, "Failed to load texture\n");
        return -1;
    }*/

    GLuint texture = load_cube_map_tx();
    glActiveTexture(GL_TEXTURE0);
  /*  glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);


    stbi_image_free(image);
*/
    GLuint programID = LoadShaders("res/shaders/vertex.vert", "res/shaders/shader.frag");
    glUseProgram(programID);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugCallback, nullptr);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    DeltaTime frametime;

    glm::vec3 position = glm::vec3(0, 0, 0);

    float init_fov = 90.0f;

    float speed = 7.0f;
    float mouse_speed = 0.01f;
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    struct {
        double horizontal = 3.14f;
        double vertical = 0.0f;
    }angle;
    do {
        frametime.Update();
        float deltatime = frametime.GetDeltaTime();

        std::cout << deltatime << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double x_pos;
        double y_pos;

        glfwGetCursorPos(window, &x_pos, &y_pos);
        
        glfwSetCursorPos(window, WIDTH/2.0, HEIGHT/2.0);


        angle.horizontal += mouse_speed * deltatime * float(WIDTH/2 - x_pos);
        angle.vertical += mouse_speed * deltatime * float(HEIGHT/2 - y_pos);
        
        glm::vec3 dir (
            cos(angle.vertical) * sin(angle.horizontal),
            sin(angle.vertical),
            cos(angle.vertical) * cos(angle.horizontal)
        );

        glm::vec3 right = glm::vec3(
            sin(angle.horizontal - 3.14f/2.0f),
            0,
            cos(angle.horizontal - 3.14f/2.0f)
        );

        glm::vec3 up = glm::cross(right, dir);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position += dir * deltatime * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position -= dir * deltatime * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position += right * deltatime * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position -= right * deltatime * speed;
        }

        // Create projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(init_fov), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
        
        glm::mat4 view = glm::lookAt(
            position,
            position + dir,
            up);
    
        // Base model matrix (no translation for the first cube)
        glm::mat4 model1 = glm::mat4(1.0f); 

        // Translate the second cube to the left
        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f)); // Shift by -2.0f along x-axis
        
        // MVP matrices for each cube
        glm::mat4 MVP1 = projection * view * model1;
        glm::mat4 MVP2 = projection * view * model2;

        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        
        // Draw the first cube
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the second cube (shifted left)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);


        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwWindowShouldClose(window) == 0 && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glDeleteProgram(programID);

    glfwTerminate();
    return 0;
}
