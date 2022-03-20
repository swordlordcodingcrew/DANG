// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "dang.hpp"
#include "Vector2T.hpp"
#include "RectT.hpp"
#include "bt/NTreeState.h"

#include <list>
#include <memory>


// forward declaration
namespace blit
{
    struct Rect;
}

namespace dang
{
    struct tmx_spriteobject;

    class SpriteIterator;
    class Imagesheet;
    class Tweenable;
    class TwAnim;
    using spTweenable = std::shared_ptr<Tweenable>;
    using spTwAnim = std::shared_ptr<TwAnim>;


    class Sprite : public std::enable_shared_from_this<Sprite>
    {
    public: // functions
        Sprite();
        Sprite(const Sprite& sp);
        Sprite(const tmx_spriteobject* so, const spImagesheet& is);
        virtual ~Sprite();

        // tree
        virtual void    addSprite(spSprite s);
        virtual void    removeMeFromTree();
        virtual void    markRemove() { _remove_from_layer = true; }

        // tween stuff
        void addTween(spTweenable tw);
        void removeTween(const spTweenable& tw, bool suppressCB);
        void removeTweens(bool suppressCB);
        bool tweenActive(const spTweenable& tw);

        // animation stuff (special tween)
        void setAnimation(spTweenable twa);
        spTweenable removeAnimation(bool suppressCB = true);
        spTweenable swapAnimation(spTweenable new_anim, bool suppressCB = true);

        void updateTweens(uint32_t dt);
        void coreUpdate(uint32_t dt);

        virtual void update(uint32_t dt);
        virtual void render(int32_t vpx, int32_t vpy);

        // image
        void setImagesheet(spImagesheet is) { _imagesheet = is; }
        void setSize(SizeF& s) {_size = s; }
        void setSize(float w, float h) {_size.w = w; _size.h = h; }
        blit::Rect getBlitRect();
        blit::Rect getBlitRect(const uint16_t img_index); // override where we can ask for a specific image

        // pos, vel, acc, gravity
        Vector2F    getPos() { return _pos; }
        Vector2F    getLastPos() { return _last_pos; }
        Vector2F    getPosDelta() { return _pos - _last_pos; }
        Vector2F    getVel() { return _vel; }
        Vector2F    getAcc() { return _acc; }
        Vector2F    getGravity() { return _gravity; }
        virtual void        setPos(const Vector2F& pos) { _pos = pos; }
        void        setVel(const Vector2F& vel) { _vel = vel; }
        void        setAcc(const Vector2F& acc) { _acc = acc; }
        void        setGravity(const Vector2F& g) {_gravity = g; };

        virtual void        setPosX(float x) {_pos.x = x; }
        virtual void        setPosY(float y) {_pos.y = y; }
        void        setVelX(float x) {_vel.x = x; }
        void        setVelY(float y) {_vel.y = y; }
        void        setVelTowardsPoint(const Vector2F& target);
        void        setVelTowardsPoint(const Vector2F& target, const float speedx, const float speedy);
        void        setVelRelToRadAngle(const double angle);
        void        setVelRelToRadAngle(const double angle, const float speedx, const float speedy);
        void        setAccX(float x) {_acc.x = x; }
        void        setAccY(float y) {_acc.y = y; }

        Vector2F    getPosG();
        Vector2F    getLastPosG();
        float       getPosX() const { return _pos.x; }
        float       getPosY() const { return _pos.y; }
        float       getLastPosX() const { return _last_pos.x; }
        float       getLastPosY() const { return _last_pos.y; }

        float        getPosGX() const { return _pos_g.x; }
        float        getPosGY() const { return _pos_g.y; }
        float        getLastPosGX() const { return _last_pos_g.x; }
        float        getLastPosGY() const { return _last_pos_g.y; }

        Vector2F    getSize() { return _size; }

        uint8_t     getTransform() const { return _transform; }
        RectF       getSizeRect();      // return size of sprite
        RectF       getSizeRectG();      // return size of sprite in global coords

    public: // variables
        bool           _visible{true};
        bool           _active{true};
        uint16_t       _img_index{0};  // index to the image of the imagesheet. (equals tmx_tile of tmx_spriteobject?)
        spImagesheet   _imagesheet{nullptr};
        uint8_t        _transform{0};      // transform for blitting
        int32_t        _z_order{0};
        uint16_t       _id{0};    // global
        std::string    _type_name{""};
        uint8_t        _type_num{0}; // 0 == undefined


    protected:  // variables

        bool          _remove_from_layer{false};  // if set to true, this sprite will be removed from the layer

        //std::string name;     // to be implemented
        Vector2F      _size{0,0};

        Vector2F     _pos{0,0};
        Vector2F     _vel{0,0};
        Vector2F     _acc{0,0};
        Vector2F     _gravity{0,0};

        Vector2F    _pos_g{0,0};  // global position
        Vector2F    _last_pos_g{0,0};  // global position


        Vector2F    _last_pos{0,0};     // used e.g. for collision detection
//        uint32_t    _last_update_time{0};

        // tween depot
        std::list<spTweenable> _tweens;
        spTweenable _animation;

    protected:      // tree
        friend class SpriteIterator;
        friend class SpriteLayer;
        friend class CollisionSpriteLayer;
        std::weak_ptr<Sprite>   _parent;
        spSprite                _child{nullptr};    // left
        spSprite                _next_sibling{nullptr};  // right
        std::weak_ptr<Sprite>   _prev_sibling;

    };

}

