// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

namespace dang
{
    static const std::vector<float> explosion = {
            0.2f, 0.2f, 0.4f, 0.6f, 0.8f, // 100ms
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 200ms
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 300ms
            0.9f, 0.8f, 0.7f, 0.6f, 0.5f, // 400ms
            0.5f, 0.4f, 0.4f, 0.3f, 0.3f, // 500ms
            0.1f, 0.1f, 0.1f, 0.1f, 0.1f, // 600ms
    };

    static const std::vector<float> double_knock = {
            0.2f, 0.4f, 0.6f, 0.8f, 1.0f, // 100ms
            1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // 200ms
            0.8f, 0.6f, 0.5f, 0.6f, 0.8f, // 300ms
            0.6f, 0.8f, 1.0f, 1.0f, 1.0f, // 400ms
            0.8f, 0.6f, 0.3f, 0.1f, 0.1f, // 500ms
    };

    static const std::vector<float> poke = {
            0.2f, 0.3f, 0.4f, 0.3f, 0.2f, // 100ms
    };
}



