// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "Vector2T.hpp"

namespace dang
{
    class TwVel : public Tweenable
    {
    public:
        TwVel();
        ~TwVel() override;
        TwVel(const Vector2F& start_vel, const Vector2F& end_vel, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(FullImgSpr& obj, uint32_t dt) override;
        void        update(FullColSpr& obj, uint32_t dt) override;

    protected:
        Vector2F  _start_vel{0, 0};
        Vector2F   _end_vel{0, 0};

    };

}


