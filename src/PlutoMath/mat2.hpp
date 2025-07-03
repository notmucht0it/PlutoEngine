#pragma once

#include <cstddef>
#include <ostream>
#include <stdexcept>
#include "vec2.hpp"

namespace plutom{

    template<typename T>
    struct mat2{
        vec2<T> columns[2];

        constexpr mat2() : columns{vec2<T>{T(0), T(0)}, vec2<T>{T(0), T(0)}} {}
        constexpr explicit mat2(T s) : columns{vec2<T>{s, T(0)}, vec2<T>{T(0), s}} {}
        constexpr mat2(const vec2<T>& col0, const vec2<T>& col1) : columns{col0, col1} {}
        constexpr mat2(T s1,T s2,T s3,T s4) : columns{vec2<T>{s1, s3}, vec2<T>{s2, s4}} {}

        template<typename U>
        constexpr explicit operator mat2<U>() const {
            return mat2<U>{
                static_cast<vec2<U>>(columns[0]),
                static_cast<vec2<U>>(columns[1])
            };
        }
        
        constexpr vec2<T>& operator[](std::size_t i) {
            if(i >= 2) throw std::out_of_range("Index must be 0 or 1");
            return columns[i];
        }
        constexpr const vec2<T>& operator[](std::size_t i) const {
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

        constexpr vec2<T> operator*(const vec2<T>& v) const{
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
                vec2<T>{C00, C10},
                vec2<T>{C01, C11}
            };
        }

        constexpr mat2 transpose() const{
            return {
                vec2<T>{columns[0].x, columns[1].x},
                vec2<T>{columns[0].y, columns[1].y}
            };
        }

        constexpr T determinant() const{
            return columns[0].x * columns[1].y - columns[1].x * columns[0].y;
        }

        constexpr T minor(const std::size_t row, const std::size_t col){
            if(row >= 2 || col >= 2) throw std::out_of_range("Index out of bounds for 2 by 2 matrix");

            std::size_t r = 0;
            for(std::size_t i = 0; i < 2; ++i){
                if(i == row) continue;
                for(std::size_t j = 0; j < 2; ++j){
                    if(j == col) continue;
                    return columns[j][i];
                }
                ++r;
            }
            return T(0);
        }

        constexpr mat2 cofactor() const{
            mat2 output;
            for(std::size_t i = 0; i < 2; ++i){
                for(std::size_t j = 0; j < 2; ++j){
                    T mul = 1;
                    if((i+j)%2 == 1){
                        mul *= -1;
                    }
                    output[j][i] = mul * minor(i,j);
                }
            }

            return output;
        }

        constexpr mat2 adjugate() const{
            return cofactor().transpose();
        }

        constexpr mat2 inverse() const{
            T det = determinant();
            if(det == T(0)) throw std::domain_error("Determinant of matrix must be non zero to calculate inverse");
            return adjugate() / det;
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

        constexpr vec2<T> col(std::size_t i) const {
            return (*this)[i]; // same as operator[]
        }

        constexpr vec2<T> row(std::size_t i) const {
            if(i >= 2) throw std::out_of_range("Row index must be 0 or 1");
            return {columns[0][i], columns[1][i]};
        }

        constexpr mat2<T> gramschimdt(T eps = std::numeric_limits<T>::epsilon()){
            T tol = 1000*2*std::max(columns[0].norm(),
                                    columns[1].norm());
            mat2<T> orth;
            for(int i = 0; i < 2; ++i){
                vec2<T> u = columns[i] - orth*orth.transpose()*columns[i];
                vec2<T> uNorm = u.normalize();
                if(uNorm > tol) orth[i] = u / uNorm;
            }
            return orth;
        }

    };

    template<typename T>
    constexpr const T* value_ptr(const mat2<T>& m) {
        return &m.columns[0].x;
    }

    template<typename T>
    constexpr T* value_ptr(mat2<T>& m) {
        return &m.columns[0].x;
    }

    template<typename T>
    constexpr mat2<T> operator*(T scalar, const mat2<T>& m) {
        return m * scalar;
    }

    using mat2f = mat2<float>;
    using mat2d = mat2<double>;
    using mat2i = mat2<int>;
}