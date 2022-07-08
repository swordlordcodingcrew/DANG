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
