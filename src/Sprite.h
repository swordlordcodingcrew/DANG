// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#pragma once

#include "dang_globals.hpp"
#include <list>

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
        blit::Point getPos() { return _pos; }
        blit::Point getVel() { return _vel; }
        blit::Point getAcc() { return _acc; }
        void        setPos(const blit::Point& pos) { _pos = pos; }
        void        setVel(const blit::Point& vel) { _vel = vel; }
        void        setAcc(const blit::Point& acc) { _acc = acc; }

        uint8_t getTransform() { return _transform; }
        blit::Rect  getSizeRect();      // return size of sprite

        // to be moved to collisionsprite subclass on a later stage
        uint16_t wantToCollideWith(std::shared_ptr<Sprite> other);

    public: // variables
        bool                            _visible{true};
        uint16_t                        _img_index{0};  // index to the image of the imagesheet. (equals tmx_tile of tmx_spriteobject?)
        std::shared_ptr<Imagesheet>     _imagesheet{nullptr};
        uint8_t                         _transform{blit::SpriteTransform::NONE};      // transform for blitting
        int32_t                         _z_order{0};
        std::string                     _type{""};

    protected:  // variables
        //std::string name;     // to be implemented
        uint16_t        _id;    // global
        blit::Size      _size{0,0};

        blit::Point     _pos{0,0};
        blit::Point     _vel{0,0};
        blit::Point     _acc{0,0};

        blit::Rect      _hotrect{0,0,0,0};

        blit::Point           _last_pos{0,0};     // could be used for collision detection

        // tween depot
        std::list<std::shared_ptr<Tweenable>> _tweens;

        // less generic params. Might be subclassed at a later stage
        bool        _is_hit{false};

    };

}

