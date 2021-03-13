// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 28.12.19.
//

#pragma once

#include "Tweenable.hpp"

namespace dang
{
    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;

    class TwAnim : public Tweenable
    {
    public:
        TwAnim();
        TwAnim(const TwAnim& tw) = default;
        TwAnim(const std::vector<uint16_t>& indices, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(void* obj, uint32_t dt);

    protected:
        std::vector<uint16_t>   _indices{0};

    };

}


