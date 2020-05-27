// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include <iostream>
#include <Sprite.h>
#include "TwPos.h"
#include "../dang_globals.hpp"

namespace dang
{
    /**
     * default constructor
     */
    TwPos::TwPos() : Tweenable()
    {

    }

    /**
     *
     * @param the_object to be manipulated. Shall be a sprite
     * @param indices range of image-indices relating to the imagesheet
     * @param duration duration of loop.
     * @param ease ease function
     * @param loops number of loops
     * @param alternating if true, the animation will reverse for every second loop
     * @param delay delay until loop shall start. Is applied for each loop
     */
    TwPos::TwPos(const Vector2F& move_to, uint32_t duration, std::unique_ptr<Ease> ease,
                 int32_t loops, bool alternating, uint32_t delay)
            : _move_to(move_to), Tweenable(duration, std::move(ease), loops, alternating, delay)
    {
        spSprite spr = std::static_pointer_cast<Sprite>(_the_object.lock());
        if (!spr) return;

        _start_from.x = spr->getPos().x;
        _start_from.y = spr->getPos().y;
    }

    /**
     * This function updates pos of the sprite which is stored in _the_object
     *
     * @param time needed for updating the tween
     */
    void TwPos::update(uint32_t time)
    {
        spSprite spr = std::static_pointer_cast<Sprite>(_the_object.lock());
        if (!spr) return;

        float fx = calc(time);
        spr->setPos(Vector2F(_start_from.x + (_move_to.x - _start_from.x) * fx, _start_from.y + (_move_to.y - _start_from.y) * fx));

    }

}