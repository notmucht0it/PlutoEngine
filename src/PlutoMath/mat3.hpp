#pragma once

#include <ostream>
#include <stdexcept>
#include "vec3.hpp"
#include "mat2.hpp"

namespace plutom{

    template<typename T>
    struct mat3{
        vec3<T> columns[3];

        constexpr mat3() : columns{ vec3<T>{T(0), T(0), T(0)}, vec3<T>{T(0), T(0), T(0)},
                                    vec3<T>{T(0), T(0), T(0)}} {}
        constexpr explicit mat3(T s) : columns{  vec3<T>{s, T(0), T(0)}, vec3<T>{T(0), s, T(0)},
                                        vec3<T>{T(0), T(0), s}} {}
        constexpr mat3(const vec3<T>& col0, const vec3<T>& col1, const vec3<T>& col2) : columns{col0, col1, col2} {}
        constexpr   mat3(T s1,T s2,T s3,T s4,T s5,T s6,T s7,T s8,T s9) : 
                    columns{vec3<T>{s1, s4, s7}, vec3<T>{s2, s5, s8}, vec3<T>{s3, s6, s9}} {}

        template<typename U>
        constexpr explicit operator mat3<U>() const {
            return mat3<U>{
                static_cast<vec3<U>>(columns[0]),
                static_cast<vec3<U>>(columns[1]),
                static_cast<vec3<U>>(columns[2])
            };
        }
        
        constexpr vec3<T>& operator[](std::size_t i) {
            if(i >= 3) throw std::out_of_range("Index must be 0, 1, or 2");
            return columns[i];
        }
        constexpr const vec3<T>& operator[](std::size_t i) const {
            if(i >= 3) throw std::out_of_range("Index must be 0, 1, or 2");
            return columns[i];
        }

        friend std::ostream& operator<<(std::ostream& os, const mat3& m) {
            os << "[[" << m.columns[0].x << ", " << m.columns[1].x << ", " << m.columns[2].x << "],\n"
               << " [" << m.columns[0].y << ", " << m.columns[1].y << ", " << m.columns[2].y << "],\n"
               << " [" << m.columns[0].z << ", " << m.columns[1].z << ", " << m.columns[2].z <<"]]\n";
            return os;
        }

        constexpr mat3 operator+(const mat3& other) const{
            return {columns[0] + other.columns[0], columns[1] + other.columns[1],
                    columns[2] + other.columns[2]};
        }

        constexpr mat3 operator-(const mat3& other) const{
            return {columns[0] - other.columns[0], columns[1] - other.columns[1],
                    columns[2] - other.columns[2]};
        }

        constexpr mat3 operator*(T scalar) const{
            return {columns[0] * scalar, columns[1] * scalar, columns[2] * scalar};
        }

        constexpr mat3 operator/(T scalar) const{
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            return {columns[0] / scalar, columns[1] / scalar, columns[2] / scalar};
        }

        constexpr mat3 operator-() const{
            return {-columns[0], -columns[1], -columns[2]};
        }

        constexpr vec3<T> operator*(const vec3<T>& v) const{
            return {columns[0][0] * v.x + columns[1][0] * v.y + columns[2][0] * v.z,
                    columns[0][1] * v.x + columns[1][1] * v.y + columns[2][1] * v.z,
                    columns[0][2] * v.x + columns[1][2] * v.y + columns[2][2] * v.z};
        }

        constexpr mat3 operator*(const mat3& other) const {
            mat2<T> a = {columns[0][0],columns[1][0],
                         columns[0][1], columns[1][1]};
            mat2<T> b = {columns[0][2], columns[1][2],
                            0,          0};
            mat2<T> c = {columns[2][0], 0,
                         columns[2][1], 0};
            mat2<T> d = {columns[2][2],0,
                            0,         0};

            mat2<T> e = {other.columns[0][0],other.columns[1][0],
                         other.columns[0][1],other.columns[1][1]};
            mat2<T> f = {other.columns[0][2], other.columns[1][2],
                            0,          0};
            mat2<T> g = {other.columns[2][0], 0,
                         other.columns[2][1], 0};
            mat2<T> h = {other.columns[2][2],0,
                            0,         0};

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

            return mat3<T>{
                // Column 0
                C00[0][0], C00[1][0], C10[0][0],
                // Column 1
                C00[0][1], C00[1][1], C10[0][1],
                // Column 2
                C01[0][0], C01[1][0], C11[0][0]
            };
        }

        constexpr mat3 transpose() const{
            return {
                vec3<T>{columns[0].x, columns[1].x, columns[2].x},
                vec3<T>{columns[0].y, columns[1].y, columns[2].y},
                vec3<T>{columns[0].z, columns[1].z, columns[2].z}
            };
        }

        constexpr mat2<T> minor(const std::size_t row, const std::size_t col){
            if(row >= 3 || col >= 3) throw std::out_of_range("Index out of bounds for 3 by 3 matrix");

            mat2<T> result;

            std::size_t r = 0;
            for(std::size_t i = 0; i < 3; ++i){
                std::size_t c = 0;
                if(i == row) continue;
                for(std::size_t j = 0; j < 3; ++j){
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
                    columns[2][0] * minor(0,2).determinant();
        }

        constexpr mat3 cofactor() const{
            mat3 output;
            for(std::size_t i = 0; i < 3; ++i){
                for(std::size_t j = 0; j < 3; ++j){
                    T mul = 1;
                    if((i+j)%2 == 1){
                        mul *= -1;
                    }
                    output[j][i] = mul * minor(i,j).determinant();
                }
            }

            return output;
        }

        constexpr mat3 adjugate() const{
            return cofactor().transpose();
        }

        constexpr mat3 inverse() const{
            T det = determinant();
            if(det == T(0)) throw std::domain_error("Determinant of matrix must be non zero to calculate inverse");
            return adjugate() / det;
        }

        constexpr bool operator==(const mat3& other) const{
            return  columns[0] == other.columns[0] && columns[1] == other.columns[1] &&
                    columns[2] == other.columns[2];
        }

        constexpr bool operator!=(const mat3& other) const{
            return  columns[0] != other.columns[0] || columns[1] != other.columns[1] ||
                    columns[2] != other.columns[2];
        }

        constexpr mat3& operator+=(const mat3& other){
            columns[0] += other.columns[0];
            columns[1] += other.columns[1];
            columns[2] += other.columns[2];
            return *this;
        }

        constexpr mat3& operator-=(const mat3& other){
            columns[0] -= other.columns[0];
            columns[1] -= other.columns[1];
            columns[2] -= other.columns[2];
            return *this;
        }

        constexpr mat3& operator*=(T scalar){
            columns[0] *= scalar;
            columns[1] *= scalar;
            columns[2] *= scalar;
            return *this;
        }

        constexpr mat3& operator/=(T scalar){
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            columns[0] /= scalar;
            columns[1] /= scalar;
            columns[2] /= scalar;
            return *this;
        }

        static constexpr mat3 identity(){
            return mat3(T(1));
        }

        constexpr vec3<T> col(std::size_t i) const {
            return (*this)[i]; // same as operator[]
        }

        constexpr vec3<T> row(std::size_t i) const {
            if(i >= 3) throw std::out_of_range("Row index must be 0, 1, or 2");
            return {columns[0][i], columns[1][i], columns[2][i]};
        }

        constexpr mat3<T> gramschimdt(T eps = std::numeric_limits<T>::epsilon()){
            T tol = 1000*3*std::max(columns[0].norm(),
                            std::max(columns[1].norm(),
                                     columns[2].norm()));
            mat3<T> orth;
            for(int i = 0; i < 3; ++i){
                vec3<T> u = columns[i] - orth*orth.transpose()*columns[i];
                vec3<T> uNorm = u.normalize();
                if(uNorm > tol) orth[i] = u / uNorm;
            }
            return orth;
        }

    };

    template<typename T>
    constexpr const T* value_ptr(const mat3<T>& m) {
        return &m.columns[0].x;
    }

    template<typename T>
    constexpr T* value_ptr(mat3<T>& m) {
        return &m.columns[0].x;
    }

    template<typename T>
    constexpr mat3<T> operator*(T scalar, const mat3<T>& m) {
        return m * scalar;
    }

    using mat3f = mat3<float>;
    using mat3d = mat3<double>;
    using mat3i = mat3<int>;
}