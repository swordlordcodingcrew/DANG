// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "Vector2T.hpp"

namespace dang
{

    class TwPos : public Tweenable
    {
    public:
        TwPos() = default;
        TwPos(const TwPos& tw) = default;
        TwPos(const Vector2F& move_to, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void    init(void* obj) override;
        void    update(void* obj, uint32_t dt) override;

    protected:
        Vector2F    _move_to{0, 0};
        Vector2F    _start_from{0, 0};

    };

}


