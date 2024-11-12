#ifndef PLATFORMER_RENDERER
#define PLATFORMER_RENDERER

#include "util.h"
#include "sprite.h"

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

class pipeline_renderer {
    private:
        GLFWwindow *window;
    public:
        pipeline_renderer(GLFWwindow *window);
    private:
        
};


#endif