// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.h"
#include <Vector2T.h>

namespace dang
{
    class TwAcc : public Tweenable
    {
    public:
        TwAcc();
        ~TwAcc() override;
        TwAcc(std::shared_ptr<void> the_object, const Vector2F& start_acc, const Vector2F& end_acc, uint32_t duration, std::unique_ptr<Ease> ease, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void        update(uint32_t time) override;

    protected:
        Vector2F  _start_acc{0, 0};
        Vector2F   _end_acc{0, 0};

    };

}


