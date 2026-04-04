#pragma once
#include "FrostMath.h"

struct Transform2D
{
    Transform2D() = default;
    vec2 position;
    float rotation;
    vec2 scale;
};