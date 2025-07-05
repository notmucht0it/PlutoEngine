//
// Created by kevin on 7/4/25.
//

#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <glad/gl.h>
#include "GLFW/glfw3.h"



class window {
public:
    window(float width, float height);
    int initialize();
    GLFWwindow* get_window() const;
private:
    GLFWwindow* wind;
    float width;
    float height;
};



#endif //WINDOW_HPP
