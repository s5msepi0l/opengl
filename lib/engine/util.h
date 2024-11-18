#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <map>
#include <list>
#include <unordered_map>
#include <memory>
#include <sstream>

#include "../../state.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace std {
    //generally used for holding ascii values or 
    typedef basic_string<unsigned char> ustring;
}

//compiler doesn't really like when i try to use a u8 as a boolean so im just gonna use a regular bool 
#define f32 float
#define f64 double 

#define i8 int8_t
#define u8 uint8_t
#define i16 int16_t  
#define u16 uint16_t  
#define u32 uint32_t 
#define i32 int32_t
#define u64 uint64_t
#define i64 int64_t

// Not really used for holding info, mostly just passing
typedef struct {
    glm::vec3 pos;

    struct dir{
        glm::vec3 forwards;
        glm::vec3 right;
        glm::vec3 up;
    }dir;
    
    f32 fov = 90;
}camera_perspective;  

//general coordinate struct || use glm::vec3 instead
typedef struct fvec3 {
    f32 x;
    f32 y;

    fvec3(): x(0), y(0) {}
    fvec3(f32 X, f32 Y): x(X), y(Y) {}


    // √(x_2-x_1)²+(y_2-y_1)²
    inline f32 distance_to(fvec3 other) const {
        return static_cast<f32>(sqrt(pow(other.x, 2) + pow(other.y, 2)));
    }

    inline f32 distance_to(f32 x_pos, f32 y_pos) const {
        return static_cast<f32>(sqrt(pow(x_pos, 2) + pow(y_pos, 2)));
    }

    void operator+=(fvec3 other) {
        x += other.x;
        y += other.y;
    }

    inline fvec3 operator*(f32 op) {
        return fvec3(this->x * op, this->y * op);
    }

}fvec3;

template <typename T>
class buf3 {
private:
    std::vector<T> data;

public:
    u32 width = 0;
    u32 height = 0;
    u32 length = 0;

    buf3() {}

    buf3(u32 w, u32 h, u32 l):
    data(w * h * l),
    width(w),
    height(h),
    length(l)
    {}

    // get direct data pointer
    std::vector<T> ref() const { return this->data; }
    inline bool is_empty() const { return data.empty(); }

    T *operator[](int index) {
        return &this->data[index * width];
    }

    const T *operator[](int index) const {
        return &data[index * width];
    }

};

template <typename key_type, typename value_type>
class lru_cache {
private:
    int capacity;
    std::list<std::pair<key_type, value_type>> cache_list; // Represents the cache as a doubly linked list of key-value pairs
    std::unordered_map<key_type, typename std::list<std::pair<key_type, value_type>>::iterator> cache_map; // Maps keys to iterators in the list

public:
    lru_cache(int capacity) : capacity(capacity) {}

    value_type get(const key_type& key) {
        auto it = cache_map.find(key);

        if (it == cache_map.end()) {
            return value_type(); 
        }

        cache_list.splice(cache_list.begin(), cache_list, it->second);

        return it->second->second;
    }

    void put(const key_type& key, const value_type& value) {
        auto it = cache_map.find(key);
        
        if (it != cache_map.end()) {
            it->second->second = value;
            cache_list.splice(cache_list.begin(), cache_list, it->second);
        } else {
            if (cache_list.size() == capacity) {
                key_type old_key = cache_list.back().first;
                cache_map.erase(old_key);
                cache_list.pop_back();
            }

            cache_list.emplace_front(key, value);
            cache_map[key] = cache_list.begin();
        }
    }
};