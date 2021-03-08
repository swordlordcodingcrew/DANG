// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 28.12.19.
//

#include <iostream>
#include "../Sprite.hpp"
#include "TwAnim.hpp"
#include "../dang.hpp"

namespace dang
{
    /**
     * default constructor
     */
    TwAnim::TwAnim() : Tweenable()
    {

    }

    /**
     *
     * @param indices range of image-indices relating to the imagesheet
     * @param duration duration of loop.
     * @param ease ease function
     * @param loops number of loops
     * @param alternating if true, the animation will reverse for every second loop
     * @param delay delay until loop shall start. Is applied for each loop
     */
    TwAnim::TwAnim(const std::vector<uint16_t> &indices, uint32_t duration, EaseFn ease_cb,
                   int32_t loops, bool alternating, uint32_t delay)
            : Tweenable(duration, ease_cb, loops, alternating, delay)
    {
        _indices = indices;
    }

    /**
     * This function updates the _img_index of the sprite which is stored in _the_object
     *
     * @param time needed for updating the tween
     */
    void TwAnim::update(uint32_t dt)
    {
        spSprite spr = std::static_pointer_cast<Sprite>(_the_object.lock());
        if (!spr) return;

        float fx = calc(dt);
        uint16_t ind = uint16_t(fx * (_indices.size()));

        // for the case that fx == 1, index equals _indices.size()
        if (ind >= _indices.size())
        {
            ind = uint16_t(_indices.size() - 1);
        }

        spr->_img_index = _indices[ind];
    }

}