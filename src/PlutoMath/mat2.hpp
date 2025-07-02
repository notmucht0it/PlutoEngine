#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>
#include <stdexcept>

#include "scalar_utils.hpp"
#include "vec2.hpp"

namespace plutom{

    template<typename T>
    struct mat2{
        plutom::vec2<T> columns[2];

        constexpr mat2() : columns{plutom::vec2<T>{T(0), T(0)}, plutom::vec2<T>{T(0), T(0)}} {}
        constexpr mat2(T s) : columns{plutom::vec2<T>{s, T(0)}, plutom::vec2<T>{T(0), s}} {}
        constexpr mat2(const plutom::vec2<T>& col0, const plutom::vec2<T>& col1) : columns{col0, col1} {}
        constexpr mat2(T s1,T s2,T s3,T s4) : columns{plutom::vec2<T>{s1, s3}, plutom::vec2<T>{s2, s4}} {}

        constexpr plutom::vec2& operator[](std::size_t i) {
            if(i >= 2) throw std::out_of_range("Index must be 0 or 1");
            return columns[i];
        }
        constexpr const plutom::vec2& operator[](std::size_t i) const {
            if(i >= 2) throw std::out_of_range("Index must be 0 or 1");
            return columns[i];
        }

        friend std::ostream& operator<<(std::ostream& os, const mat2& m) {
            os << "[[" << m.columns[0].x << ", " << m.columns[1].x << "],\n"
               << " [" << m.columns[0].y << ", " << m.columns[1].y << "]]";
            return os;
        }

        constexpr mat2 operator+(const mat2& other) const{
            return {columns[0] + other.columns[0], columns[1] + other.columns[1]};
        }

        constexpr mat2 operator-(const mat2& other) const{
            return {columns[0] - other.columns[0], columns[1] - other.columns[1]};
        }

        constexpr mat2 operator*(T scalar) const{
            return {columns[0] * scalar, columns[1] * scalar};
        }

        constexpr plutom::vec2<T> operator*(const plutom::vec2<T>& v) const{
            return {columns[0][0] * v.x + columns[1][0] * v.y, columns[0][1] * v.x + columns[1][1] * v.y};
        }

        constexpr mat2 operator*(const mat2& other) const {
            T a = columns[0][0], b = columns[1][0];
            T c = columns[0][1], d = columns[1][1];

            T e = other.columns[0][0], f = other.columns[1][0];
            T g = other.columns[0][1], h = other.columns[1][1];

            T M1 = (a + d) * (e + h);
            T M2 = (c + d) * e;
            T M3 = a * (f - h);
            T M4 = d * (g - e);
            T M5 = (a + b) * h;
            T M6 = (c - a) * (e + f);
            T M7 = (b - d) * (g + h);

            T C00 = M1 + M4 - M5 + M7;
            T C01 = M3 + M5;
            T C10 = M2 + M4;
            T C11 = M1 - M2 + M3 + M6;

            return mat2{
                plutom::vec2<T>{C00, C10},
                plutom::vec2<T>{C01, C11}
            };
        }

        constexpr mat2 transpose() const{
            return {
                plutom::vec2<T>{columns[0].x, columns[1].x},
                plutom::vec2<T>{columns[0].y, columns[1].y}
            }
        }

        

    };

    using mat2f = mat2<float>;
    using mat2d = mat2<double>;
    using mat2i = mat2<int>;
}