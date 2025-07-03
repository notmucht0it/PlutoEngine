#pragma once

#include <cmath>
#include <ostream>
#include <type_traits>
#include <stdexcept>

#include "scalar_utils.hpp"

namespace plutom{

    template<typename T>
    struct vec2{
        T x, y;

        constexpr vec2() : x(T(0)), y(T(0)) {}
        explicit constexpr vec2(T val) : x(val), y(val) {}
        explicit  constexpr vec2(T x, T y) : x(x), y(y) {}
        template<typename U>
        explicit constexpr vec2(const vec2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

        T& operator[](std::size_t i) {
            if (i == 0) return x;
            if (i == 1) return y;
            throw std::out_of_range("vec2 index must be 0 or 1");
        }
        const T& operator[](std::size_t i) const {
            if (i == 0) return x;
            if (i == 1) return y;
            throw std::out_of_range("vec2 index must be 0 or 1");
        }

        constexpr vec2 operator+(const vec2& other) const{
            return {x + other.x, y + other.y};
        }

        constexpr vec2 operator-(const vec2& other) const{
            return {x - other.x, y - other.y};
        }

        constexpr vec2 cwise_mul(const vec2& other) const{
            return {x*other.x, y*other.y};
        }

        constexpr vec2 operator*(T scalar) const{
            return {x*scalar, y*scalar};
        }

        constexpr vec2 operator/(T scalar) const{
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            return {x/scalar, y/scalar};
        }

        constexpr vec2 operator-() const{
            return {-x, -y};
        }

        constexpr vec2& operator+=(const vec2& other){
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr vec2& operator-=(const vec2& other){
            x -= other.x;
            y -= other.y;
            return *this;
        }

        constexpr vec2& operator*=(T scalar){
            x *= scalar;
            y *= scalar;
            return *this;
        }

        constexpr vec2& operator/=(T scalar){
            if(scalar == T(0)) throw std::domain_error("Cannot divide by zero");
            x /= scalar;
            y /= scalar;
            return *this;
        }

        constexpr T dot(const vec2& other) const{
            return x*other.x + y*other.y;
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

        vec2 normalize() const {
            static_assert(std::is_floating_point<T>::value, "normalize() only available for float/double types");
            T len = length();
            return len == T(0) ? *this : *this / len;
        }

        T distance(const vec2& other) const{
            static_assert(std::is_floating_point<T>::value, "distance() only available for float/double types");
            return std::sqrt((x - other.x)*(x - other.x) + (y - other.y)*(y - other.y));
        }

        constexpr T distance_squared(const vec2& other) const{
            return (x - other.x)*(x - other.x) + (y - other.y)*(y - other.y);
        }

        constexpr bool operator==(const vec2& other) const{
            return  almostequal(x,other.x) && almostequal(y,other.y);
        }

        constexpr bool operator!=(const vec2& other) const{
            return  !almostequal(x,other.x) || !almostequal(y,other.y);
        }

        T cross(const vec2& other) const{
            return x * other.y - y*other.x;
        }



            // Stream output
        friend std::ostream& operator<<(std::ostream& os, const vec2& v) {
            return os << "(" << v.x << ", " << v.y << ")";
        }

    };

    template<typename T>
    constexpr vec2<T> operator*(T scalar, const vec2<T>& v) {
        return v * scalar;
    }

    template<typename T>
    constexpr vec2<T> clamp(const vec2<T>& v, const vec2<T>& min, const vec2<T>& max){
            return {clamp_scalar(v.x, min.x, max.x), clamp_scalar(v.y,min.y, max.y)};
        }

    template<typename T>
    constexpr vec2<T> lerp(const vec2<T>& a, const vec2<T>& b, const vec2<T>& t){
        if (t.x < T(0) || t.x > T(1) || t.y < T(0) || t.y > T(1))
            throw std::invalid_argument("t vector values must be between 0 and 1");
        return {a.x * (T(1) - t.x) + b.x * t.x, a.y * (T(1) - t.y) + b.y * t.y};
    }

    template<typename T>
    constexpr vec2<T> lerp(const vec2<T>& a, const vec2<T>& b, const T t){
        if (t < T(0) || t > T(1))
            throw std::invalid_argument("t must be between 0 and 1");
        return {a.x * (T(1) - t) + b.x * t, a.y * (T(1) - t) + b.y * t};
    }

    template<typename T>
    T angle_between(const vec2<T>& a, const vec2<T>& b) {
        static_assert(std::is_floating_point<T>::value, "Requires floating-point type");
        T zero_check = a.length() * b.length();
        if (zero_check == T(0))
            throw std::domain_error("Cannot compute angle between zero-length vectors");
        T cos_theta = a.dot(b) / (a.length() * b.length());
        return std::acos(clamp_scalar(cos_theta, T(-1), T(1)));  // Clamp to prevent domain error
    }

    template<typename T>
    constexpr vec2<T> step(const vec2<T>& edge, const vec2<T>& x) {
        return {step_scalar(edge.x, x.x),step_scalar(edge.y, x.y)};
    }

    template<typename T>
    constexpr vec2<T> smoothstep(const vec2<T>& edge, const vec2<T>& x) {
        return {smoothstep_scalar(edge.x, x.x),smoothstep_scalar(edge.y, x.y)};
    }

    template<typename T>
    constexpr vec2<T> faceforward(const vec2<T>& n, const vec2<T>& i, const vec2<T>& nref) {
        return (nref.dot(i) < T(0)) ? n : -n;
    }

    template<typename T>
    constexpr vec2<T> reflect(const vec2<T>& v, const vec2<T>& n) {
        static_assert(std::is_floating_point<T>::value, "Requires floating-point type");
        vec2<T> n_norm = n.normalize();
        return v - T(2) * v.dot(n_norm) * n_norm;
    }

    template<typename T>
    constexpr vec2<T> refract(const vec2<T>& v, const vec2<T>& n, T eta) {
        static_assert(std::is_floating_point<T>::value, "Requires floating-point type");
        vec2<T> n_norm = n.normalize();
        T cosI = -n_norm.dot(v);
        T sinT2 = eta * eta * (T(1) - cosI * cosI);
        if (sinT2 > T(1)) return vec2<T>(T(0)); // Total internal reflection
        T cosT = std::sqrt(T(1) - sinT2);
        return eta * v + (eta * cosI - cosT) * n_norm;
    }

    template<typename T>
    constexpr vec2<T> min(const vec2<T>& a, const vec2<T>& b) {
        return {std::min(a.x,b.x), std::min(a.y,b.y)};
    }

    template<typename T>
    constexpr vec2<T> max(const vec2<T>& a, const vec2<T>& b) {
        return {std::max(a.x,b.x), std::max(a.y,b.y)};
    }

    template<typename T>
    constexpr vec2<T> abs(const vec2<T>& a) {
        return {std::abs(a.x), std::abs(a.y)};
    }

    using vec2f = vec2<float>;
    using vec2d = vec2<double>;
    using vec2i = vec2<int>;
}