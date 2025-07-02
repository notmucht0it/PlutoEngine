#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>


const GLuint WIDTH = 800, HEIGHT = 600;

const char *vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *fragmentShaderSource2 = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);



int main()
{
    //Intializes GLFW and set ups window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
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

    //Sets GL rendering to specific portion of the window
    glViewport(0, 0, WIDTH, HEIGHT);
    // Handles resizing so window doesnt crash
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Sets callback so when a key pressed that function is excuted
    glfwSetKeyCallback(window, key_callback);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShaders[2];
    fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaders[0],1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShaders[0]);

    glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::1::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaders[1],1,&fragmentShaderSource2,NULL);
    glCompileShader(fragmentShaders[1]);

    glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::2::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderPrograms[2];
    shaderPrograms[0] = glCreateProgram();

    glAttachShader(shaderPrograms[0],vertexShader);
    glAttachShader(shaderPrograms[0],fragmentShaders[0]);
    glLinkProgram(shaderPrograms[0]);

    glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderPrograms[0], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    shaderPrograms[1] = glCreateProgram();

    glAttachShader(shaderPrograms[1],vertexShader);
    glAttachShader(shaderPrograms[1],fragmentShaders[1]);
    glLinkProgram(shaderPrograms[1]);

    glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderPrograms[1], 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaders[0]);
    glDeleteShader(fragmentShaders[1]);

    float vertices1[] = {
        -0.25f,  -0.25f, 0.0f,  // top right
        0.0f, -0.25f, 0.0f,  // bottom right
        -0.125f, 0.25f, 0.0f,  // bottom left
    };
    float vertices2[] = {
        0.25f,-0.25f, 0.0f,
        0.0f, -0.25f, 0.0f,
        0.125f, 0.25f, 0.0f,
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  


    unsigned int VBOs[2], VAOs[2], EBO;
    glGenVertexArrays(2,VAOs);
    glGenBuffers(2, VBOs);
    // glGenBuffers(1,&EBO);
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


    while(!glfwWindowShouldClose(window)){
        // Runs through possible events, key press, resizing, etc.
        glfwPollEvents(); 

        // Clears the color buffer so if things where moving they are not left on the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderPrograms[0]);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderPrograms[1]);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(2,VAOs);
    glDeleteBuffers(2,VBOs);
    glDeleteProgram(shaderPrograms[0]);
    glDeleteProgram(shaderPrograms[1]);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}