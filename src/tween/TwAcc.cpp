// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>

#include "Sprite.h"
#include "TwAcc.h"

namespace dang
{
    /**
     * default constructor
     */
    TwAcc::TwAcc() : Tweenable()
    {

    }

    /**
     *
     * @param start_acc start acceleration
     * @param end_acc end acceleration
     * @param duration duration of loop.
     * @param ease ease function
     * @param loops number of loops
     * @param alternating if true, the animation will reverse for every second loop
     * @param delay delay until loop shall start. Is applied for each loop
     */
    TwAcc::TwAcc(const Vector2F& start_acc, const Vector2F& end_acc, uint32_t duration, std::unique_ptr<Ease> ease,
                 int32_t loops, bool alternating, uint32_t delay)
            : _start_acc(start_acc), _end_acc(end_acc), Tweenable(duration, std::move(ease), loops, alternating, delay)
    {

    }

    /**
     * This function updates vel of the sprite which is stored in _the_object
     *
     * @param time needed for updating the tween
     */
    void TwAcc::update(uint32_t time)
    {
        spSprite spr = std::static_pointer_cast<Sprite>(_the_object.lock());
        if (!spr) return;

        float fx = calc(time);
        spr->setAcc(_start_acc + (_end_acc - _start_acc) * fx);

    }

    TwAcc::~TwAcc()
    {
        std::cout << "TwAcc destroyed" << std::endl;
    }

}