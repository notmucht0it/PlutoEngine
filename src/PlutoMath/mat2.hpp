#pragma once

#include <ostream>
#include <stdexcept>
#include "vec2.hpp"

namespace plutom{

    template<typename T>
    struct mat2{
        plutom::vec2<T> columns[2];

        constexpr mat2() : columns{plutom::vec2<T>{T(0), T(0)}, plutom::vec2<T>{T(0), T(0)}} {}
        constexpr mat2(T s) : columns{plutom::vec2<T>{s, T(0)}, plutom::vec2<T>{T(0), s}} {}
        constexpr mat2(const plutom::vec2<T>& col0, const plutom::vec2<T>& col1) : columns{col0, col1} {}
        constexpr mat2(T s1,T s2,T s3,T s4) : columns{plutom::vec2<T>{s1, s3}, plutom::vec2<T>{s2, s4}} {}

        template<typename U>
        constexpr explicit operator mat2<U>() const {
            return mat2<U>{
                static_cast<plutom::vec2<U>>(columns[0]),
                static_cast<plutom::vec2<U>>(columns[1])
            };
        }
        
        constexpr plutom::vec2<T>& operator[](std::size_t i) {
            if(i >= 2) throw std::out_of_range("Index must be 0 or 1");
            return columns[i];
        }
        constexpr const plutom::vec2<T>& operator[](std::size_t i) const {
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

        constexpr mat2 operator/(T scalar) const{
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            return {columns[0] / scalar, columns[1] / scalar};
        }

        constexpr mat2 operator-() const{
            return {-columns[0], -columns[1]};
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
            };
        }

        constexpr T determinant() const{
            return columns[0].x * columns[1].y - columns[1].x * columns[0].y;
        }

        constexpr mat2 inverse() const{
            T det = determinant();
            if(det == T(0)) throw std::domain_error("Determinant of matrix must be non zero to calculate inverse");
            return mat2{columns[1].y, -columns[1].x, -columns[0].y, columns[0].x} / det;
        }

        constexpr bool operator==(const mat2& other) const{
            return columns[0] == other.columns[0] && columns[1] == other.columns[1];
        }

        constexpr bool operator!=(const mat2& other) const{
            return columns[0] != other.columns[0] || columns[1] != other.columns[1];
        }

        constexpr mat2& operator+=(const mat2& other){
            columns[0] += other.columns[0];
            columns[1] += other.columns[1];
            return *this;
        }

        constexpr mat2& operator-=(const mat2& other){
            columns[0] -= other.columns[0];
            columns[1] -= other.columns[1];
            return *this;
        }

        constexpr mat2& operator*=(T scalar){
            columns[0] *= scalar;
            columns[1] *= scalar;
            return *this;
        }

        constexpr mat2& operator/=(T scalar){
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            columns[0] /= scalar;
            columns[1] /= scalar;
            return *this;
        }

        static constexpr mat2 identity(){
            return mat2(T(1));
        }

        constexpr plutom::vec2<T> col(std::size_t i) const {
            return (*this)[i]; // same as operator[]
        }

        constexpr plutom::vec2<T> row(std::size_t i) const {
            if(i >= 2) throw std::out_of_range("Row index must be 0 or 1");
            return {columns[0][i], columns[1][i]};
        }

    };

    template<typename T>
    constexpr plutom::vec2<T> operator*(const plutom::vec2<T>& v, const plutom::mat2<T>& m) {
        return {
            v.x * m[0][0] + v.y * m[0][1],
            v.x * m[1][0] + v.y * m[1][1]
        };
    }

    template<typename T>
    constexpr mat2<T> operator*(T scalar, const mat2<T>& m) {
        return {
            scalar * m[0][0], scalar * m[1][0],
            scalar * m[0][1], scalar * m[1][1]
        };
    }

    using mat2f = mat2<float>;
    using mat2d = mat2<double>;
    using mat2i = mat2<int>;
}