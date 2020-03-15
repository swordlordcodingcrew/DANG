// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 31.1.20.
//

#pragma once

#include "dang_globals.hpp"
#include <list>

namespace dang
{
    class Sprite
    {
    public: // functions
        Sprite();
//        Sprite(const Sprite& sp);
        Sprite(const spriteobject& so, std::shared_ptr<Imagesheet> is);
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
        uint16_t wantToCollideWith(std::shared_ptr<sprite> other);

    public: // variables
        bool                            _visible{true};
        uint16_t                        _img_index{0};  // index to the image of the imagesheet. (equals tile of spriteobject?)
        std::shared_ptr<Imagesheet>     _imagesheet{nullptr};
        uint8_t                         _transform{blit::SpriteTransform::NONE};      // transform for blitting
        int32_t                         _z_order{0};
        std::string                     _type{""};

    protected:  // variables
        //std::string name; // not needed, the reference contains the name
        uint16_t        _id; // global
        blit::Size      _size{0,0};


        blit::Point     _pos{0,0};
        blit::Point     _vel{0,0};
        blit::Point     _acc{0,0};

        blit::Rect      _hotrect{0,0,0,0};

        point           _last_pos{0,0};     // could be used for collision detection

        // tween depot
        std::list<std::shared_ptr<Tweenable>> _tweens;

        // less generic params. Might be subclassed at a later stage
        bool        _is_hit{false};

    };

}

