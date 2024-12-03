#pragma once


#include "../../state.h"
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

    struct Transform{
                    
        // every object just has a basic square hitbox
        // i (might) try and add a support later 
        //for more complex hitboxes via making the size an array of vec3
        glm::vec3 pos;
        glm::vec3 size;
                
        //google said ts was standard practice so imma js keep it until it breaks smt
        direction dir;

        void *data = nullptr;
    };

    class component;
    //if size == 0 collision is just not checked
    //more or less just a template for storing game data
    class entity {
        public:
            // something something opengl magic model, 
            GLuint model;
            
            struct Transform transform;
            
            bool colliding = false;
            entity *colliding_partner = nullptr;

        private:
            std::string name;
            std::vector<std::unique_ptr<component>> comps;

        public:
            // Used to add compontents upon instancing
            virtual void init() = 0;

            std::string get_name() const;
            void set_name(std::string new_name);

            void add_component(std::unique_ptr<component> comp);
            
            inline f64 distance_to(entity &other);
            
            void tick();

            void set_collision(bool state, entity *other);

            ~entity();
    };

    class ecs {
        public:
            //true if succesfull
            bool add_entity(const std::string &name, entity *obj) {
                if (live_entities_map.find(name) == live_entities_map.end()) {
                    
                    //call constructor
                    
                    obj->init();
                    obj->set_name(name);
                    
                    
                    live_entities_map[name] = obj;
                    live_entities.push_back(obj);
                    return true;
                }

                return false;
            }
    
            entity* get_entity(const std::string& name) {
                return live_entities_map.count(name) ? live_entities_map[name] : nullptr;
            }

            std::vector<entity*>& get_entities() {
               return live_entities;
            }

            //true if succesfull
            bool add_static_entity(const std::string &name, entity *obj) {
                if (static_entities_map.find(name) == static_entities_map.end()) {
                    
                    //call constructor
                    //obj->init();
                    
                    static_entities_map[name] = obj;
                    static_entities.push_back(obj);
                    return true;
                }

                return false;
            }
    
            entity* get_static_entity(const std::string& name) {
                return live_entities_map.count(name) ? live_entities_map[name] : nullptr;
            }

            std::vector<entity*>& get_static_entities() {
               return live_entities;
            }
    
    
            /* does NOT check collision with other objects
            Static objects update script is executed after
            live objects but are much less computationally expensive */
            
            
            i32 object_count() { return live_entities.size() + static_entities.size(); }    

        private:
            std::unordered_map<std::string, entity*> static_entities_map;
            std::vector<entity*> static_entities;


            
            std::unordered_map<std::string, entity*> live_entities_map;
            std::vector<entity*> live_entities;
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
            void set_framerate(u32 framerate);
            void set_start();
            void set_end();
    };

    class component {
        public:
            //used to change the objects position and whatnot
            //I'm not really super happy with it as it is quite
            // "hacky" but I've been stuck on it for an hour
            // and i can't think of anything better atm
            // this does allow for more freely passing data
            // to components but adds alot of boilerplate code
            struct Transform *transform;


            virtual void start() = 0;
            virtual void stop() = 0;
            virtual void tick() = 0;
            virtual void collision_enter(entity *other) = 0;
            virtual void collision_exit(entity *other) = 0;
    };

    class engine {
        public:
            void init() {
                
            }

            ~engine() { }

        private:

            bool running = true;

        public:

            //return exit code, normal: 0, general error: -1
            u8 tick() {
                std::vector<entity*> entities = state.entities->get_entities();

                for (u32 i = 0; i<entities.size(); i++) {
                    std::cout << "ticking() ";
                    entities[i]->tick();
                }

                return 0;
            }

        private:

            //kinda "hackey" and only allows for retangular hitboxes
            void collision_detection() {
                std::cout << "collision_detection()\n";
                
                std::vector<entity*> live_entities = state.entities->get_entities();
                std::vector<entity*> static_entities = state.entities->get_static_entities();

                for (entity *live_obj: live_entities) {
                    for (entity *static_obj: static_entities) {
                
                        //mind fuck trying to debug                        
                        if (live_obj->transform.pos.x < static_obj->transform.pos.x + static_obj->transform.size.x &&
                            live_obj->transform.pos.x + live_obj->transform.pos.x > static_obj->transform.pos.x &&
                            live_obj->transform.pos.y < static_obj->transform.pos.y + static_obj->transform.size.y &&
                            live_obj->transform.pos.y + live_obj->transform.size.y > static_obj->transform.pos.y) {
                            //collision detected            
                            live_obj->set_collision(true, static_obj);
                            std::cout << "true\n";
                        } else {
                            live_obj->set_collision(false, static_obj);
                            std::cout << "false\n";
                        }

                    }
                }
            } 
    };
};