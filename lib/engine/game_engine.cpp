#include "game_engine.h"

void game_engine::frametime_manager::set_framerate(u32 framerate) {
    frame_time = std::floor(1000.0 / framerate);
}

void game_engine::frametime_manager::set_start() {
    start_time = glfwGetTime();
}

// as long as there isn't any threading running this should work 
void game_engine::frametime_manager::set_end() {
    elaplsed_time = glfwGetTime() - start_time;
    if (elaplsed_time < frame_time) {
        sleep(frame_time);
    }
}

std::string game_engine::entity::get_name() const{
    return this->name;
}

void game_engine::entity::set_name(std::string new_name) {
    this->name = new_name;
}

inline f64 game_engine::entity::distance_to(game_engine::entity &other) {
    return sqrt(
        pow((this->transform.pos.x - other.transform.pos.x), 2) +
        pow((this->transform.pos.y - other.transform.pos.y), 2) +
        pow((this->transform.pos.z - other.transform.pos.z), 2)
        );
}

void game_engine::entity::tick() {
        std::cout << "game_engine::entity::Position: \nglm::vec3(" << transform.pos.x << ", " 
                  << transform.pos.y << ", " << transform.pos.z << ")\n";


    for (u32 i = 0; i<comps.size(); i++) {
        comps[i]->tick();
    }
}

void game_engine::entity::add_component(std::unique_ptr<component> comp) {
    std::cout << "adding component now\n";

    comp->transform = &this->transform;
    comp->start();
    comps.push_back(std::move(comp));
}

void game_engine::entity::set_collision(bool state, entity *other) {
                   std::cout << "SET_COLLISION\n";
    if (this->colliding && !state) {
        for (u32 i = 0; i<comps.size(); i++) {
            comps[i]->collision_exit(this->colliding_partner);
            this->colliding = false;
        }
    }
                
    if (!this->colliding && state) {
        for (u32 i = 0; i<comps.size(); i++) {
            comps[i]->collision_enter(other);
            this->colliding_partner = other;
            this->colliding = true;
        }
    }
                
    if (this->colliding && state) {
        for (u32 i = 0; i<comps.size(); i++) {
            comps[i]->collision_exit(this->colliding_partner);
            comps[i]->collision_enter(other);
        }
    }   
}

game_engine::entity::~entity() {
    std::cout << "destruct\n";
    for (u32 i = 0; i < comps.size(); i++) {
        comps[i]->stop(); //
    }
}