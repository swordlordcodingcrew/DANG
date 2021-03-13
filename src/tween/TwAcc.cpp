// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <cassert>

#include "../Sprite.hpp"
#include "TwAcc.hpp"

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
    TwAcc::TwAcc(const Vector2F& start_acc, const Vector2F& end_acc, uint32_t duration,  EaseFn ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : _start_acc(start_acc), _end_acc(end_acc), Tweenable(duration, ease_cb, loops, alternating, delay)
    {

    }

    /**
     * This function updates vel of the sprite which is stored in _the_object
     *
     * @param dt needed for updating the tween
     */
    void TwAcc::update(void* obj, uint32_t dt)
    {
        assert(obj != nullptr);
        Sprite* spr = static_cast<Sprite*>(obj);

        float fx = calc(dt);
        spr->setAcc(_start_acc + (_end_acc - _start_acc) * fx);
    }

    TwAcc::~TwAcc()
    {
        std::cout << "TwAcc destroyed" << std::endl;
    }

}