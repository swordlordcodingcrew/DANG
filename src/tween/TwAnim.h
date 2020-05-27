// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 28.12.19.
//

#pragma once

#include "Tweenable.h"

namespace dang
{
    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;

    class TwAnim : public Tweenable
    {
    public:
        TwAnim();
        TwAnim(const std::vector<uint16_t>& indices, uint32_t duration, std::unique_ptr<Ease> ease, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(uint32_t time);

    protected:
        std::vector<uint16_t>   _indices{0};

    };

}


