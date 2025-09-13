// Copyright (c) Eric Jeker 2025.

#include "core/utils/Random.h"

#include <random>

#include <cassert>

namespace
{

std::mt19937 gen(std::random_device{}());

} // namespace

void Random::Seed(const std::uint32_t seed)
{
    gen.seed(seed);
}

float Random::UniformFloat(const float min, const float max)
{
    assert(min <= max && "Min must be less than max");
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

int Random::UniformInt(const int min, const int max)
{
    assert(min <= max && "Min must be less than max");
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}
