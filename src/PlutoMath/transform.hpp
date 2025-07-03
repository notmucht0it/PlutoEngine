#pragma once

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

namespace plutom {
namespace transform2D{
    template<typename T>
    constexpr mat3<T> scale(const vec2<T>& s){
        mat3<T> iden = mat3<T>::identity();
        iden[0][0] = s.x;
        iden[1][1] = s.y;
        return  iden;
    }

    template<typename T>
    constexpr mat3<T> scale(T s){
        mat3<T> iden = mat3<T>::identity();
        iden[0][0] = s;
        iden[1][1] = s;
        return  iden;
    }
    
    template<typename T>
    constexpr mat3<T> translate(const vec2<T>& s){
        mat3<T> iden = mat3<T>::identity();
        iden[0][2] = s.x;
        iden[1][2] = s.y;
        return  iden;
    }

    template<typename T>
    constexpr mat3<T> translate(T s){
        mat3<T> iden = mat3<T>::identity();
        iden[0][2] = s;
        iden[1][2] = s;
        return  iden;
    }

    template<typename T>
    constexpr mat3<T> rotate(T theta) {
    T c = std::cos(theta);
    T s = std::sin(theta);
    return mat3<T>{
        c, -s, T(0),
        s,  c, T(0),
        T(0), T(0), T(1)};
    }

    template<typename T>
    constexpr mat3<T> trs(const vec2<T>& t,T theta,const vec2<T>& s) {
        return translate(t) * rotate(theta) * scale(s);
    }

    template<typename T>
    constexpr mat3<T> trs(T t,T theta,T s) {
        return translate(t) * rotate(theta) * scale(s);
    }
}
namespace transform3D{

    template<typename T>
    constexpr mat4<T> scale(const vec3<T>& s){
        mat4<T> iden = mat4<T>::identity();
        iden[0][0] = s.x;
        iden[1][1] = s.y;
        iden[2][2] = s.z;
        return  iden;
    }

    template<typename T>
    constexpr mat4<T> scale(T s){
        mat4<T> iden = mat4<T>::identity();
        iden[0][0] = s;
        iden[1][1] = s;
        iden[2][2] = s;
        return  iden;
    }

    template<typename T>
    constexpr mat4<T> translate(const vec3<T>& s){
        mat4<T> iden = mat4<T>::identity();
        iden[0][3] = s.x;
        iden[1][3] = s.y;
        iden[2][3] = s.z;
        return  iden;
    }

    template<typename T>
    constexpr mat4<T> translate(T s){
        mat4<T> iden = mat4<T>::identity();
        iden[0][3] = s;
        iden[1][3] = s;
        iden[2][3] = s;
        return  iden;
    }

    template<typename T>
    constexpr mat4<T> rotateX(T theta){
        return mat4<T>{T(1),T(0),T(0),T(0),
                       T(0), std::cos(theta), std::sin(theta),T(0),
                       T(0),-std::sin(theta), std::cos(theta),T(0),
                       T(0),T(0),T(0),T(1)};
    }

    template<typename T>
    constexpr mat4<T> rotateY(T theta){
        return mat4<T>{std::cos(theta),T(0),std::sin(theta),T(0),
                       T(0), T(1), T(0),T(0),
                       -std::sin(theta),T(0), std::cos(theta),T(0),
                       T(0),T(0),T(0),T(1)};
    }

    template<typename T>
    constexpr mat4<T> rotateZ(T theta){
        return mat4<T>{std::cos(theta),-std::sin(theta),T(0),T(0),
                       std::sin(theta), std::cos(theta),T(0),T(0),
                       T(0),T(0),T(1),T(0),
                       T(0),T(0),T(0),T(1)};
    }
}
}