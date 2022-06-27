// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "Vector2T.hpp"

namespace dang
{
    class TwAccY : public Tweenable
    {
    public:
        TwAccY() = default;
        TwAccY(const TwAccY& tw) = default;
        ~TwAccY() override;
        TwAccY(const float start_acc_y, const float end_acc_y, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(void* obj, uint32_t dt) override;

    protected:
        float    _start_acc_y{0};
        float    _end_acc_y{0};

    };

}


