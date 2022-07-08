// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "src/Vector2T.hpp"

#include <memory>

// forward declaration
namespace blit
{
    struct Rect;
    struct Surface;
}

namespace dang
{
    struct tmx_spriteobject;

    class Imagesheet;
    using spImagesheet = std::shared_ptr<Imagesheet>;

    class ImageObject
    {
    public:
        ImageObject() = default;
        ImageObject(const ImageObject& sp);
        ImageObject(const tmx_spriteobject* so, const spImagesheet& is);
        virtual ~ImageObject() = default;

        // image
        void setImagesheet(spImagesheet is) { _imagesheet = is; }
        blit::Rect getBlitRect();
        blit::Rect getBlitRect(const uint16_t img_index); // override where we can ask for a specific image
        blit::Surface* getSurface() const;
        const spImagesheet& getImagesheet() const { return _imagesheet; }
        uint8_t     getTransform() const { return _transform; }
        void        setTransform(uint8_t transform) { _transform = transform; }

        bool        isVisible() const { return _visible; }
        void        setVisible(bool visible);

        void        setImgIndex(uint16_t img_index) { _img_index = img_index; }

    private:
        uint16_t        _img_index{0};  // index to the image of the imagesheet. (equals tmx_tile of tmx_spriteobject?)
        uint8_t         _transform{0};      // transform for blitting
        spImagesheet    _imagesheet{nullptr};
        bool            _visible{true};

    };

}

