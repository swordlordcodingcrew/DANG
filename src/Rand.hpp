// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <random>

namespace dang
{
    class Rand
    {
    public:
        Rand() = delete;
        static void seed();

        static uint32_t get(uint32_t min, uint32_t max);
        static int32_t  get(int32_t min, int32_t max);
        static float    get(float min, float max);

    protected:
        static std::default_random_engine gen;
    };

}


