// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"

namespace dang
{
    class TwNull : public Tweenable
    {
    public:
        TwNull();
        TwNull(const TwNull& tw) = default;
        TwNull(const uint32_t duration, EaseFn ease_cb = Ease::Linear, int32_t loops = 1, uint32_t delay = 0);

        void        update(FullColSpr& obj, uint32_t dt) override;
        void        update(FullImgSpr& obj, uint32_t dt) override;

    protected:
        // none for now
    };
}


