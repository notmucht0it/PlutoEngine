#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>

namespace plutom{

    template<typename T>
    struct vec2{
        T x, y;

        constexpr vec2() : x(0), y(0) {}
        constexpr vec2(T x, T y) : x(x), y(y) {}

        constexpr vec2 operator+(const vec2& other) const{
            return {x + other.x, y + other.y};
        }

        constexpr vec2 operator-(const vec2& other) const{
            return {x - other.x, y - other.y};
        }

        constexpr vec2 operator*(T scalar) const{
            return {x*scalar, y*scalar};
        }

        constexpr vec2 operator/(T scalar) const{
            return {x/scalar, y/scalar};
        }

        constexpr vec2 operator-() const{
            return {-x, -y};
        }

        

    };

    using vec2f = vec2<float>;
    using vec2d = vec2<double>;
    using vec2i = vec2<int>;
}