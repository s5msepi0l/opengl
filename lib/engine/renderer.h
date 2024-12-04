#pragma once

#include "../../state.h"
#include "util.h"
#include "../stb/std_image.h"


#include <vector>
#include <ctime>

#define VISABLE   1
#define INVISIBLE 0

typedef struct {
    u8 r = 255;
    u8 g = 255;
    u8 b = 255;
}rgb;

typedef struct {
    u16 x;
    u16 y;
} px_offset;

static const GLfloat g_vertex_buffer_data[6][30] = {
    { //front face
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    },
    { //back face
    1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    },
    { //left face
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    },
    { //right face
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    1.0f,  1.0f, -1.0f,  0.0f, 0.0f,   
    },
    { //top face
    1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    1.0f,  1.0f, -1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,   
    },
    { //bottom face
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    1.0f, -1.0f,  1.0f,  0.0f, 0.0f   
    }
};

GLuint load_cube_map_tx();
GLuint load_shaders(const char *vertex_file_path, const char *fragment_file_path);

struct instance_data {
    glm::mat4 model;
    glm::vec4 texture_id;
};

class pipeline_renderer {
    public:
        GLuint programID;
        GLuint texture;
        GLuint vertexBuffer;
        GLuint VertexArrayID;

        //load textures and stuff
        void init();

        void render(camera_perspective *cam);
    
        ~pipeline_renderer();
    private:
        void render_faces(const std::vector<glm::vec3> &faces, u32 chunk_x, u32 chunk_y, face_sides face);
};