#include "window.hpp"
#include <iostream>

window::window(float width, float height) {
    this->width = width;
    this->height = height;
}

int window::initialize() {
    //Intializes GLFW and set ups window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(this->width, this->height, "LearnOpenGL", nullptr, nullptr);
    this->wind = window;
    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //Load glad (needed for graphics calls)
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    //Sets GL rendering to specific portion of the window
    glViewport(0, 0, this->width, this->height);
    return 0;
}

GLFWwindow* window::get_window() const {
    return this->wind;
}
