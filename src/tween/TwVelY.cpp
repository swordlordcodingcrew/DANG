// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwVelY.hpp"
#include "sprite/FullImgSpr.hpp"
#include "sprite/FullColSpr.hpp"

namespace dang
{
    /**
     *
     * @param start_vel start velocity
     * @param end_vel end velocity
     * @param duration duration of loop.
     * @param ease ease function
     * @param loops number of loops
     * @param alternating if true, the animation will reverse for every second loop
     * @param delay delay until loop shall start. Is applied for each loop
     */
    TwVelY::TwVelY(float start_vel_y, float end_vel_y, uint32_t duration, EaseFn ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : _start_vel_y(start_vel_y), _end_vel_y(end_vel_y), Tweenable(duration, ease_cb, loops, alternating, delay)
    {

    }

    void TwVelY::update(FullColSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setVelY(_start_vel_y + (_end_vel_y - _start_vel_y) * fx);
    }

    void TwVelY::update(FullImgSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setVelY(_start_vel_y + (_end_vel_y - _start_vel_y) * fx);
    }

    TwVelY::~TwVelY()
    {
        D_DEBUG_PRINT("TwVelY destroyed\n");
    }

}