#include "state.h"
#include "lib/engine/game_engine.h" // For the engine and ecs classes
#include "lib/engine/renderer.h" // For the engine and ecs classes


State state;

State::State() {
    game     = std::make_unique<game_engine::engine>();
    entities = std::make_unique<game_engine::ecs>();
    renderer = std::make_unique<pipeline_renderer>();
}