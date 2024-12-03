#pragma once

#define DEFAULT_CHUNK_DISTANCE 8

#include "../../state.h"
#include "util.h"

#define BLK_AIR   0
#define BLK_GRASS 1

//which sides are covered by other blocks
typedef struct {
    u8 f = 0;
    u8 b = 0;
    u8 l = 0;
    u8 r = 0;
    u8 u = 0;
    u8 d = 0;
}face;

typedef struct {
    u8 id = 0; //air
    face faces;
    //light levels and so on
}block;

typedef struct {
    block *data[16][256][16];
    glm::vec3 pos;
}chunk;

class world{
    public:
        chunk *chunks;
        u32 size;

        glm::vec3 position;

    public:
        
        void update_distance(u32 distance);

        void init();
        void update(glm::vec3 pos);
        block *get(glm::vec3 pos);

        ~world();
};