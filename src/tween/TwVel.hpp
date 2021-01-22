// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "../Vector2T.hpp"

namespace dang
{
    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;

    class TwVel : public Tweenable
    {
    public:
        TwVel();
        ~TwVel() override;
        TwVel(const Vector2F& start_vel, const Vector2F& end_vel, uint32_t duration, std::unique_ptr<Ease> ease, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(uint32_t dt) override;


    protected:
        Vector2F  _start_vel{0, 0};
        Vector2F   _end_vel{0, 0};

    };

}


