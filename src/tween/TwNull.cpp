// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwNull.hpp"


namespace dang
{
    /**
     * default constructor
     */
    TwNull::TwNull() : Tweenable()
    {

    }

    /**
     *
     * @param duration duration of loop.
     * @param ease ease function
     * @param loops number of loops
     * @param alternating if true, the animation will reverse for every second loop
     * @param delay delay until loop shall start. Is applied for each loop
     */
    TwNull::TwNull(uint32_t duration, EaseFn ease_cb, int32_t loops, uint32_t delay)
            : Tweenable(duration, ease_cb, loops, false, delay)
    {
    }

    void TwNull::init(void* obj)
    {
        // nothing to be done
    }

    /**
     * This function does nothing since it is a Null Tween (except adding progress via calc)
     *
     * @param time needed for updating the tween
     */
    void TwNull::update(void* obj, uint32_t dt)
    {
        // have the base class calculate diff. No calc, no progress
        calc(dt);
    }
}