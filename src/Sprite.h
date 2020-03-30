// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#pragma once

#include "dang_globals.hpp"
#include <list>
#include <geom/Vector2T.h>
#include <geom/RectT.h>

namespace dang
{
    struct tmx_spriteobject;

    class Imagesheet;
    class Tweenable;

    class Sprite
    {
    public: // functions
        Sprite();
//        Sprite(const Sprite& sp);
        Sprite(const tmx_spriteobject& so, std::shared_ptr<Imagesheet> is);
        virtual ~Sprite();
        void addTween(std::shared_ptr<Tweenable> tw);
        void updateTweens(uint32_t time);
        void removeTween(std::shared_ptr<Tweenable> tw, bool suppressCB);

        void coreUpdate(uint32_t time);
        virtual void update(uint32_t time);

        // pos, vel, acc
        Vector2f    getPos() { return _pos; }
        Vector2f    getLastPos() { return _last_pos; }
        Vector2f    getVel() { return _vel; }
        Vector2f    getAcc() { return _acc; }
        void        setPos(const Vector2f& pos) { _pos = pos; }
        void        setVel(const Vector2f& vel) { _vel = vel; }
        void        setAcc(const Vector2f& acc) { _acc = acc; }

        Vector2f    getSize() { return _size; }

        uint8_t     getTransform() { return _transform; }
        Rectf       getSizeRect();      // return size of sprite
        blit::Rect  getSizeRecti();


    public: // variables
        bool                            _visible{true};
        uint16_t                        _img_index{0};  // index to the image of the imagesheet. (equals tmx_tile of tmx_spriteobject?)
        std::shared_ptr<Imagesheet>     _imagesheet{nullptr};
        uint8_t                         _transform{blit::SpriteTransform::NONE};      // transform for blitting
        int32_t                         _z_order{0};
        std::string                     _type{""};
        uint16_t                        _id{0};    // global

    protected:  // variables
        //std::string name;     // to be implemented
        Vector2f      _size{0,0};

        Vector2f     _pos{0,0};
        Vector2f     _vel{0,0};
        Vector2f     _acc{0,0};

        Vector2f     _last_pos{0,0};     // could be used for collision detection

        // tween depot
        std::list<spTweenable> _tweens;

        // *** collision stuff ***
        // to be moved to collisionsprite subclass on a later stage
    public:
        uint16_t    wantToCollideWith(std::shared_ptr<Sprite> other);
        bool        _is_hit{false};
        Rectf       _hotrect{0,0,0,0};

    };

}

