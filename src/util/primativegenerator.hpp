#pragma once
#include <vector>

#ifndef PRIMATIVEGENERATOR_HPP
#define PRIMATIVEGENERATOR_HPP

struct primative {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

class primative_generator {
public:
    static primative get_square();
    static primative get_cube();
    static primative get_circle();
    static primative get_sphere();
private:
    static void generate_square();
    static void generate_cube();
    static void generate_circle();
    static void generate_sphere();

    static primative square;
    static primative cube;
    static primative circle;
    static primative sphere;

    static constexpr unsigned int circleSteps = 30;
};



#endif //PRIMATIVEGENERATOR_HPP
