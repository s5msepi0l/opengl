#include "world.h"

//populate w some block so i can get things going
void world::init() {
    update_distance(DEFAULT_CHUNK_DISTANCE);
    for (int x = 0; x<16; x++) {
        for (int z = 0; z<16; z++) {
            block *current = get({x, 0, z});
            current->id   = BLK_GRASS;
            current->prop = 0;
        }
    }
}

//update position and shift cached chunks if needed
void world::update(glm::vec3 pos) {

}

world::~world() {
    
}

void world::update_distance(u32 distance) {

}


/* Before every frame every loaded chunk is iterated through to check the block facing
and coverings so that the gpu doesn't need to render blocks that are underground and so on,
returns a matrix of every loaded chunk with a 6 byte struct containing the face information
on every block, in total for a 8 chunk rendering distance should at most 312 megabytes (i think)
64 chunks * 6 face arrays each holding maximum of 65536 * 13 bytes
*/
world_faces world::face_calc() {
    world_faces faces(this->size, this->size);

    // this is gonna take a while to execute (might) have to think of smthing else if its too slow
    for (u32 chunk_x = 0; chunk_x < this->size; chunk_x++) {
        for (u32 chunk_y = 0; chunk_y < this->size; chunk_y++) {
            chunk_faces face = chunk_calc(chunk_x, chunk_y);
            
            faces.chunks[chunk_x][chunk_y] = face;
        }
    }
    return faces;
}

chunk_faces world::chunk_calc(u32 chunk_x, u32 chunk_y) {
    chunk_faces face;
    
    for (u32 x = 0; x<CHUNK_WIDTH; x++) {
        for (u32 y = 0; y<CHUNK_HEIGHT; y++) {
            for (u32 z = 0; z<CHUNK_WIDTH; z++) {

                /*  opengl requires glm::vec3 in the form of a float */
                block *current = get({x, y, z});
                if (current->is_opaque()) {
                    if (get({x + 1 , y, z})->is_opaque()) face.right.push_back({x, y, z});
                    if (get({x - 1 , y, z})->is_opaque()) face.left.push_back({x, y, z});
                    if (get({x, y, z + 1})->is_opaque()) face.front.push_back({x, y, z});
                    if (get({x, y, z - 1})->is_opaque()) face.back.push_back({x, y, z});
                    if (get({x, y + 1, z})->is_opaque()) face.top.push_back({x, y, z});
                    if (get({x, y - 1, z})->is_opaque()) face.bottom.push_back({x, y, z});
                }
            }
        }
    }

    return face;
}

//get block from world position (as long as its within a loaded chunk<)
inline block *world::get(glm::vec3 pos) {
    i64 chunk_x = (u32)pos.x / 16 + (u32)pos.x % 16;
    i64 chunk_z = (u32)pos.z / 16 + (u32)pos.z % 16;
    if (chunk_x < 0 || chunk_z < 0) {
        return nullptr;
    }

    return &chunks[chunk_x][chunk_z].data[(i32)pos.x][(i32)pos.y][(i32)pos.z];
}