// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwPosX.hpp"
#include "sprite/FullColSpr.hpp"
#include "sprite/FullImgSpr.hpp"

namespace dang
{
    /**
     *
     * @param the_object to be manipulated. Shall be a sprite
     * @param move_to dest point x
     * @param duration duration of loop.
     * @param ease ease function
     * @param loops number of loops
     * @param alternating if true, the animation will reverse for every second loop
     * @param delay delay until loop shall start. Is applied for each loop
     */
    TwPosX::TwPosX(const float move_to, uint32_t duration, EaseFn ease_cb,
                 int32_t loops, bool alternating, uint32_t delay)
            : Tweenable(duration, ease_cb, loops, alternating, delay), _move_to(move_to)
    {
    }

    void TwPosX::init(FullColSpr& obj)
    {
        _start_from = obj.getPosX();
    }

    void TwPosX::init(FullImgSpr& obj)
    {
        _start_from = obj.getPosX();
    }

    void TwPosX::update(FullImgSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setPosX(_start_from + (_move_to - _start_from) * fx);
    }

    void TwPosX::update(FullColSpr& obj, uint32_t dt)
    {
        float fx = calc(dt);
        obj.setPosX(_start_from + (_move_to - _start_from) * fx);
    }

}
