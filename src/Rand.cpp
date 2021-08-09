// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Rand.hpp"
#include <chrono>

namespace dang
{
    std::default_random_engine Rand::gen(std::chrono::system_clock::now().time_since_epoch().count());

    uint32_t Rand::get(uint32_t min, uint32_t max)
    {
        std::uniform_int_distribution<uint32_t> distribution(min,max);
        return distribution(Rand::gen);
    }

    int32_t Rand::get(int32_t min, int32_t max)
    {
        std::uniform_int_distribution<int32_t> distribution(min,max);
        return distribution(Rand::gen);
    }

    float Rand::get(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min,max);
        return distribution(Rand::gen);
    }

    void Rand::seed()
    {
        uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count();
        Rand::gen.seed(seed);
    }
}
