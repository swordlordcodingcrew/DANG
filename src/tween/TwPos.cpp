// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwPos.hpp"
#include "../Sprite.hpp"
#include "../dang.hpp"

#include <iostream>

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
    TwPos::TwPos(const Vector2F& move_to, uint32_t duration, EaseFn& ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : _move_to(move_to), Tweenable(duration, ease_cb, loops, alternating, delay)
    {
        // removed the initialisation of start_from vector since
        // the_object cannot be set in this situation (creation of
        // tween comes first, it is then (later) added to the sprite).
        // See setObject
    }

    /**
     * This function sets the pointer of the sprite in _the_object
     * and initialises the start_from vector
     *
     * @param pointer to sprite
     */
    void  TwPos::setObject(std::weak_ptr<void> obj)
    {
        _the_object = obj;

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
    void TwPos::update(uint32_t dt)
    {
        spSprite spr = std::static_pointer_cast<Sprite>(_the_object.lock());
        if (!spr) return;

        float fx = calc(dt);
        spr->setPos(Vector2F(_start_from.x + (_move_to.x - _start_from.x) * fx, _start_from.y + (_move_to.y - _start_from.y) * fx));
    }
}