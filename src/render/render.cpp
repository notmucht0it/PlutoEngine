#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "render.hpp"
#include "../PlutoMath/plutomath.hpp"

Renderer::Renderer(GLFWwindow *window): window(window) {
    constexpr float axis_lines[] = {
        // X axis (red)
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

        // Y axis (green)
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

        // Z axis (blue)
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &axisVAO);
    glGenBuffers(1, &axisVBO);
    glBindVertexArray(axisVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis_lines), axis_lines, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    axisShader = std::make_shared<Shader>("shaders/axis.vs", "shaders/axis.fs");
}

void Renderer::add_shape(const ShapeDescriptor& desc) {
    Shape shape;
    if (this->lastShader < 0) throw std::range_error("No shaders have been added, create one before adding shapes");
    shape.shaderID = this->lastShader;
    shape.sType = desc.sType;
    shape.id = this->currentID;
    this->currentID += 1;
    shape.type = desc.type;
    shape.position = desc.position;
    shape.scalingVector = desc.scalingVector;
    shape.rotationAngle = desc.rotationAngle;
    shape.rotationSpeed = desc.rotationSpeed;
    shape.rotationAxis = desc.rotationAxis;
    shape.color = desc.color;
    shape.shininess = desc.shininess;
    shape.visible = desc.visible;
    shape.wireframe = desc.wireframe;
    shape.hasTexture = desc.hasTexture;

    if (desc.type == "cube" || desc.type == "square" || desc.type == "circle"){ // || desc.type == "sphere") {
        create_shape(shape, desc.texturePath.c_str());
    }else {
        std::cout << "This shape is not currently supported" << std::endl;
    }

    this->shapes.emplace_back(shape);
}


void Renderer::create_shape(Shape &shape, const char* file) {
    if (file == "temp") file = "../../res/awesomeface.png";
    glGenVertexArrays(1,&shape.VAO);
    glGenBuffers(1, &shape.VBO);
    glGenBuffers(1, &shape.EBO);
    glBindVertexArray(shape.VAO);
    glBindBuffer(GL_ARRAY_BUFFER,shape.VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.EBO);
    //TODO This needs to change when support moves to include more than cubes
    primative currentShape;
    if (shape.type == "cube") currentShape = primative_generator::get_cube();
    if (shape.type == "square") currentShape = primative_generator::get_square();
    if (shape.type == "circle") currentShape = primative_generator::get_circle();
    glBufferData(GL_ARRAY_BUFFER,static_cast<long>(currentShape.vertices.size() * sizeof(float)), currentShape.vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(currentShape.indices.size() * sizeof(unsigned int)), currentShape.indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    shape.indicesCount = currentShape.indices.size();
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
    plutom::vec3f lightPos, lightColor;

    for (auto& shape : this->shapes) {
        if (shape.sType == ShaderType::Source) { // your light cube type
            const auto time = static_cast<float>(glfwGetTime());
            shape.position = plutom::vec3f(sin(time)*2.0f, sin(time)*1.0f, cos(time)*2.0f);
            shape.color = plutom::vec3f(cos(time)*0.5f + 0.5f, sin(time)*0.5f + 0.5f, sin(time)*0.5f + 0.5f); // white light
            lightPos = shape.position;
            lightColor = shape.color;
            //cam.Position = lightPos + plutom::vec3f{0.0f, 0.3f, 0.0f};
            break;
        }
    }
    for (auto & shape : this->shapes) {
        if (!shape.visible) continue;
        auto& shader = *shaders[shape.shaderID];
        shader.use();
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
        shape.rotationAngle += shape.rotationSpeed * deltaTime;
        model = plutom::transform3D::rotate(model ,plutom::radians(shape.rotationAngle), shape.rotationAxis);
        model = plutom::transform3D::scale(model,shape.scalingVector);
        glBindVertexArray(shape.VAO);
        shader.setMat4f("projection", value);
        shader.setMat4f("view", view);
        shader.setMat4f("model", model);
        switch (shape.sType) {
            case ShaderType::Lighting:
                shader.setVec3f("viewPos", cam.Position);
                shader.setVec3f("lightColor", lightColor);
                shader.setVec3f("lightPos", lightPos);
                shader.setVec3f("objectColor", shape.color);
                shader.setFloat("shine",shape.shininess);
                break;
            case ShaderType::Basic:
                shader.setVec3f("color", shape.color);
                break;
            case ShaderType::Source:
                shader.setVec3f("color", shape.color);
                break;
        }
        //glDrawArrays(GL_TRIANGLES,0,shape.indicesCount);
        glDrawElements(GL_TRIANGLES,static_cast<int>(shape.indicesCount),GL_UNSIGNED_INT,nullptr);
        //std::cout << "Shape ID: " << shape.id << ", Shader ID: " << shape.shaderID << std::endl;
    }

    if (cam.show_debug_axis) {
        axisShader->use();
        axisShader->setMat4f("model", plutom::mat4f::identity());
        axisShader->setMat4f("view", cam.get_view_matrix());
        axisShader->setMat4f("projection", plutom::perspective(
            plutom::radians(cam.Zoom), ratio, 0.1f, 100.0f));

        glBindVertexArray(axisVAO);
        glDrawArrays(GL_LINES, 0, 6);
    }
}

void Renderer::add_shader(const std::shared_ptr<Shader>& shader) {
    this->shaders.push_back(shader);
    this->lastShader += 1;
}

