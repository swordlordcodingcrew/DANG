// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"

namespace dang
{
    class TwAccX : public Tweenable
    {
    public:
        TwAccX() = default;
        TwAccX(const TwAccX& tw) = default;
        ~TwAccX() override;
        TwAccX(const float start_acc_x, const float end_acc_x, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(void* obj, uint32_t dt) override;

    protected:
        float    _start_acc_x{0};
        float    _end_acc_x{0};

    };

}


