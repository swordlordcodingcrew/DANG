// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "ImageObject.hpp"
#include "src/TmxExtruder.hpp"
#include "src/tween/Tweenable.hpp"
#include "src/Imagesheet.hpp"

#include <32blit.hpp>
#include <cassert>

namespace dang
{

    ImageObject::ImageObject(const ImageObject &io) :
    _visible(io._visible), _img_index(io._img_index), _imagesheet(io._imagesheet), _transform(io._transform)
    {
        assert(_imagesheet != nullptr);
        if (io._animation != nullptr)
        {
            setAnimation(std::make_shared<Tweenable>(*(io._animation)));
        }
        else
        {
            _animation = nullptr;
        }

    }

    ImageObject::ImageObject(const tmx_spriteobject *so, const spImagesheet &is)
    {
        _visible = so->visible;
        _img_index = so->tile;
        _imagesheet = is;

        assert(_imagesheet != nullptr);

        if (so->transform.test(0))
        {
            _transform |= blit::SpriteTransform::HORIZONTAL;
        }
        if (so->transform.test(1))
        {
            _transform |= blit::SpriteTransform::VERTICAL;
        }
        if (so->transform.test(2))
        {
            _transform |= blit::SpriteTransform::XYSWAP;
        }

    }

    ImageObject::~ImageObject()
    {
        removeAnimation(true);
    }

    void ImageObject::setAnimation(spTweenable twa)
    {
        _animation = twa;
        _animation->init(this);
    }

    spTweenable ImageObject::removeAnimation(bool suppressCB)
    {
        if (_animation)
        {
            _animation->finish(suppressCB);
            _animation->reset();
        }
        return std::move(_animation);
    }

    spTweenable ImageObject::swapAnimation(spTweenable new_anim, bool suppressCB)
    {
        spTweenable ret = removeAnimation(suppressCB);
        _animation = new_anim;
        return ret;
    }

    blit::Rect ImageObject::getBlitRect()
    {
        assert(_imagesheet != nullptr);
        blit::Rect sr = _imagesheet->getBlitRect(_img_index);
        return sr;
    }

    blit::Rect ImageObject::getBlitRect(const uint16_t img_index)
    {
        assert(_imagesheet != nullptr);
        blit::Rect sr = _imagesheet->getBlitRect(img_index);
        return sr;
    }

    void ImageObject::updateAnimation(uint32_t dt)
    {
        // special case animation. There can be only one
        if (_animation)
        {
            _animation->update(this, dt);
            if (_animation->isFinished())
            {
                _animation.reset();
            }
        }
    }

    blit::Surface *ImageObject::getSurface() const
    {
        assert(_imagesheet != nullptr);
         return _imagesheet->getSurface();
    }

    void ImageObject::setVisible(bool visible)
    {
        _visible = visible;
    }

}
