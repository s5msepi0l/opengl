#include "engine/game_engine.h"

int main(int argc, const char **argv) {

    glfwWindowHint(GLFW_SAMPLES, 4);                                    // anti-aliasing samples
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    u32 width       = 1280;
    u32 height      = 720;
    u32 framerate   = 60;

    if (argc >= 3) {
        width       = atoi(argv[1]);
        height      = atoi(argv[2]);
        framerate   = atoi(argv[3]);
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "Block game", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    game_engine::engine engine(window);
    game_engine::deltatime Deltatime;

    game_engine::frametime_manager frametime;
    frametime.set_framerate(framerate);


    do {
        frametime.set_start();
        const f64 deltatime = Deltatime.update();




        frametime.set_end();
    }while(glfwWindowShouldClose(window) == 0 && !glfwGetKey(window, GLFW_KEY_ESCAPE));

}