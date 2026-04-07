#pragma once
#include <cstdint>
#include <bitset>

//ID
using ID = std::uint8_t;

//Entity
using Entity = std::uint32_t;
using Signature = std::bitset<MAX_COMPONENTS>;
constexpr Entity MAX_ENTITIES = 5000;

//Component
using ComponentType = std::uint8_t;
constexpr ComponentType MAX_COMPONENTS = 32;

//System