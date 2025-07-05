//
// Created by kevin on 7/4/25.
//

#ifndef RENDER_HPP
#define RENDER_HPP
#include <memory>
#include <vector>
#include "../PlutoMath/plutomath.hpp"
#include "shader.hpp"
#include "../input/camera.hpp"
#include "../util/primativegenerator.hpp"
#include "GLFW/glfw3.h"

enum class ShaderType { Basic, Lighting, Source };

struct Shape {
    ShaderType sType;
    std::string type;
    std::string tag;
    unsigned int shaderID;
    unsigned int id;
    unsigned int indicesCount;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int textureID;

    plutom::vec3f color;
    plutom::vec3f position;
    plutom::vec3f rotationAxis;
    plutom::vec3f scalingVector;

    bool wireframe;
    bool visible;
    bool hasTexture;

    float shininess;
    float rotationSpeed;
    float rotationAngle;
};

struct ShapeDescriptor {
    std::string type = "cube";
    ShaderType sType = ShaderType::Basic;
    plutom::vec3f color = {1.0f, 1.0f, 1.0f};
    plutom::vec3f position = {0.0f, 0.0f, 0.0f};
    plutom::vec3f rotationAxis = {0.0f, 1.0f, 0.0f};
    plutom::vec3f scalingVector = {1.0f, 1.0f, 1.0f};
    float rotationSpeed = 0.0f;
    float rotationAngle = 0.0f;
    float shininess = 32.0f;
    bool wireframe = false;
    bool visible = true;
    bool hasTexture = false;
    std::string tag = "none";
    std::string texturePath = "res/awesomeface.png";
};

class Renderer {
public:
    explicit Renderer(GLFWwindow* window);

    void add_shader(const std::shared_ptr<Shader>& shader);
    void add_shape(const ShapeDescriptor& desc);
    void visualize(const Camera &cam, float ratio, float deltaTime);

private:
    GLFWwindow* window;
    std::vector<std::shared_ptr<Shader>> shaders;
    std::vector<Shape> shapes;
    unsigned int currentID = 0;
    unsigned int lastShader = -1;

    unsigned int axisVAO{}, axisVBO{};
    std::shared_ptr<Shader> axisShader;

    static unsigned int load_texture(const char* filepath, bool flip);
    static void create_shape(Shape &shape,const char *file = "temp");
};



#endif //RENDER_HPP
