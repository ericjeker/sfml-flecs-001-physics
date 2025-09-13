// Copyright (c) Eric Jeker 2025.

#pragma once

#include <cstdint>


namespace Random
{

void Seed(std::uint32_t seed);
float UniformFloat(float min, float max);
int UniformInt(int min, int max);

}
