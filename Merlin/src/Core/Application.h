#ifndef MERLINGL_APPLICATION_H
#define MERLINGL_APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Merlin {
    class Application {
    public:
        Application();
        ~Application();

        void run();
        void quit();
    private:
        GLFWwindow* window;
    };
}

#endif //MERLINGL_APPLICATION_H
