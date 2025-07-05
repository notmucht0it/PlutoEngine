// input.cpp
#include "input.hpp"

input::input(GLFWwindow* win, float width, float height, const plutom::vec3f& cam_pos)
    : free_move(cam_pos), window(win), lastX(width / 2.0f), lastY(height / 2.0f), deltaTime(0), first_mouse(true)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void input::update_delta(float new_delta) {
    deltaTime = new_delta;
}

Camera& input::get_camera() {
    return free_move;
}

// Static Callbacks: extract Input* from window
void input::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* inp = static_cast<input*>(glfwGetWindowUserPointer(window));
    if (!inp) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_W) inp->free_move.ProcessKeyboard(FORWARD, inp->deltaTime);
        if (key == GLFW_KEY_S) inp->free_move.ProcessKeyboard(BACK, inp->deltaTime);
        if (key == GLFW_KEY_A) inp->free_move.ProcessKeyboard(LEFT, inp->deltaTime);
        if (key == GLFW_KEY_D) inp->free_move.ProcessKeyboard(RIGHT, inp->deltaTime);
    }
}

void input::mouse_callback(GLFWwindow* window, const double xpos, const double y_pos) {
    auto* inp = static_cast<input*>(glfwGetWindowUserPointer(window));
    if (!inp) return;

    const auto x = static_cast<float>(xpos);
    const auto y = static_cast<float>(y_pos);

    if (inp->first_mouse) {
        inp->lastX = x;
        inp->lastY = y;
        inp->first_mouse = false;
    }

    const float x_offset = x - inp->lastX;
    const float y_offset = inp->lastY - y;

    inp->lastX = x;
    inp->lastY = y;

    inp->free_move.ProcessMouseMovement(x_offset, y_offset);
}

void input::scroll_callback(GLFWwindow* window, double x_offset, const double y_offset) {
    auto* inp = static_cast<input*>(glfwGetWindowUserPointer(window));
    if (!inp) return;
    inp->free_move.ProcessMouseScroll(static_cast<float>(y_offset));
}