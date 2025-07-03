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
    constexpr mat3<T> scale(const mat3<T>& mat,const vec2<T>& s){
        return  mat * scale(s);
    }

    template<typename T>
    constexpr mat3<T> scale(T s){
        mat3<T> iden = mat3<T>::identity();
        iden[0][0] = s;
        iden[1][1] = s;
        return  iden;
    }

    template<typename T>
    constexpr mat3<T> scale(const mat3<T>& mat, T s){
        return  mat * scale(s);
    }
    
    template<typename T>
    constexpr mat3<T> translate(const vec2<T>& s){
        mat3<T> iden = mat3<T>::identity();
        iden[2][0] = s.x;
        iden[2][1] = s.y;
        return  iden;
    }

    template<typename T>
    constexpr mat3<T> translate(const mat3<T>& mat, const vec2<T>& s){
        return  mat * translate(s);
    }

    template<typename T>
    constexpr mat3<T> translate(T s){
        mat3<T> iden = mat3<T>::identity();
        iden[2][0] = s;
        iden[2][1] = s;
        return  iden;
    }

    template<typename T>
    constexpr mat3<T> translate(const mat3<T>& mat, T s){
        return  mat * translate(s);
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
    constexpr mat3<T> rotate(const mat3<T>& mat, T theta) {
        return mat * rotate(theta);
    }

    template<typename T>
    constexpr mat3<T> trs(const vec2<T>& t,T theta,const vec2<T>& s) {
        return translate(t) * rotate(theta) * scale(s);
    }

    template<typename T>
    constexpr mat3<T> trs(T t,T theta,T s) {
        return translate(t) * rotate(theta) * scale(s);
    }

    template<typename T>
    constexpr mat3<T> trs(const mat3<T>& mat, const vec2<T>& t,T theta,const vec2<T>& s) {
        return mat * translate(t) * rotate(theta) * scale(s);
    }

    template<typename T>
    constexpr mat3<T> trs(const mat3<T>& mat, T t,T theta,T s) {
        return mat * translate(t) * rotate(theta) * scale(s);
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
    constexpr mat4<T> scale(const mat4<T>& mat, const vec3<T>& s){
        return mat * scale(s);
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
    constexpr mat4<T> scale(const mat4<T>& mat, T s){
        return mat * scale(s);
    }

    template<typename T>
    constexpr mat4<T> translate(const vec3<T>& s){
        mat4<T> iden = mat4<T>::identity();
        iden[3][0] = s.x;
        iden[3][1] = s.y;
        iden[3][2] = s.z;
        return  iden;
    }

    template<typename T>
    constexpr mat4<T> translate(const mat4<T>& mat, const vec3<T>& s){
        return mat * translate(s);
    }

    template<typename T>
    constexpr mat4<T> translate(T s){
        mat4<T> iden = mat4<T>::identity();
        iden[3][0] = s;
        iden[3][1] = s;
        iden[3][2] = s;
        return  iden;
    }

    template<typename T>
    constexpr mat4<T> translate(const mat4<T>& mat, T s){
        return mat * translate(s);
    }

    template<typename T>
    constexpr mat4<T> rotateX(T theta){
        return mat4<T>{T(1),T(0),T(0),T(0),
                       T(0), std::cos(-theta), std::sin(-theta),T(0),
                       T(0),-std::sin(-theta), std::cos(-theta),T(0),
                       T(0),T(0),T(0),T(1)};
    }

    template<typename T>
    constexpr mat4<T> rotateX(const mat4<T>& mat, T theta){
        return mat * rotateX(theta);
    }

    template<typename T>
    constexpr mat4<T> rotateY(T theta){
        return mat4<T>{std::cos(-theta),T(0),std::sin(-theta),T(0),
                       T(0), T(1), T(0),T(0),
                       -std::sin(-theta),T(0), std::cos(-theta),T(0),
                       T(0),T(0),T(0),T(1)};
    }

    template<typename T>
    constexpr mat4<T> rotateY(const mat4<T>& mat, T theta){
        return mat * rotateY(theta);
    }

    template<typename T>
    constexpr mat4<T> rotateZ(T theta){
        return mat4<T>{std::cos(-theta),-std::sin(-theta),T(0),T(0),
                       std::sin(-theta), std::cos(-theta),T(0),T(0),
                       T(0),T(0),T(1),T(0),
                       T(0),T(0),T(0),T(1)};
    }

    template<typename T>
    constexpr mat4<T> rotateZ(const mat4<T>& mat, T theta){
        return mat * rotateZ(theta);
    }

    template<typename T>
    constexpr mat4<T> rotate(T theta, const vec3<T>& axis){
        vec3<T> axisNorm = axis.normalize();
        T cos = std::cos(-theta);
        T oCos = T(1) - cos;
        T sin = std::sin(-theta);
        T x = axisNorm.x, y = axisNorm.y, z = axisNorm.z;
        return mat4<T>{ x * x * oCos + cos, x * y * oCos + z * sin, x * z * oCos - y * sin, T(0),
                        x * y * oCos - z * sin, y * y * oCos + cos, y * z * oCos + x * sin, T(0),
                        x * z * oCos + y * sin, y * z * oCos - x * sin, z * z * oCos + cos, T(0),
                        T(0), T(0), T(0), T(1)};
    }

    template<typename T>
    constexpr mat4<T> rotate(const mat4<T>& mat, T theta, const vec3<T>& axis){
        return mat * rotate(theta, axis);
    }
}
}