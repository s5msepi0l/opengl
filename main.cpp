
#define STB_IMAGE_IMPLEMENTATION

#include "state.h"
#include "lib/engine/game_engine.h"
#include "components/player.h"

int main(int argc, const char **argv) {
    glewExperimental = true;
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        
    }

    glfwWindowHint(GLFW_SAMPLES, 4);                                    // anti-aliasing samples
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (argc >= 3) {
        state.width     = atoi(argv[1]);
        state.height    = atoi(argv[2]);
        state.framerate = atoi(argv[3]);
    }

    state.window = glfwCreateWindow(state.width, state.height, "Block game", nullptr, nullptr);
    if (state.window == nullptr) {
        std::cerr << "Failed to open GLFW window\n";
        std::cout << "width: " << state.width << "\n" << state.height << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(state.window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed\n";
        return -1;
    }

    state.renderer->init();

    game_engine::deltatime Deltatime;
    game_engine::frametime_manager frametime;
    frametime.set_framerate(state.framerate);

    state.game->init();

    Player player;
    state.entities->add_entity("Player", &player);
    state.renderer->init();

    while(glfwWindowShouldClose(state.window) == 0 && !glfwGetKey(state.window, GLFW_KEY_ESCAPE)){
        std::cout << "tick\n";
        frametime.set_start();
        state.deltatime = Deltatime.update();

        state.game->tick();
        state.renderer->render(
            static_cast<camera_perspective*>(player.transform.data)
            
        );

        glfwSwapBuffers(state.window);
        glfwPollEvents();  

        frametime.set_end();
    }

    glfwTerminate();
}