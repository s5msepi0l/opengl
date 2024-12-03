#include "world.h"


//populate w some block so i can get things going
void world::init() {
    update_distance(DEFAULT_CHUNK_DISTANCE);
    for (int x = 0; x<16; x++) {
        for (int z = 0; z<16; z++) {
            chunks->data[x][0][z]->id = BLK_GRASS;
        }
    }
}

world::~world() {
    for (int x = 0; x<16; x++) {
        for (int y = 0; y<256; y++) {
            for (int z = 0; z<16; z++) {
                delete chunks->data[x][y][z];
            }
        }
    }

    delete chunks->data;
}

void world::update_distance(u32 distance) {
    if (chunks->data == nullptr) { //initial allocation
        for (int x = 0; x<16; x++) {
            for (int y = 0; y<256; y++) {
                for (int z = 0; z<16; z++) {
                    chunks->data[x][y][z] = new block;
                }
            }
        }
    }
}

//update position and shift cached chunks if needed
void world::update(glm::vec3 pos) {

}

//get block from world position (as long as its within a loaded chunk<)
inline block *world::get(glm::vec3 pos) {
    i64 chunk_x = (u32)pos.x / 16 + (u32)pos.x % 16;
    i64 chunk_z = (u32)pos.z / 16 + (u32)pos.z % 16;
    if (chunk_x < 0 || chunk_z < 0) {
        return nullptr;
    }

    return chunks->data[(i32)pos.x][(i32)pos.y][(i32)pos.z];
}