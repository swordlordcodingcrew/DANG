// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 16.01.20.
//

#include <iostream>
#include "TwMove.h"
#include "../dang_globals.hpp"

namespace dang
{
    /**
     * default constructor
     */
    TwMove::TwMove() : Tweenable()
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
    TwMove::TwMove(std::shared_ptr<void> the_object, const point& move_to, uint32_t duration, std::unique_ptr<Ease> ease,
                   int32_t loops, bool alternating, uint32_t delay)
            : _move_to(move_to), Tweenable(the_object, duration, std::move(ease), loops, alternating, delay)
    {
        if (_the_object != nullptr)
        {
            std::shared_ptr<sprite> spr = std::static_pointer_cast<sprite>(_the_object);
            _start_from.x = spr->x;
            _start_from.y = spr->y;
        }
    }

    /**
     * This function updates pos of the sprite which is stored in _the_object
     *
     * @param time needed for updating the tween
     */
    void TwMove::update(uint32_t time)
    {
        if (_the_object == nullptr) return;

        std::shared_ptr<sprite> spr = std::static_pointer_cast<sprite>(_the_object);

        float fx = calc(time);
        spr->x = int32_t(_start_from.x + (_move_to.x - _start_from.x) * fx);
        spr->y = int32_t(_start_from.y + (_move_to.y - _start_from.y) * fx);

    }

}