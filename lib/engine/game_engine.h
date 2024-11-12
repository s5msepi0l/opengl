#ifndef PLATFORMER_GAME_ENGINE
#define PLATFORMER_GAME_ENGINE

#include "renderer.h"
#include "util.h"
#include <unistd.h>
#include <map>

#if defined(__linux__)

//nanoseconds 
#define sleep(ms) usleep(ms * 1000)

#else //windows

// no clue if this works or not
#include <synchapi.h>
#define sleep(ms) Sleep(ms) 

#endif


#define LIVE_entity_LIMIT 64

namespace game_engine {
    f32 Delta_time = 0;

    class component;
    class entity;


    class ecs {
        public:
            
            /* Live objects should be used very sparingly they're 
            collision detection system is quite shit and slow */
            std::map<std::string, entity*> live_objects;
            
            /* does NOT check collision with other objects
            Static objects update script is executed after
            live objects but are much less computationally expensive */
            std::map<std::string, entity*> static_objects;
            
            i32 object_count() { return live_objects.size() + static_objects.size(); }    
    };

    class deltatime {
        public:
            deltatime() : last_time(0.0), delta_time(0.0) {}

            inline const f64 update() {
                f64 current_time = glfwGetTime();
                delta_time = current_time - last_time;
                last_time = current_time;

                return delta_time;
            }


        private:
            f64 last_time;
            f64 delta_time;
    };

    //manages the amount of time should be slept to keep a consistent framerate
    class frametime_manager {
        private:
            f32 frame_time;

            u64 start_time;
            u64 elaplsed_time;

        public:
            inline void set_framerate(u32 framerate);
            void set_start();
            void set_end();
    };

    class component {
        public:
            //used to change the objects position and whatnot
            entity *self;

            virtual void start() = 0;
            virtual void stop() = 0;
            virtual void update() = 0;
            virtual void collision_enter(entity *other) = 0;
            virtual void collision_exit(entity *other) = 0;
    };

    //if size == 0 collision is just not checked
    //more or less just a template for storing game data
    class entity {
        public:

            // something something opengl magic model

            
            // every object just has a basic square hitbox
            glm::vec3 pos;
            glm::vec3 size;
            
            bool colliding = false;
            entity *colliding_partner = nullptr;

        private:
            std::string name;
            std::vector<component *> comps;

        public:
            virtual void init() = 0;

            std::string get_name() const;
            void set_name(std::string new_name);
            void add_component(component *comp);
            inline f64 distance_to(entity &other);
            void tick();

            void set_collision(bool state, entity *other);

            ~entity();
    };

    class engine {
        public:
            engine(GLFWwindow *window):
            renderer(window) {}

            ~engine() { }

        private:
            //runtime variables 
            pipeline_renderer renderer;
            frametime_manager frametime;
            ecs entities;

            bool running = true;

        public:
            // return true if succesfull
            bool add_object(entity *object) {
                if (entities.live_objects.size() < LIVE_entity_LIMIT) {
                    entities.live_objects[object->get_name()] = object;
                    return true;
                }

                return false;
            }

            void add_static_object(entity *object) {
                entities.static_objects[object->get_name()] = object;
            }

            //return exit code, normal: 0, general error: -1
            u8 tick() {
                while(this->running) {
                    frametime.set_start();
                    

                    frametime.set_end();
                }

                return 0;
            }

        private:

            //kinda "hackey" and only allows for retangular hitboxes
            void collision_detection() {
                std::cout << "collision_detection\n";
                for (auto &live_obj: entities.live_objects) {
                    for (auto &static_obj: entities.static_objects) {
                        //mind fuck trying to debug                        
                        if (live_obj.second->pos.x < static_obj.second->pos.x + static_obj.second->size.x &&
                            live_obj.second->pos.x + live_obj.second->size.x > static_obj.second->pos.x &&
                            live_obj.second->pos.y < static_obj.second->pos.y + static_obj.second->size.y &&
                            live_obj.second->pos.y + live_obj.second->size.y > static_obj.second->pos.y) {
                            //collision detected            
                            live_obj.second->set_collision(true, static_obj.second);
                        
                            std::cout << "true\n";
                        } else {
                            live_obj.second->set_collision(false, static_obj.second);
                            std::cout << "false\n";
                        }

                    }
                }
            } 
    };
};

#endif