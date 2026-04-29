#pragma once
#include "FrostMath.h"
#include "ECS/ECS.h"

struct Transform2D
{
    Transform2D() = default;
    Transform2D(vec2 _pos, float _rotation, vec2 _size) : position(_pos), rotation(_rotation), size(_size) {}
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
    RigidBody2D():RigidBody2D(vec2(0,0), 0){};
    RigidBody2D(vec2 _vel, float _gravity) : velocity(_vel), gravity(_gravity) {}
    vec2 velocity{};
    float gravity{};
    bool falling{true};
};

struct Rect
{
    float left = 0.0f;
    float top = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    float right() const { return left + width; }
    float bottom() const { return top + height; }
};

struct BoxCollider
{
    Rect rect;
};







