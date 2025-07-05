#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "app/window.hpp"
#include "input/input.hpp"
#include "render/shader.hpp"
#include "PlutoMath/plutomath.hpp"
#include "render/render.hpp"

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

constexpr GLuint WIDTH = 800, HEIGHT = 600;
constexpr float WID = 800.0, HIGH = 600.0f;

int main(){

    window win(WID,HIGH);
    if (win.initialize() != 0) throw std::runtime_error("Initialization failed");
    const Shader ourShader("shaders/shader.vs","shaders/shader.fs");
    auto control = input(win.get_window(),WID,HIGH,plutom::vec3f(0.0f,0.0f,-3.0f));

    auto renderer = Renderer(win.get_window());
    renderer.add_shader(ourShader);

    for (unsigned int i = 0; i < 15; ++i) {
        renderer.add_shape(i,"cube",plutom::vec3f(i, 0.1f*i,3.0f*i),plutom::vec3f(0.5f),
                    plutom::vec3f(1.0f, 0.3f, -0.5f),plutom::vec3f(1.0f),i * 10.0f,
                    0.0f,36.0f,false,"../res/wall.jpg");
    }

    while(!glfwWindowShouldClose(win.get_window())){

        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        control.update_delta(deltaTime);
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.visualize(control.get_camera(),WID/HIGH,deltaTime);
        glfwSwapBuffers(win.get_window());
    }
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

