// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwAccX.hpp"
#include "sprite/MotionObject.hpp"

#include <cassert>

namespace dang
{
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
     * This function updates vel of the sprite
     *
     * @param dt needed for updating the tween
     */
    void TwAccX::update(void* obj, uint32_t dt)
    {
        assert(obj != nullptr);
        MotionObject* spr = static_cast<MotionObject*>(obj);

        float fx = calc(dt);
        spr->setAccX(_start_acc_x + (_end_acc_x - _start_acc_x) * fx);
    }

    TwAccX::~TwAccX()
    {
        D_DEBUG_PRINT("TwAcc destroyed\n");
    }

}