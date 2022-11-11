// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwAccY.hpp"
#include "sprite/FullColSpr.hpp"
#include "sprite/FullImgSpr.hpp"

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
    TwAccY::TwAccY(const float start_acc_y, const float end_acc_y, uint32_t duration,  EaseFn ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : Tweenable(duration, ease_cb, loops, alternating, delay), _start_acc_y(start_acc_y), _end_acc_y(end_acc_y)
    {

    }

    void TwAccY::update(FullColSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setAccY(_start_acc_y + (_end_acc_y - _start_acc_y) * fx);
    }

    void TwAccY::update(FullImgSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setAccY(_start_acc_y + (_end_acc_y - _start_acc_y) * fx);
    }

    TwAccY::~TwAccY()
    {
        D_DEBUG_PRINT("TwAcc destroyed\n");
    }

}