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
}