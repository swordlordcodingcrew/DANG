// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwVel.hpp"
#include "sprite/MotionObject.hpp"

#include <cassert>

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
    TwVel::TwVel(const Vector2F& start_vel, const Vector2F& end_vel, uint32_t duration, EaseFn ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : _start_vel(start_vel), _end_vel(end_vel), Tweenable(duration, ease_cb, loops, alternating, delay)
    {

    }

    /**
     * This function updates vel of the sprite which is stored in _the_object
     *
     * @param time needed for updating the tween
     */
    void TwVel::update(void* obj, uint32_t dt)
    {
        assert(obj != nullptr);
        MotionObject* spr = static_cast<MotionObject*>(obj);

        float fx = calc(dt);
        spr->setVel(_start_vel + (_end_vel - _start_vel) * fx);
    }

    TwVel::~TwVel()
    {
        D_DEBUG_PRINT("TwVel destroyed\n");
    }

}