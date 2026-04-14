#pragma once
#include "FrostMath.h"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

struct Transform2D
{
    Transform2D() = default;
    Transform2D(vec2 _pos, float _rotation, vec2 _size):position(_pos), rotation(_rotation), size(_size){}
    vec2 position;
    float rotation;
    vec2 size;

    mat4 GetModelMatrix() const
    {
        return translate(vec3(position.x, position.y, 0.0f)) * scale(vec3(size.x, size.y, 1.0f));
    }

};

struct RigidBody2D
{
    RigidBody2D() = default;
    RigidBody2D(vec2 _vel):velocity(_vel){}
    vec2 velocity;
};


struct ScriptComponent
{
    sol::protected_function update{ sol::lua_nil }, render{ sol::lua:nill }

};