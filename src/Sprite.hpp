// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "dang.hpp"
#include "Vector2T.hpp"
#include "RectT.hpp"
#include "bt/bt.hpp"
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

    class TreeState;
    class Imagesheet;
    class Tweenable;
    class TwAnim;
    using spTweenable = std::shared_ptr<Tweenable>;
    using spTwAnim = std::shared_ptr<TwAnim>;

    /**
         * definition of _type_num of sprites.
         */
    enum class SpriteType : int8_t
    {
        UNDEFINED = 0,

        // 1 - 9 king and associated stuff to king
        KING = 1,
        BUBBLE = 5,

        // 10 - 39 enemies
        ENEMIES = 10,
        PIG_NORMAL = 11,
        NORMAL_PIG_HIVE = 12,
        PIG_BETTER = 13,
        PIG_BOX = 14,
        PIG_BOMB = 15,
        PIG_CANNON = 16,
        CANNON = 17,
        FLYING_CANNONBALL = 18,
        FLYING_CRATE = 19,
        FLYING_BOMB = 20,
        EXPLOSION = 38,
        ENEMIES_END = 39,

        // 40 - 49 hotrects
        HOTRECT = 40,
        HOTRECT_PLATFORM = 41,

        // 50 - 69 coins and rewards in general
        REWARDS = 50,
        COIN_SILVER = 51,
        COIN_GOLD = 52,
        GEM_BLUE = 53,
        GEM_GREEN = 54,
        GEM_RED = 55,
        POTION_BLUE = 56,
        POTION_RED = 57,
        POTION_GREEN = 58,
        PIG_REWARD = 59,
        REWARDS_END = 69,

        // 70 - 79 triggers
        TRIGGERS = 70,
        ROOM_TRIGGER = 71,
        WARP_ROOM_TRIGGER = 72,
        TRIGGERS_END = 79,

        // 80 - 89 Mood stuff
        PIG_POOF = 81,
        CANNON_MUZZLE = 82,
    };

    class Sprite : public std::enable_shared_from_this<Sprite>
    {
    public: // functions
        Sprite();
        Sprite(const Sprite& sp);
        Sprite(const tmx_spriteobject* so, std::shared_ptr<Imagesheet> is);
        virtual ~Sprite();

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

        // simple image
        void setImagesheet(std::shared_ptr<Imagesheet> is) { _imagesheet = is; }
        void setSize(SizeF& s) {_size = s; }
        void setSize(float w, float h) {_size.w = w; _size.h = h; }
        blit::Rect getBlitRect();

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

        // Behaviour Tree functions
        static BTNodeStatus BTLoiter(std::shared_ptr<Sprite> s);
        static BTNodeStatus BTIsHeroAround(std::shared_ptr<Sprite> s);
        static BTNodeStatus BTSleep1Sec(std::shared_ptr<Sprite> s);

        // ts pointer will get moved
        void setTreeState(std::shared_ptr<TreeState> ts);
        std::shared_ptr<TreeState>& getTreeState();
        void setNTreeState(std::shared_ptr<NTreeState> ts);
        std::shared_ptr<NTreeState>& getNTreeState();


        // TEST
        bool is_hungry{true};
        void eat_food() {};

        bool has_food() const
        {
            return _has_food;
        }

        bool _has_food{true};
        // ENDTEST

        Vector2F    getSize() { return _size; }

        uint8_t     getTransform() const { return _transform; }
        RectF       getSizeRect();      // return size of sprite

    public: // variables
        bool                            _visible{true};
        uint16_t                        _img_index{0};  // index to the image of the imagesheet. (equals tmx_tile of tmx_spriteobject?)
        std::shared_ptr<Imagesheet>     _imagesheet{nullptr};
        uint8_t                         _transform{0};      // transform for blitting
        int32_t                         _z_order{0};
        uint16_t                        _id{0};    // global
        std::string                     _type_name{""};
        SpriteType                      _type_num{SpriteType::UNDEFINED};

    protected:  // variables
        std::shared_ptr<TreeState>      _btTreeState{nullptr};
        std::shared_ptr<NTreeState>      _nTreeState{nullptr};

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

