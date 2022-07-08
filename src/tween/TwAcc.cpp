// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwAcc.hpp"
#include "sprite/FullImgSpr.hpp"
#include "sprite/FullColSpr.hpp"

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
    TwAcc::TwAcc(const Vector2F& start_acc, const Vector2F& end_acc, uint32_t duration,  EaseFn ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : _start_acc(start_acc), _end_acc(end_acc), Tweenable(duration, ease_cb, loops, alternating, delay)
    {

    }

    void TwAcc::update(FullImgSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setAcc(_start_acc + (_end_acc - _start_acc) * fx);
    }

    void TwAcc::update(FullColSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setAcc(_start_acc + (_end_acc - _start_acc) * fx);
    }

    TwAcc::~TwAcc()
    {
        D_DEBUG_PRINT("TwAcc destroyed\n");
    }

}