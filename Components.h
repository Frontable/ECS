#pragma once
#include "FrostMath.h"
#include "Rendering/Essentials/Texture.h"

struct Transform2D
{
    vec2 position;
    float rotation = 0.0f;
    vec2 scale = { 1.0f, 1.0f };
};

struct Velocity2D
{
    vec2 velocity;
    float angularVelocity = 0.0f;
};

struct Sprite
{
    vec2 uv;
    vec2 size;
};

struct CircleCollider
{
    float radius = 10.0f;
};

struct Lifetime
{
    float timeLeft = 1.0f;
};

struct PlayerTag {};
struct AsteroidTag {};
struct BulletTag {};







