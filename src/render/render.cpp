#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "render.hpp"
#include "../PlutoMath/plutomath.hpp"

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

Renderer::Renderer(GLFWwindow *window): window(window) {
    shaders.emplace_back("shaders/shader.vs","shaders/shader.fs");
}

void Renderer::add_shape(const char* type, const plutom::vec3f& position, const float rotation_speed){

    Shape shape;
    shape.type = type;
    if (this->lastShader < 0) throw std::range_error("No shaders have been added, create one before adding shapes");
    shape.shaderID = this->lastShader;
    shape.tag = "Not Assigned";
    shape.position = position;
    shape.rotationSpeed = rotation_speed;
    shape.id = this->currentID;
    this->currentID += 1;

    shape.color = plutom::vec3f(1.0f);
    shape.rotationAxis = plutom::vec3f(0.0f);
    shape.scalingVector = plutom::vec3f(1.0f);

    shape.wireframe = false;
    shape.visible = true;
    shape.shininess = 32.0f;
    shape.rotationAngle = 0.0f;

    if (type == "cube") {
        Renderer::create_shape(shape);
    }else {
        std::cout << "This shape is not currently supported" << std::endl;
    }

    this->shapes.emplace_back(shape);
}

void Renderer::add_shape(const unsigned int id, const std::string &type, const plutom::vec3f &position, const plutom::vec3f &color,
                const plutom::vec3f &rotationAxis, const plutom::vec3f &scaling, const float rotation_speed,
                const float rotation_angle, const float shininess, const bool hasTexture,const std::string& textureFile) {
    if (id < this->currentID) throw std::range_error("Cannot add shape to existing id");
    Shape shape;
    if (this->lastShader < 0) throw std::range_error("No shaders have been added, create one before adding shapes");
    shape.shaderID = this->lastShader;
    shape.id = id;
    shape.type = type;
    shape.position = position;
    shape.scalingVector = scaling;
    shape.rotationAngle = rotation_angle;
    shape.rotationSpeed = rotation_speed;
    shape.rotationAxis = rotationAxis;
    shape.color = color;
    shape.shininess = shininess;
    shape.visible = true;
    shape.wireframe = false;
    shape.hasTexture = hasTexture;

    if (type == "cube") {
        Renderer::create_shape(shape, textureFile.c_str());
    }else {
        std::cout << "This shape is not currently supported" << std::endl;
    }

    this->shapes.emplace_back(shape);
}


void Renderer::create_shape(Shape &shape, const char* file) {
    if (file == "temp") file = "../../res/awesomeface.png";
    glGenVertexArrays(1,&shape.VAO);
    glGenBuffers(1, &shape.VBO);
    glBindVertexArray(shape.VAO);
    glBindBuffer(GL_ARRAY_BUFFER,shape.VBO);
    //TODO This needs to change when support moves to include more than cubes
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    shape.vertexCount = 36;
    if (shape.hasTexture) shape.textureID = load_texture(file, true);
}

unsigned int Renderer::load_texture(const char* filepath, bool flip) {
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(filepath,&width, &height, &nrChannels, 0);
    if(data){
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(format), width, height, 0,
                     format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return tex;
}

void Renderer::visualize(const Camera &cam, const float ratio, const float deltaTime) {
    for (auto & shape : this->shapes) {
        if (!shape.visible) continue;
        this->shaders[shape.shaderID].use();
        if (shape.wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (shape.hasTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, shape.textureID);
        }
        const auto value = plutom::perspective(plutom::radians(cam.Zoom), ratio, 0.1f, 100.0f);
        const auto view = cam.get_view_matrix();
        auto model = plutom::transform3D::translate(plutom::mat4f(1.0f), shape.position);
        model = plutom::transform3D::scale(model,shape.scalingVector);
        shape.rotationAngle += shape.rotationSpeed * deltaTime;
        model = plutom::transform3D::rotate(model ,plutom::radians(shape.rotationAngle), shape.rotationAxis);
        glBindVertexArray(shape.VAO);
        this->shaders[shape.shaderID].setMat4f("projection", value);
        this->shaders[shape.shaderID].setMat4f("view", view);
        this->shaders[shape.shaderID].setMat4f("model", model);
        if (!shape.hasTexture)
        this->shaders[shape.shaderID].setVec3f("color", shape.color);
        //TODO change this when different type of shapes are implemented (Only works for CUBES)
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(shape.vertexCount));
    }
}

void Renderer::add_shader(const Shader &shader) {
    this->shaders.emplace_back(shader);
    this->lastShader += 1;
}

