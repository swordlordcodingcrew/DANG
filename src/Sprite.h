// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#pragma once

#include "dang_globals.hpp"
#include "SweptAABBCollision.h"
#include <list>
#include <Vector2T.h>
#include <RectT.h>

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
        Vector2F    getPos() { return _pos; }
        Vector2F    getLastPos() { return _last_pos; }
        Vector2F    getPosDelta() { return _pos - _last_pos; }
        Vector2F    getVel() { return _vel; }
        Vector2F    getAcc() { return _acc; }
        void        setPos(const Vector2F& pos) { _pos = pos; }
        void        setVel(const Vector2F& vel) { _vel = vel; }
        void        setAcc(const Vector2F& acc) { _acc = acc; }

        Vector2F    getSize() { return _size; }

        uint8_t     getTransform() { return _transform; }
        RectF       getSizeRect();      // return size of sprite


    public: // variables
        bool                            _visible{true};
        uint16_t                        _img_index{0};  // index to the image of the imagesheet. (equals tmx_tile of tmx_spriteobject?)
        std::shared_ptr<Imagesheet>     _imagesheet{nullptr};
        uint8_t                         _transform{blit::SpriteTransform::NONE};      // transform for blitting
        int32_t                         _z_order{0};
        std::string                     _type{""};
        uint16_t                        _id{0};    // global

    protected:  // variables
        friend class SweptAABBCollision;

        //std::string name;     // to be implemented
        Vector2F      _size{0,0};

        Vector2F     _pos{0,0};
        Vector2F     _vel{0,0};
        Vector2F     _acc{0,0};

        Vector2F     _last_pos{0,0};     // could be used for collision detection
        uint32_t    _last_update_time{0};

        // tween depot
        std::list<spTweenable> _tweens;

        // *** collision stuff ***
        // to be moved to collisionsprite subclass on a later stage
    public:
        uint16_t    _coll_response{SweptAABBCollision::CR_BOUNCE};
        uint16_t    _coll_object_type{SweptAABBCollision::COT_DYNAMIC};
        bool        _is_hit{false};
        RectF       _hotrect{0,0,0,0};
        RectF       getHotrect() { return _hotrect; };
        RectF       getHotrectAbs();
        virtual void collide(const manifold &mf);

    };

}

