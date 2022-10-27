// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"

namespace dang
{

    class TwPosY : public Tweenable
    {
    public:
        TwPosY() = default;
        TwPosY(const TwPosY& tw) = default;
        TwPosY(const float move_to, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void    init(FullColSpr& obj) override;
        void    init(FullImgSpr& obj) override;
        void    update(FullColSpr& obj, uint32_t dt) override;
        void    update(FullImgSpr& obj, uint32_t dt) override;

    protected:
        float    _move_to{0};
        float    _start_from{0};

    };

}


