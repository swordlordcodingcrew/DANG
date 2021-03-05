// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "../Vector2T.hpp"

namespace dang
{

    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;

    class TwPos : public Tweenable
    {
    public:
        TwPos();
        TwPos(const Vector2F& move_to, uint32_t duration, EaseFn& ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(uint32_t dt) override;
        void        setObject(std::weak_ptr<void> obj) override;

    protected:
        Vector2F  _move_to{0, 0};
        Vector2F   _start_from{0, 0};

    };

}


