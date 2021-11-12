// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <cassert>

#include "../Sprite.hpp"
#include "TwAccX.hpp"

namespace dang
{
    /**
     * default constructor
     */
    TwAccX::TwAccX() : Tweenable()
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
    TwAccX::TwAccX(const float start_acc_x, const float end_acc_x, uint32_t duration,  EaseFn ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : _start_acc_x(start_acc_x), _end_acc_x(end_acc_x), Tweenable(duration, ease_cb, loops, alternating, delay)
    {

    }

    /**
     * This function updates vel of the sprite which is stored in _the_object
     *
     * @param dt needed for updating the tween
     */
    void TwAccX::update(void* obj, uint32_t dt)
    {
        assert(obj != nullptr);
        Sprite* spr = static_cast<Sprite*>(obj);

        float fx = calc(dt);
        spr->setAccX(_start_acc_x + (_end_acc_x - _start_acc_x) * fx);
    }

    TwAccX::~TwAccX()
    {
//        std::cout << "TwAcc destroyed" << std::endl;
    }

}