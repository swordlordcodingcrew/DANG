// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwPos.hpp"
#include "sprite/FullColSpr.hpp"
#include "sprite/FullImgSpr.hpp"

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

    void TwPos::init(FullColSpr& obj)
    {
        _start_from = obj.getPos();
    }

    void TwPos::init(FullImgSpr& obj)
    {
        _start_from = obj.getPos();
    }

    void TwPos::update(FullImgSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setPos(_start_from.x + (_move_to.x - _start_from.x) * fx, _start_from.y + (_move_to.y - _start_from.y) * fx);
    }

    void TwPos::update(FullColSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setPos(_start_from.x + (_move_to.x - _start_from.x) * fx, _start_from.y + (_move_to.y - _start_from.y) * fx);
    }

}
