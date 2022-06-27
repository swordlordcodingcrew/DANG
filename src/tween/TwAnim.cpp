// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwAnim.hpp"
#include "sprite/ImageObject.hpp"

#include <cassert>

namespace dang
{

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
     *
     * @param indices range of image-indices relating to the imagesheet
     * @param duration duration of loop.
     * @param ease ease function
     * @param loops number of loops
     * @param alternating if true, the animation will reverse for every second loop
     * @param delay delay until loop shall start. Is applied for each loop
     */
    TwAnim::TwAnim(const spImagesheet is, const std::vector<uint16_t> &indices, uint32_t duration, EaseFn ease_cb,
                   int32_t loops, bool alternating, uint32_t delay)
            : Tweenable(duration, ease_cb, loops, alternating, delay)
    {
        _indices = indices;
        _imagesheet = is;
    }

    /**
     * This function updates the _img_index of the sprite which is stored in _the_object
     *
     * @param time needed for updating the tween
     */
    void TwAnim::update(void* obj, uint32_t dt)
    {
        assert(obj != nullptr);
        ImageObject* spr = static_cast<ImageObject*>(obj);

        float fx = calc(dt);
        uint16_t ind = uint16_t(fx * (_indices.size()));

        // for the case that fx == 1, index equals _indices.size()
        if (ind >= _indices.size())
        {
            ind = uint16_t(_indices.size() - 1);
        }

        spr->setImgIndex(_indices[ind]);
    }

    void TwAnim::init(void *obj)
    {
        if (_imagesheet != nullptr)
        {
            assert(obj != nullptr);
            ImageObject* spr = static_cast<ImageObject*>(obj);
            spr->setImagesheet(_imagesheet);
            spr->setImgIndex(_indices[0]);
        }
    }

}