// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <vector>
#include <cstdint>
#include "Ease.hpp"

namespace dang
{
    float Ease::OutCubic(const float x)
    {
        float f = x - 1;
        return 1 + f * f * f;

    }

    float Ease::InOutCubic(const float x)
    {
        if (x < 0.5f)
        {
            return 4 * x * x * x;
        }
        else
        {
            float f = ((2 * x) - 2);
            return 0.5f * f * f * f + 1;
        }
//        return x < 0.5f ? 4 * x * x * x : 1 + (--x) * (2 * (--x)) * (2 * x);
    }
}