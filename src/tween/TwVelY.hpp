// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "Vector2T.hpp"

namespace dang
{
    class TwVelY : public Tweenable
    {
    public:
        TwVelY() = default;
        ~TwVelY() override;
        TwVelY(float start_vel_y, float end_vel_y, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(FullImgSpr& obj, uint32_t dt) override;
        void        update(FullColSpr& obj, uint32_t dt) override;

    protected:
        float  _start_vel_y{0};
        float  _end_vel_y{0};

    };

}


