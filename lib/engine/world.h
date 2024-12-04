#pragma once

#define DEFAULT_CHUNK_DISTANCE 8

#include "../../state.h"
#include "util.h"

#define BLK_AIR   0
#define BLK_GRASS 1

#define CHUNK_WIDTH  16
#define CHUNK_DEPTH  16
#define CHUNK_HEIGHT 256


/*
#define FACE_COVERED 1
#define FACE_CLEAR   0
*/
//which sides are covered by other blocks, 1 = covered

enum class face_sides{
    front_face = 1,
    back_face = 2,
    left_face = 3,
    right_face = 4,
    top_face = 5,
    bottom_face = 6
};
typedef struct {
    std::vector<glm::vec3> front;
    std::vector<glm::vec3> back;
    std::vector<glm::vec3> left;
    std::vector<glm::vec3> right;
    std::vector<glm::vec3> top;
    std::vector<glm::vec3> bottom;
}chunk_faces;

typedef struct world_faces{
    world_faces(u32 x_mem, u32 y_mem):
    chunks(x_mem, std::vector<chunk_faces>(y_mem)) {}

    std::vector<std::vector<chunk_faces>> chunks;
    size_t size;
}world_faces;

typedef struct {
    u8 id = 0; //air
    
    /*are they transluscent/transparent, 1 = opaque */
    u8 prop = 1;

    face_sides faces;
    //light levels and so on

    inline bool is_opaque() const {
        (prop == 1) ? true : false;
    } 
}block;

typedef struct {
    block data[16][256][16];
    glm::vec3 pos;
}chunk;

class world{
    public:
        std::vector<std::vector<chunk>> chunks;
        u32 size;

        glm::vec3 position;

    public:
        
        void update_distance(u32 distance);

        world_faces face_calc();

        chunk_faces chunk_calc(u32 chunk_x, u32 chunk_y);
        void init();
        void update(glm::vec3 pos);


        block *get(glm::vec3 pos);

        ~world();
};