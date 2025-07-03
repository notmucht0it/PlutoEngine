#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>
#include <stdexcept>

#include "scalar_utils.hpp"

namespace plutom{

    template<typename T>
    struct vec4{
        T x, y, z, w;

        constexpr vec4() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
        explicit constexpr vec4(T val) : x(val), y(val), z(val), w(val) {}
        explicit  constexpr vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        template<typename U>
        explicit constexpr vec4(const vec4<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)),
                                                        z(static_cast<T>(other.z)), w(static_cast<T>(other.w)) {}

        T& operator[](std::size_t i) {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            if (i == 3) return w;
            throw std::out_of_range("vec4 index must be 0, 1, 2, or 3");
        }
        const T& operator[](std::size_t i) const {
            if (i == 0) return x;
            if (i == 1) return y;
            if (i == 2) return z;
            if (i == 3) return w;
            throw std::out_of_range("vec4 index must be 0, 1, 2, or 3");
        }

        constexpr vec4 operator+(const vec4& other) const{
            return {x + other.x, y + other.y, z + other.z, w + other.w};
        }

        constexpr vec4 operator-(const vec4& other) const{
            return {x - other.x, y - other.y, z - other.z, w - other.w};
        }

        constexpr vec4 cwise_mul(const vec4& other) const{
            return {x*other.x, y*other.y, z*other.z, w*other.w};
        }

        constexpr vec4 operator*(T scalar) const{
            return {x*scalar, y*scalar, z*scalar, w*scalar};
        }

        constexpr vec4 operator/(T scalar) const{
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            return {x/scalar, y/scalar, z/scalar, w/scalar};
        }

        constexpr vec4 operator-() const{
            return {-x, -y, -z, -w};
        }

        constexpr vec4& operator+=(const vec4& other){
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        constexpr vec4& operator-=(const vec4& other){
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        constexpr vec4& operator+=(T scalar){
            x += scalar;
            y += scalar;
            z += scalar;
            w += scalar;
            return *this;
        }

        constexpr vec4& operator-=(T scalar){
            x -= scalar;
            y -= scalar;
            z -= scalar;
            w -= scalar;
            return *this;
        }

        constexpr vec4& operator*=(T scalar){
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        constexpr vec4& operator/=(T scalar){
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        constexpr T dot(const vec4& other) const{
            return x*other.x + y*other.y + z*other.z + w*other.w;
        }

        T length() const {
            static_assert(std::is_floating_point<T>::value, "length() only available for float/double types");
            return std::sqrt(dot(*this));
        }

        constexpr T length_squared() const {
            return dot(*this);
        }

        T magnitude() const {
            return length();
        }

        vec4 normalize() const {
            static_assert(std::is_floating_point<T>::value, "normalize() only available for float/double types");
            T len = length();
            return len == T(0) ? *this : *this / len;
        }

        T distance(const vec4& other) const{
            static_assert(std::is_floating_point<T>::value, "distance() only available for float/double types");
            return std::sqrt((x - other.x)*(x - other.x) + (y - other.y)*(y - other.y) + (z - other.z)*(z - other.z) + (w - other.w)*(w - other.w));
        }

        T norm() const{
            static_assert(std::is_floating_point<T>::value, "distance() only available for float/double types");
            return std::sqrt(x*x + y*y + z*z + w*w);
        }

        constexpr T distance_squared(const vec4& other) const{
            return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y) + (z - other.z)*(z - other.z) + (w - other.w)*(w - other.w);
        }

        constexpr bool operator==(const vec4& other) const{
            return  almostequal(x,other.x) && almostequal(y,other.y) && 
                    almostequal(z,other.z) && almostequal(w,other.w);
        }

        constexpr bool operator!=(const vec4& other) const{
            return  !almostequal(x,other.x) || !almostequal(y,other.y) || 
                    !almostequal(z,other.z) || !almostequal(w,other.w);
        }

        //constexpr vec4 cross(const vec4& other) const{
        //    return {y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x};
        //}

            // Stream output
        friend std::ostream& operator<<(std::ostream& os, const vec4& v) {
            return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        }

    };

    template<typename T>
    constexpr vec4<T> operator*(T scalar, const vec4<T>& v) {
        return v * scalar;
    }

    template<typename T>
    constexpr vec4<T> clamp(const vec4<T>& v, const vec4<T>& min, const vec4<T>& max){
            return {clamp_scalar(v.x, min.x, max.x),clamp_scalar(v.y,min.y, max.y), 
                    clamp_scalar(v.z,min.z, max.z),clamp_scalar(v.w,min.w, max.w)};
        }

    template<typename T>
    constexpr vec4<T> lerp(const vec4<T>& a, const vec4<T>& b, const vec4<T>& t){
        if (t.x < T(0) || t.x > T(1) || t.y < T(0) || t.y > T(1) || t.z < T(0) || t.z > T(1)|| t.w < T(0) || t.w > T(1))
            throw std::invalid_argument("t vector values must be between 0 and 1");
        return {a.x * (T(1) - t.x) + b.x * t.x, a.y * (T(1) - t.y) + b.y * t.y, a.z * (T(1) - t.z) + b.z * t.z, a.w * (T(1) - t.w) + b.w * t.w};
    }

    template<typename T>
    constexpr vec4<T> lerp(const vec4<T>& a, const vec4<T>& b, const T t){
        if (t < T(0) || t > T(1))
            throw std::invalid_argument("t must be between 0 and 1");
        return {a.x * (T(1) - t) + b.x * t, a.y * (T(1) - t) + b.y * t, a.z * (T(1) - t) + b.z * t, a.w * (T(1) - t) + b.w * t};
    }

    template<typename T>
    T angle_between(const vec4<T>& a, const vec4<T>& b) {
        static_assert(std::is_floating_point<T>::value, "Requires floating-point type");
        T zero_check = a.length() * b.length();
        if (zero_check == T(0))
            throw std::domain_error("Cannot compute angle between zero-length vectors");
        T cos_theta = a.dot(b) / (a.length() * b.length());
        return std::acos(clamp_scalar(cos_theta, T(-1), T(1)));  // Clamp to prevent domain error
    }

    template<typename T>
    constexpr vec4<T> step(const vec4<T>& edge, const vec4<T>& x) {
        return {step_scalar(edge.x, x.x),step_scalar(edge.y, x.y),
                step_scalar(edge.z, x.z),step_scalar(edge.w, x.w)};
    }

    template<typename T>
    constexpr vec4<T> smoothstep(const vec4<T>& edge, const vec4<T>& x) {
        return {smoothstep_scalar(edge.x, x.x),smoothstep_scalar(edge.y, x.y),
                smoothstep_scalar(edge.z, x.z),smoothstep_scalar(edge.w, x.w)};
    }

    template<typename T>
    vec4<T> faceforward(const vec4<T>& n, const vec4<T>& i, const vec4<T>& nref) {
        return (nref.dot(i) < T(0)) ? n : -n;
    }

    template<typename T>
    constexpr vec4<T> reflect(const vec4<T>& v, const vec4<T>& n) {
        static_assert(std::is_floating_point<T>::value, "Requires floating-point type");
        vec4<T> n_norm = n.normalize();
        return v - T(2) * v.dot(n_norm) * n_norm;
    }

    template<typename T>
    constexpr vec4<T> refract(const vec4<T>& v, const vec4<T>& n, T eta) {
        static_assert(std::is_floating_point<T>::value, "Requires floating-point type");
        vec4<T> n_norm = n.normalize();
        T cosI = -n_norm.dot(v);
        T sinT2 = eta * eta * (T(1) - cosI * cosI);
        if (sinT2 > T(1)) return vec4<T>(T(0)); // Total internal reflection
        T cosT = std::sqrt(T(1) - sinT2);
        return eta * v + (eta * cosI - cosT) * n_norm;
    }

    template<typename T>
    constexpr vec4<T> min(const vec4<T>& a, const vec4<T>& b) {
        return {std::min(a.x,b.x), std::min(a.y,b.y),
                std::min(a.z,b.z), std::min(a.w,b.w)};
    }

    template<typename T>
    constexpr vec4<T> max(const vec4<T>& a, const vec4<T>& b) {
        return {std::max(a.x,b.x), std::max(a.y,b.y),
                std::max(a.z,b.z), std::max(a.w,b.w)};
    }

    template<typename T>
    constexpr vec4<T> abs(const vec4<T>& a) {
        return {std::abs(a.x), std::abs(a.y),
                std::abs(a.z), std::abs(a.w)};
    }

    using vec4f = vec4<float>;
    using vec4d = vec4<double>;
    using vec4i = vec4<int>;
}