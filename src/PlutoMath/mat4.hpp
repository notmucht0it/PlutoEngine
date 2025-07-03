#pragma once

#include <ostream>
#include <stdexcept>
#include "vec4.hpp"
#include "mat2.hpp"
#include "mat3.hpp"

namespace plutom{

    template<typename T>
    struct mat4{
        vec4<T> columns[4];

        constexpr mat4() : columns{ vec4<T>{T(0), T(0), T(0), T(0)}, vec4<T>{T(0), T(0), T(0), T(0)},
                                    vec4<T>{T(0), T(0), T(0), T(0)}, vec4<T>{T(0), T(0), T(0), T(0)}} {}
        constexpr mat4(T s) : columns{  vec4<T>{s, T(0), T(0), T(0)}, vec4<T>{T(0), s, T(0), T(0)},
                                        vec4<T>{T(0), T(0), s, T(0)}, vec4<T>{T(0), T(0), T(0), s}} {}
        constexpr   mat4(const vec4<T>& col0, const vec4<T>& col1, const vec4<T>& col2, const vec4<T>& col3) :
                    columns{col0, col1, col2, col3} {}
        constexpr   mat4(T s1,T s2,T s3,T s4,T s5,T s6,T s7,T s8,T s9, T s10, T s11, T s12, T s13, T s14, T s15, T s16) : 
                    columns{vec4<T>{s1, s5, s9, s13}, vec4<T>{s2, s6, s10, s14},
                            vec4<T>{s3, s7, s11, s15},vec4<T>{s4, s8, s12, s16}} {}

        template<typename U>
        constexpr explicit operator mat4<U>() const {
            return mat4<U>{
                static_cast<vec4<U>>(columns[0]),
                static_cast<vec4<U>>(columns[1]),
                static_cast<vec4<U>>(columns[2]),
                static_cast<vec4<U>>(columns[3])
            };
        }
        
        constexpr vec4<T>& operator[](std::size_t i) {
            if(i >= 4) throw std::out_of_range("Index must be less than 4");
            return columns[i];
        }
        constexpr const vec4<T>& operator[](std::size_t i) const {
            if(i >= 4) throw std::out_of_range("Index must be less than 4");
            return columns[i];
        }

        friend std::ostream& operator<<(std::ostream& os, const mat4& m) {
            os << "[[" << m.columns[0].x << ", " << m.columns[1].x << ", " << m.columns[2].x << ", " << m.columns[3].x << "],\n"
               << " [" << m.columns[0].y << ", " << m.columns[1].y << ", " << m.columns[2].y << ", " << m.columns[3].y << "],\n"
               << " [" << m.columns[0].z << ", " << m.columns[1].z << ", " << m.columns[2].z << ", " << m.columns[3].z << "],\n"
               << " [" << m.columns[0].w << ", " << m.columns[1].w << ", " << m.columns[2].w << ", " << m.columns[3].w << "]]\n";
            return os;
        }

        constexpr mat4 operator+(const mat4& other) const{
            return {columns[0] + other.columns[0], columns[1] + other.columns[1],
                    columns[2] + other.columns[2], columns[3] + other.columns[3]};
        }

        constexpr mat4 operator-(const mat4& other) const{
            return {columns[0] - other.columns[0], columns[1] - other.columns[1],
                    columns[2] - other.columns[2], columns[3] - other.columns[3]};
        }

        constexpr mat4 operator*(T scalar) const{
            return {columns[0] * scalar, columns[1] * scalar, columns[2] * scalar, columns[3] * scalar};
        }

        constexpr mat4 operator/(T scalar) const{
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            return {columns[0] / scalar, columns[1] / scalar, columns[2] / scalar, columns[3] / scalar};
        }

        constexpr mat4 operator-() const{
            return {-columns[0], -columns[1], -columns[2], -columns[3]};
        }

        constexpr vec4<T> operator*(const vec4<T>& v) const{
            return {columns[0][0] * v.x + columns[1][0] * v.y + columns[2][0] * v.z + columns[3][0] * v.w,
                    columns[0][1] * v.x + columns[1][1] * v.y + columns[2][1] * v.z + columns[3][1] * v.w,
                    columns[0][2] * v.x + columns[1][2] * v.y + columns[2][2] * v.z + columns[3][2] * v.w,
                    columns[0][3] * v.x + columns[1][3] * v.y + columns[2][3] * v.z + columns[3][3] * v.w};
        }

        constexpr mat4 operator*(const mat4& other) const {
            mat2<T> a = {columns[0][0], columns[1][0],
                         columns[0][1], columns[1][1]};
            mat2<T> b = {columns[0][2], columns[1][2],
                         columns[0][3], columns[1][3]};
            mat2<T> c = {columns[2][0], columns[3][0],
                         columns[2][1], columns[3][1]};
            mat2<T> d = {columns[2][2], columns[3][2],
                         columns[2][3], columns[3][3]};

            mat2<T> e = {other.columns[0][0], other.columns[1][0],
                         other.columns[0][1], other.columns[1][1]};
            mat2<T> f = {other.columns[0][2], other.columns[1][2],
                         other.columns[0][3], other.columns[1][3]};
            mat2<T> g = {other.columns[2][0], other.columns[3][0],
                         other.columns[2][1], other.columns[3][1]};
            mat2<T> h = {other.columns[2][2], other.columns[3][2],
                         other.columns[2][3], other.columns[3][3]};

            mat2<T> M1 = (a + d) * (e + h);
            mat2<T> M2 = (c + d) * e;
            mat2<T> M3 = a * (f - h);
            mat2<T> M4 = d * (g - e);
            mat2<T> M5 = (a + b) * h;
            mat2<T> M6 = (c - a) * (e + f);
            mat2<T> M7 = (b - d) * (g + h);

            mat2<T> C00 = M1 + M4 - M5 + M7;
            mat2<T> C01 = M3 + M5;
            mat2<T> C10 = M2 + M4;
            mat2<T> C11 = M1 - M2 + M3 + M6;

            return mat4<T>{
                // Column 0 (col 0 of result)
                C00[0][0], C00[1][0],
                C10[0][0], C10[1][0],

                // Column 1 (col 1 of result)
                C00[0][1], C00[1][1],
                C10[0][1], C10[1][1],

                // Column 2 (col 2 of result)
                C01[0][0], C01[1][0],
                C11[0][0], C11[1][0],

                // Column 3 (col 3 of result)
                C01[0][1], C01[1][1],
                C11[0][1], C11[1][1]
            };
        }

        constexpr mat4 transpose() const{
            return {
                vec4<T>{columns[0].x, columns[1].x, columns[2].x, columns[3].x},
                vec4<T>{columns[0].y, columns[1].y, columns[2].y, columns[3].y},
                vec4<T>{columns[0].z, columns[1].z, columns[2].z, columns[3].z},
                vec4<T>{columns[0].w, columns[1].w, columns[2].w, columns[3].w}
            };
        }

        constexpr mat3<T> minor(std::size_t row, std::size_t col){
            if(row >= 4 || col >= 4) throw std::out_of_range("Index out of bounds for 4 by 4 matrix");

            mat3<T> result;

            std::size_t r = 0;
            for(std::size_t i = 0; i < 4; ++i){
                std::size_t c = 0;
                if(i == row) continue;
                for(std::size_t j = 0; j < 4; ++j){
                    if(j == col) continue;
                    result[c][r] = columns[j][i];
                    ++c;
                }
                ++r;
            }
            return result;
        }

        constexpr T determinant() const{
            return  columns[0][0] * minor(0,0).determinant() - columns[1][0] * minor(0,1).determinant() +
                    columns[2][0] * minor(0,2).determinant() - columns[3][0] * minor(0,3).determinant();
        }

        constexpr mat4 cofactor() const{
            mat4 output;
            for(std::size_t i = 0; i < 4; ++i){
                for(std::size_t j = 0; j < 4; ++j){
                    T mul = 1;
                    if((i+j)%2 == 1){
                        mul *= -1;
                    }
                    output[j][i] = mul * minor(i,j).determinant();
                }
            }

            return output;
        }

        constexpr mat4 adjugate() const{
            return cofactor().transpose();
        }

        constexpr mat4 inverse() const{
            T det = determinant();
            if(det == T(0)) throw std::domain_error("Determinant of matrix must be non zero to calculate inverse");
            return adjugate() / det;
        }

        constexpr bool operator==(const mat4& other) const{
            return  columns[0] == other.columns[0] && columns[1] == other.columns[1] &&
                    columns[2] == other.columns[2] && columns[3] == other.columns[3];
        }

        constexpr bool operator!=(const mat4& other) const{
            return  columns[0] != other.columns[0] || columns[1] != other.columns[1] ||
                    columns[2] != other.columns[2] || columns[3] != other.columns[3];
        }

        constexpr mat4& operator+=(const mat4& other){
            columns[0] += other.columns[0];
            columns[1] += other.columns[1];
            columns[2] += other.columns[2];
            columns[3] += other.columns[3];
            return *this;
        }

        constexpr mat4& operator-=(const mat4& other){
            columns[0] -= other.columns[0];
            columns[1] -= other.columns[1];
            columns[2] -= other.columns[2];
            columns[3] -= other.columns[3];
            return *this;
        }

        constexpr mat4& operator*=(T scalar){
            columns[0] *= scalar;
            columns[1] *= scalar;
            columns[2] *= scalar;
            columns[3] *= scalar;
            return *this;
        }

        constexpr mat4& operator/=(T scalar){
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            columns[0] /= scalar;
            columns[1] /= scalar;
            columns[2] /= scalar;
            columns[3] /= scalar;
            return *this;
        }

        static constexpr mat4 identity(){
            return mat4(T(1));
        }

        constexpr vec4<T> col(std::size_t i) const {
            return (*this)[i]; // same as operator[]
        }

        constexpr vec4<T> row(std::size_t i) const {
            if(i >= 4) throw std::out_of_range("Row index must be less than 4");
            return {columns[0][i], columns[1][i], columns[2][i]};
        }

        constexpr mat4<T> gramschimdt(T eps = std::numeric_limits<T>::epsilon()){
            T tol = 1000*4*std::max(columns[0].norm(),
                         std::max(columns[1].norm(),
                         std::max(columns[2].norm(), 
                                  columns[3].norm())));
            mat4<T> orth;
            for(int i = 0; i < 4; ++i){
                vec4<T> u = columns[i] - orth*orth.transpose()*columns[i];
                vec4<T> uNorm = u.normalize();
                if(uNorm > tol) orth[i] = u / uNorm;
            }
            return orth;
        }

    };

    template<typename T>
    constexpr const T* value_ptr(const mat4<T>& m) {
        return &m.columns[0].x;
    }

    template<typename T>
    constexpr T* value_ptr(mat4<T>& m) {
        return &m.columns[0].x;
    }

    template<typename T>
    constexpr mat4<T> operator*(T scalar, const mat4<T>& m) {
        return m * scalar;
    }

    using mat4f = mat4<float>;
    using mat4d = mat4<double>;
    using mat4i = mat4<int>;
}