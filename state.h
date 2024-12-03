#pragma once

#include <memory>
#include "lib/engine/util.h"
#include "lib/engine/renderer.h"
#include "lib/engine/world.h"

//using namespace game_engine;

/* jesus crist this piece of shit took me goddamn hour just to get working across files
*/

namespace game_engine {
    class engine;
    class ecs;
};

class pipeline_renderer;


struct State {
    public:
    u32 width     = 1920;
    u32 height    = 1080;
    u32 framerate = 60;

    std::unique_ptr<game_engine::engine> game;
    std::unique_ptr<game_engine::ecs> entities;

    f32 deltatime;

    GLFWwindow *window;
    std::unique_ptr<pipeline_renderer> renderer;
    State();
};


extern State state;