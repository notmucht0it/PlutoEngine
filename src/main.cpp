#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "PlutoMath/transform.hpp"
#include "util/shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "PlutoMath/plutomath.hpp"

constexpr GLuint WIDTH = 800, HEIGHT = 600;

constexpr float WID = 800.0, HIGH = 600.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

plutom::vec3f cameraPos   = plutom::vec3f(0.0f, 0.0f,  3.0f);
plutom::vec3f cameraFront = plutom::vec3f(0.0f, 0.0f, -1.0f);
plutom::vec3f cameraUp    = plutom::vec3f(0.0f, 1.0f,  0.0f);


float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float percent = 0.0f;

int main(){

    //Intializes GLFW and set ups window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
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
    glViewport(0, 0, WIDTH, HEIGHT);
    // Handles resizing so window doesnt crash
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Sets callback so when a key pressed that function is excuted
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    const Shader ourShader("shaders/shader.vs","shaders/shader.fs");

    constexpr float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    unsigned int VBO, VAO, EBO, textures[2];
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load("res/wall.jpg",&width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("res/awesomeface.png",&width, &height, &nrChannels, 0);
    if(data){
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture2", 1);

    //std::cout << trans << std::endl;

    const plutom::vec3f cubePositions[] = {
        plutom::vec3f(  0.0f,  0.0f,  0.0f),
        plutom::vec3f( 2.0f,  5.0f, -15.0f),
        plutom::vec3f(-1.5f, -2.2f, -2.5f),
        plutom::vec3f(-3.8f, -2.0f, -12.3f),
        plutom::vec3f( 2.4f, -0.4f, -3.5f),
        plutom::vec3f(-1.7f,  3.0f, -7.5f),
        plutom::vec3f( 1.3f, -2.0f, -2.5f),
        plutom::vec3f( 1.5f,  2.0f, -2.5f),
        plutom::vec3f( 1.5f,  0.2f, -1.5f),
        plutom::vec3f(-1.3f,  1.0f, -1.5f)
    };

    while(!glfwWindowShouldClose(window)){

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Runs through possible events, key press, resizing, etc.
        glfwPollEvents(); 

        // Clears the color buffer so if things where moving they are not left on the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //float timeValue = glfwGetTime();
        //float posValue = (sin(timeValue) / 2.0f) + 0.5f;
        //ourShader.setFloat("horizontalOffset",posValue)
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        //const plutom::mat4f view = plutom::transform3D::translate(plutom::mat4f(1.0f),plutom::vec3f(0.0f,0.0f,-4.0f));
        const plutom::mat4f projection = plutom::perspective(plutom::radians(45.0f), WID/HIGH, 0.1f, 100.0f);

        ourShader.use();
        ourShader.setFloat("visible",percent);
        ourShader.setMat4f("projection", projection);

        glBindVertexArray(VAO);
        constexpr float radius = 10.0f;
        const float camX = sin(glfwGetTime()) * radius;
        const float camZ = cos(glfwGetTime()) * radius;
        const plutom::mat4f view = plutom::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


        ourShader.setMat4f("view", view);
        for (unsigned int i = 0; i < 10; ++i) {
            // First translate

            //float angle = 20.0f * static_cast<float>(i)* glfwGetTime();
            float angle = 20.0f * i;
            if(i % 3 == 4)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
                angle = glfwGetTime() * 25.0f;
            // Then rotate *on the translated matrix*
            plutom::mat4f model = plutom::transform3D::translate(plutom::mat4f(1.0f), cubePositions[i]);
            //plutom::mat4f model = plutom::transform3D::translate(plutom::mat4f(1.0f), plutom::vec3f(1.0f, 0.3f, -0.5f));
            model = plutom::transform3D::rotate(model ,plutom::radians(angle), plutom::vec3f(1.0f, 0.3f, -0.5f));
            ourShader.setMat4f("model", model);


            glDrawArrays(GL_TRIANGLES, 0, 36);

        }


        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height){
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_UP && action == GLFW_PRESS && percent <= 0.9f)
        percent += 0.1f;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && percent >= 0.1f)
        percent -= 0.1;
    const float cameraSpeed = 10.0f * deltaTime; // adjust accordingly
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraPos += cameraSpeed * cameraFront;
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraPos -= cameraSpeed * cameraFront;
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraPos -= cameraUp.cross(cameraFront).normalize() * cameraSpeed;
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
        cameraPos += cameraUp.cross(cameraFront).normalize() * cameraSpeed;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0 && percent <= 0.9f)
        percent += 0.1f;
    if (yoffset < 0 && percent >= 0.1f)
        percent -= 0.1f;
}