//
// Created by kevin on 7/4/25.
//

#ifndef RENDER_HPP
#define RENDER_HPP
#include <vector>
#include "../PlutoMath/plutomath.hpp"
#include "shader.hpp"
#include "../input/camera.hpp"
#include "GLFW/glfw3.h"

struct Shape {
    std::string type;
    std::string tag;
    unsigned int shaderID;
    unsigned int id;
    unsigned int vertexCount;

    unsigned int VAO;
    unsigned int VBO;
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

class Renderer {
public:
    explicit Renderer(GLFWwindow* window);

    void add_shader(const Shader& shader);
    void add_shape(const char *type, const plutom::vec3f &position, float rotation_speed);
    void add_shape(unsigned int id, const std::string &type, const plutom::vec3f &position, const plutom::vec3f &color,
                   const plutom::vec3f &rotationAxis, const plutom::vec3f &scaling, float rotation_speed,
                   float rotation_angle, float shininess, bool hasTexture, const std::string& textureFile);
    void visualize(const Camera &cam, float ratio, float deltaTime);

private:
    GLFWwindow* window;
    std::vector<Shader> shaders;
    std::vector<Shape> shapes;
    unsigned int currentID = 0;
    unsigned int lastShader = -1;

    static unsigned int load_texture(const char* filepath, bool flip);
    static void create_shape(Shape &shape,const char *file = "temp");
};



#endif //RENDER_HPP
