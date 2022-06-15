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
    class Tweenable;
    class TwAnim;
    using spTweenable = std::shared_ptr<Tweenable>;
    using spTwAnim = std::shared_ptr<TwAnim>;
    using spImagesheet = std::shared_ptr<Imagesheet>;

    class ImageObject
    {
    public:
        ImageObject() = default;
        ImageObject(const ImageObject& sp);
        ImageObject(const tmx_spriteobject* so, const spImagesheet& is);
        virtual ~ImageObject();

        // image
        void setImagesheet(spImagesheet is) { _imagesheet = is; }
        void setSize(SizeF& s) {_size = s; }
        void setSize(float w, float h) {_size.w = w; _size.h = h; }
        const Vector2F& getSize() const { return _size; }
        blit::Rect getBlitRect();
        blit::Rect getBlitRect(const uint16_t img_index); // override where we can ask for a specific image
        blit::Surface* getSurface() const;
        uint8_t     getTransform() const { return _transform; }
        void        setTransform(uint8_t transform) { _transform = transform; }

        bool        isVisible() const { return _visible; }
        void        setVisible(bool visible);


        // animation stuff (special tween)
        void setAnimation(spTweenable twa);
        spTweenable removeAnimation(bool suppressCB = true);
        spTweenable swapAnimation(spTweenable new_anim, bool suppressCB = true);
        void        updateAnimation(uint32_t dt);

    private:
        uint16_t        _img_index{0};  // index to the image of the imagesheet. (equals tmx_tile of tmx_spriteobject?)
        uint8_t         _transform{0};      // transform for blitting
        spImagesheet    _imagesheet{nullptr};
        bool            _visible{true};
        spTweenable     _animation;
        Vector2F        _size{0,0};

    };

}

