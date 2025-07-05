//
// Created by kevin on 7/5/25.
//

#include "primativegenerator.hpp"

#include <cmath>

primative primative_generator::square;
primative primative_generator::cube;
primative primative_generator::circle;
primative primative_generator::sphere;

primative primative_generator::get_square() {
    if (square.vertices.empty()) generate_square();
    return square;
}

void primative_generator::generate_square() {
    square.vertices = {
        // x, y, z, nx, ny, nz, u, v
        -0.5f, -0.5f, 0.0f,  0,0,1,  0,0,
         0.5f, -0.5f, 0.0f,  0,0,1,  1,0,
         0.5f,  0.5f, 0.0f,  0,0,1,  1,1,
        -0.5f,  0.5f, 0.0f,  0,0,1,  0,1
    };
    square.indices = {
        0, 1, 2,
        2, 3, 0
    };
}


primative primative_generator::get_cube() {
    if (cube.vertices.empty()) generate_cube();
    return cube;
}

void primative_generator::generate_cube() {
    cube.vertices = {
        // x, y, z, nx, ny, nz, u, v
        //Front
        -0.5f, -0.5f, 0.5f,  0,0,1,  0,0,
         0.5f, -0.5f, 0.5f,  0,0,1,  1,0,
         0.5f,  0.5f, 0.5f,  0,0,1,  1,1,
        -0.5f,  0.5f, 0.5f,  0,0,1,  0,1,
        //Back
        -0.5f, -0.5f, -0.5f,  0,0,-1,  0,0,
         0.5f, -0.5f, -0.5f,  0,0,-1,  1,0,
         0.5f,  0.5f, -0.5f,  0,0,-1,  1,1,
        -0.5f,  0.5f, -0.5f,  0,0,-1,  0,1,
        //Left
        -0.5f, -0.5f, -0.5f,  -1,0,0,  0,0,
        -0.5f,  0.5f, -0.5f,  -1,0,0,  1,0,
        -0.5f,  0.5f,  0.5f,  -1,0,0,  1,1,
        -0.5f, -0.5f,  0.5f,  -1,0,0,  0,1,
        //Right
        0.5f, -0.5f, -0.5f,  1,0,0,  0,0,
        0.5f,  0.5f, -0.5f,  1,0,0,  1,0,
        0.5f,  0.5f,  0.5f,  1,0,0,  1,1,
        0.5f, -0.5f,  0.5f,  1,0,0,  0,1,
        //Top
        -0.5f, 0.5f, -0.5f,  0,1,0,  0,0,
         0.5f, 0.5f, -0.5f,  0,1,0,  1,0,
         0.5f, 0.5f,  0.5f,  0,1,0,  1,1,
        -0.5f, 0.5f,  0.5f,  0,1,0,  0,1,
        //Bottom
        -0.5f, -0.5f, -0.5f,  0,-1,0,  0,0,
         0.5f, -0.5f, -0.5f,  0,-1,0,  1,0,
         0.5f, -0.5f,  0.5f,  0,-1,0,  1,1,
        -0.5f, -0.5f,  0.5f,  0,-1,0,  0,1,
    };
    cube.indices = {
        //Front
        0, 1, 2,
        2, 3, 0,
        //Back
        4, 5, 6,
        6, 7, 4,
        //Left
        8,9,10,
        10,11,8,
        //Right
        12,13,14,
        14,15,12,
        //Top
        16,17,18,
        18,19,16,
        //Bottom
        20,21,22,
        22,23,20
    };
}

primative primative_generator::get_circle() {
    if (circle.vertices.empty()) generate_circle();
    return circle;
}

void primative_generator::generate_circle() {
    circle.vertices.emplace_back(0.0f);
    circle.vertices.emplace_back(0.0f);
    circle.vertices.emplace_back(0.0f);
    circle.vertices.emplace_back(0.0f);
    circle.vertices.emplace_back(0.0f);
    circle.vertices.emplace_back(1.0f);
    circle.vertices.emplace_back(0.0f);
    circle.vertices.emplace_back(0.0f);
    for (unsigned int i = 0; i < circleSteps; ++i) {
        const float theta = (i / static_cast<float>(circleSteps)) * 2.0f * M_PI;
        circle.vertices.emplace_back(std::cos(theta));
        circle.vertices.emplace_back(std::sin(theta));
        circle.vertices.emplace_back(0.0f);
        circle.vertices.emplace_back(0.0f);
        circle.vertices.emplace_back(0.0f);
        circle.vertices.emplace_back(1.0f);
        circle.vertices.emplace_back(0.0f);
        circle.vertices.emplace_back(0.0f);
    }

    for (unsigned int i = 0; i < circleSteps; ++i) {
        circle.indices.emplace_back(0); // center
        circle.indices.emplace_back(i + 1);
        circle.indices.emplace_back((i + 1) % circleSteps + 1); // wrap
    }

}

primative primative_generator::get_sphere() {
    if (sphere.vertices.empty()) generate_sphere();
    return sphere;
}

void primative_generator::generate_sphere() {

}
