// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwPos.hpp"
#include "sprite/SpriteObject.hpp"

#include <cassert>

namespace dang
{
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
    TwPos::TwPos(const Vector2F& move_to, uint32_t duration, EaseFn ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : _move_to(move_to), Tweenable(duration, ease_cb, loops, alternating, delay)
    {
    }

    void TwPos::init(void* obj)
    {
        assert(obj != nullptr);
        SpriteObject* spr = static_cast<SpriteObject*>(obj);
        _start_from.x = spr->getPos().x;
        _start_from.y = spr->getPos().y;
    }

    /**
     * This function updates pos of the sprite
     *
     * @param time needed for updating the tween
     */
    void TwPos::update(void* obj, uint32_t dt)
    {
        assert(obj != nullptr);
        SpriteObject* spr = static_cast<SpriteObject*>(obj);

        float fx = calc(dt);
        spr->setPos(_start_from.x + (_move_to.x - _start_from.x) * fx, _start_from.y + (_move_to.y - _start_from.y) * fx);
    }
}