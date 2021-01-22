// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "dang.hpp"
#include "Vector2T.hpp"
#include "RectT.hpp"

#include <list>
#include <memory>

namespace dang
{
    struct tmx_spriteobject;

    class Imagesheet;
    class Tweenable;
    class TwAnim;
    using spTweenable = std::shared_ptr<Tweenable>;

    class Sprite : public std::enable_shared_from_this<Sprite>
    {
    public: // functions
        Sprite();
//        Sprite(const Sprite& sp);
        Sprite(const tmx_spriteobject& so, std::shared_ptr<Imagesheet> is);
        virtual ~Sprite();

        // tween stuff
        void addTween(std::shared_ptr<Tweenable> tw);
        void removeTween(std::shared_ptr<Tweenable> tw, bool suppressCB);
        void removeTweens(bool suppressCB);
        bool tweenActive(const std::shared_ptr<Tweenable>& tw);

        // animation stuff (special tween)
        void setAnimation(std::shared_ptr<Tweenable> twa);
        void removeAnimation(bool suppressCB = true);

        void updateTweens(uint32_t dt);
        void coreUpdate(uint32_t dt);
        virtual void update(uint32_t dt);

        // simple image
        void setImagesheet(std::shared_ptr<Imagesheet> is) { _imagesheet = is; }
        void setSize(SizeF& s) {_size = s; }
        void setSize(float w, float h) {_size.w = w; _size.h = h; }

        // pos, vel, acc, gravity
        Vector2F    getPos() { return _pos; }
        Vector2F    getLastPos() { return _last_pos; }
        Vector2F    getPosDelta() { return _pos - _last_pos; }
        Vector2F    getVel() { return _vel; }
        Vector2F    getAcc() { return _acc; }
        Vector2F    getGravity() { return _gravity; }
        void        setPos(const Vector2F& pos) { _pos = pos; }
        void        setVel(const Vector2F& vel) { _vel = vel; }
        void        setAcc(const Vector2F& acc) { _acc = acc; }
        void        setGravity(const Vector2F& g) {_gravity = g; };

        void        setPosX(float x) {_pos.x = x; }
        void        setPosY(float y) {_pos.y = y; }
        void        setVelX(float x) {_vel.x = x; }
        void        setVelY(float y) {_vel.y = y; }
        void        setAccX(float x) {_acc.x = x; }
        void        setAccY(float y) {_acc.y = y; }

        float        getPosX() const { return _pos.x; }
        float        getPosY() const { return _pos.y; }
        float        getLastPosX() const { return _last_pos.x; }
        float        getLastPosY() const { return _last_pos.y; }

        Vector2F    getSize() { return _size; }

        uint8_t     getTransform() const { return _transform; }
        RectF       getSizeRect();      // return size of sprite


    public: // variables
        bool                            _visible{true};
        uint16_t                        _img_index{0};  // index to the image of the imagesheet. (equals tmx_tile of tmx_spriteobject?)
        std::shared_ptr<Imagesheet>     _imagesheet{nullptr};
        uint8_t                         _transform{0};      // transform for blitting
//        uint8_t                         _transform{blit::SpriteTransform::NONE};      // transform for blitting
        int32_t                         _z_order{0};
        std::string                     _type{""};
        uint16_t                        _id{0};    // global
        int32_t                         _type_num{0};

    protected:  // variables

        //std::string name;     // to be implemented
        Vector2F      _size{0,0};

        Vector2F     _pos{0,0};
        Vector2F     _vel{0,0};
        Vector2F     _acc{0,0};
        Vector2F     _gravity{0,0};

        Vector2F    _last_pos{0,0};     // used e.g. for collision detection
//        uint32_t    _last_update_time{0};

        // tween depot
        std::list<spTweenable> _tweens;
        spTweenable _animation;

    };

}

