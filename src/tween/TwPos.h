// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 16.1.20.
//

#pragma once

#include "Tweenable.h"

namespace dang
{
    class TwPos : public Tweenable
    {
    public:
        TwPos();
        TwPos(std::shared_ptr<void> the_object, const Vec2& move_to, uint32_t duration, std::unique_ptr<Ease> ease, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(uint32_t time);


    protected:
        Vec2   _move_to{0, 0};
        Vec2   _start_from{0, 0};

    };

}

