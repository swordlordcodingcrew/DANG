// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "Vector2T.hpp"

namespace dang
{
    class TwAcc : public Tweenable
    {
    public:
        TwAcc() = default;
        TwAcc(const TwAcc& tw) = default;
        ~TwAcc() override;
        TwAcc(const Vector2F& start_acc, const Vector2F& end_acc, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(FullImgSpr& obj, uint32_t dt) override;
        void        update(FullColSpr& obj, uint32_t dt) override;

    protected:
        Vector2F    _start_acc{0, 0};
        Vector2F    _end_acc{0, 0};

    };

}


