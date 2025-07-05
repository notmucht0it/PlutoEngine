#pragma once

#include "mat2.hpp"
#include "mat3.hpp"
#include "mat4.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace plutom{
    template<typename T>
    constexpr vec2<T> projection(const vec2<T>& v, const vec2<T>& onto){
        if(onto == vec2<T>()) throw std::domain_error("Cannot divide by zero");
        return (v.dot(onto) / onto.length_squared()) * onto;
    }

    template<typename T>
    constexpr vec3<T> projection(const vec3<T>& v, const vec3<T>& onto){
        if(onto == vec3<T>()) throw std::domain_error("Cannot divide by zero");
        return (v.dot(onto) / onto.length_squared()) * onto;
    }

    template<typename T>
    constexpr vec4<T> projection(const vec4<T>& v, const vec4<T>& onto){
        if(onto == vec4<T>()) throw std::domain_error("Cannot divide by zero");
        return (v.dot(onto) / onto.length_squared()) * onto;
    }

    template<typename T>
    constexpr vec2<T> projection(const vec2<T>& v, const mat2<T>& onto){
        vec2<T> result{};
        mat2<T> gram = onto.gramschimdt();
        for(int i = 0; i < 2; ++i){
            const vec2<T>& u = gram[i];
            result += (v.dot(u)) * u;
        }
        return result;
    }

    template<typename T>
    constexpr vec3<T> projection(const vec3<T>& v, const mat3<T>& onto){
        vec3<T> result{};
        mat3<T> gram = onto.gramschimdt();
        for(int i = 0; i < 3; ++i){
            const vec3<T>& u = gram[i];
            result += (v.dot(u)) * u;
        }
        return result;
    }

    template<typename T>
    constexpr vec4<T> projection(const vec4<T>& v, const mat4<T>& onto){
        vec4<T> result{};
        mat4<T> gram = onto.gramschimdt();
        for(int i = 0; i < 4; ++i){
            const vec4<T>& u = gram[i];
            result += (v.dot(u)) * u;
        }
        return result;
    }

    template<typename T>
    constexpr mat4<T> ortho(const T left, const T right, const T bottom, const T top, const T near, const T far){
        mat4<T> ret = mat4<T>::identity();
        ret[0][0] = 2 / (right - left);
        ret[1][1] = 2 / (top - bottom);
        ret[2][2] = -2 / (far - near);
        ret[3][0] = - (right + left) / (right - left);
        ret[3][1] = - (top + bottom) / (top - bottom);
        ret[3][2] = - (far + near) / (far - near);
        return ret;
    }

    template<typename T>
    constexpr mat4<T> perspective(const T fovRad, const T aspectRatio, const T near, const T far){
        const T f = 1 / std::tan(fovRad / 2);
        mat4<T> ret = mat4<T>();
        ret[0][0] = f / aspectRatio;
        // NOTE: Y-flip applied to match OpenGL's upward Y convention.
        ret[1][1] = -f;
        ret[2][2] = (far + near) / (near - far);
        ret[2][3] = -1;
        ret[3][2] = (2 * far * near) / (near - far);
        return ret;
    }

    /* Lookout Implemenations
        |  s.x   s.y   s.z  -dot(s, eye) |
        |  u.x   u.y   u.z  -dot(u, eye) |
        | -f.x  -f.y  -f.z   dot(f, eye) |
        |   0     0     0         1      |
     */
    template<typename T>
    constexpr mat4<T> lookAt(const vec3<T> cam, const vec3<T> tar, const vec3<T> up={T(0), T(1), T(0)}) {
        mat4<T> ret = mat4<T>::identity();
        mat4<T> retT = mat4<T>::identity();
        vec3<T> dir = (cam - tar).normalize();
        vec3<T> right = (up.normalize().cross(dir)).normalize();
        vec3<T> true_up = right.cross(dir);
        ret[0][0] = right.x; ret[1][0] = right.y; ret[2][0] = right.z;
        retT[3][0] = -cam.x;
        ret[0][1] = true_up.x; ret[1][1] = true_up.y; ret[2][1] = true_up.z;
        retT[3][1] = -cam.y;
        ret[0][2] = -dir.x; ret[1][2] = -dir.y; ret[2][2] = -dir.z;
        retT[3][2] = -cam.z;
        return ret * retT;
    }

}