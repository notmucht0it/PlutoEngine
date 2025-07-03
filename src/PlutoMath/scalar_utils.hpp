#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>
namespace plutom{
    template<typename T>

    constexpr T clamp_scalar(T val, T min_val, T max_val){
        return (val < min_val) ? min_val : (val > max_val) ? max_val : val;
    }

    template<typename T>
    constexpr T step_scalar(T edge, T val){
        return (val > edge) ? T(1) : T(0);
    }

    template<typename T>
    constexpr T smoothstep_scalar(T edge0, T edge1, T val){
        T t = clamp_scalar((val - edge0) / (edge1 - edge0), T(0), T(1));
        return t * t * (T(3) - T(2) * t);
    }

    template<typename T>
    constexpr bool almostequal(T a, T b, T eps = std::numeric_limits<T>::epsilon() * 100){
        return std::abs(a - b) <= eps * std::max(T(1), std::max(std::abs(a), std::abs(b)));
    }

    template<typename T>
    constexpr T determinant(const T& a){
        return a;
    }

}