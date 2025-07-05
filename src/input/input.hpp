// input.hpp
#ifndef INPUT_HPP
#define INPUT_HPP

#include "camera.hpp"
#include <GLFW/glfw3.h>

class input {
public:
    input(GLFWwindow* window, float width, float height, const plutom::vec3f& cam_pos);
    void update_delta(float new_delta);
    Camera& get_camera();

private:
    Camera free_move;
    GLFWwindow* window;
    float lastX, lastY;
    float deltaTime;
    bool first_mouse;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double y_pos);
    static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
};

#endif