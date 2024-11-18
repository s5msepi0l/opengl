#include "../lib/engine/game_engine.h"

using entity = game_engine::entity;

class player_controller: public game_engine::component {
    public:
        
        f32 mouse_speed = 0.01;
        f32 speed = 7.0;

        struct {
            f64 horizontal = 3.14f;
            f64 vertical   = 0;
        }angle; 

        void start() override {}

        void tick() override {
            f64 x_pos, y_pos;

            
        std::cout << "game_engine::component::player_controller::Position: \nglm::vec3(" << transform->pos.x << ", " 
                  << transform->pos.y << ", " << transform->pos.z << ")\n";

        std::cout << "deltatime: " << state.deltatime << '\n';
            

            glfwGetCursorPos(state.window, &x_pos, &y_pos);
            glfwSetCursorPos(state.window, state.width/2.0, state.height/2.0);

            angle.horizontal += mouse_speed * state.deltatime * (f32)state.width/2.0 - x_pos;
            angle.vertical   += mouse_speed * state.deltatime * (f32)state.height/2.0 - y_pos;
            
            transform->dir.forward = glm::vec3 (
                cos(angle.vertical) * sin(angle.horizontal),
                sin(angle.vertical),
                cos(angle.vertical) * cos(angle.horizontal)
            );

            transform->dir.right = glm::vec3(
                sin(angle.horizontal - 3.14f/2.0f),
                0,
                cos(angle.horizontal - 3.14f/2.0f)
            );

            transform->dir.up = glm::cross(transform->dir.right, transform->dir.forward);

            if (glfwGetKey(state.window, GLFW_KEY_W) == GLFW_PRESS) {
                transform->pos += transform->dir.forward * state.deltatime * speed;
            }
            
            if (glfwGetKey(state.window, GLFW_KEY_S) == GLFW_PRESS) {
                transform->pos -= transform->dir.forward * state.deltatime * speed;
            }
            
            if (glfwGetKey(state.window, GLFW_KEY_D) == GLFW_PRESS) {
                transform->pos += transform->dir.right * state.deltatime * speed;
            }
            
            if (glfwGetKey(state.window, GLFW_KEY_A) == GLFW_PRESS) {
                transform->pos -= transform->dir.right * state.deltatime * speed;
            }
            
        }

        void stop() override {}

        void collision_enter(entity *other) override {}
        void collision_exit(entity *other) override {}
};

class Player: public game_engine::entity {
    public:
    struct player_transform : public game_engine::base_transform{
        camera_perspective cam;  // New member to hold camera perspective
    };

    // Now the Player class has its own transform, which is a `player_transform`
    player_transform Transform;  // This will hold position, size, direction and the camera perspective


        void init() {
            std::cout << "init() Player\n";
            transform.pos.x = 5;
            transform.pos.y = 5;
            transform.pos.z = 5;

            std::unique_ptr<player_controller> variable = std::make_unique<player_controller>();
            variable->transform = &Transform;


            this->add_component(std::move(variable));
        }

    private:
};