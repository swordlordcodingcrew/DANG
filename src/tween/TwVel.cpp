// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>

#include "Sprite.h"
#include "TwVel.h"

namespace dang
{
    /**
     * default constructor
     */
    TwVel::TwVel() : Tweenable()
    {

    }

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
    TwVel::TwVel(const Vector2F& start_vel, const Vector2F& end_vel, uint32_t duration, std::unique_ptr<Ease> ease,
                 int32_t loops, bool alternating, uint32_t delay)
            : _start_vel(start_vel), _end_vel(end_vel), Tweenable(duration, std::move(ease), loops, alternating, delay)
    {

    }

    /**
     * This function updates vel of the sprite which is stored in _the_object
     *
     * @param time needed for updating the tween
     */
    void TwVel::update(uint32_t time)
    {
        spSprite spr = std::static_pointer_cast<Sprite>(_the_object.lock());
        if (!spr) return;

        float fx = calc(time);
        spr->setVel(_start_vel + (_end_vel - _start_vel) * fx);

    }

    TwVel::~TwVel()
    {
        std::cout << "TwVel destroyed" << std::endl;
    }

}