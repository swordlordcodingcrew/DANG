// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "src/Vector2T.hpp"

#include <memory>
#include <bitset>

namespace dang
{
    struct tmx_spriteobject;
    class SpriteObject;
    using spSpriteObject = std::shared_ptr<SpriteObject>;

    class SpriteObject
    {
    public:
        SpriteObject() = default;
        SpriteObject(const SpriteObject& spo) = default;
        virtual ~SpriteObject() = default;
        explicit SpriteObject(const tmx_spriteobject* so);

        // tree
        virtual void    addSpriteObject(spSpriteObject s);
        virtual void    markRemove() { _operable[2] = true; }
        virtual bool    isMarkedRemove() { return _operable[2]; }

        // position
        virtual void        setPos(const Vector2F& pos) { _pos = pos; }
        virtual void        setPosX(float x) {_pos.x = x; }
        virtual void        setPosY(float y) {_pos.y = y; }
        virtual const Vector2F&    getPos() const { return _pos; }
        virtual float       getPosX() const { return _pos.x; }
        virtual float       getPosY() const { return _pos.y; }
        virtual Vector2F    local2Global(const Vector2F& in);
        virtual Vector2F    global2Local(const Vector2F& in);

        virtual void coreUpdate(uint32_t dt) = 0;
        virtual void update(uint32_t dt) = 0;
        virtual void render(int32_t vpx, int32_t vpy) = 0;

        bool    isActive() { return _operable[0]; }
        void    setActive(bool active) {_operable[0] = active;}

        bool    inZone() { return _operable[1]; }

    private:                                  // params
        uint16_t        _id{0};                 // global id copied from Tiled
        uint8_t         _type_num{0};           // 0 == undefined. type of sprite (to be set acc. to type of tiled)
        uint8_t         _z_order{0};            // 0 == back, 255 == front
        Vector2F     	_pos{0,0};     // position


        // to save memory these params are not implemented
        //std::string   _type_name{""};
        //std::string   _name{""};

    private:
        friend class SprLayer;
        void    setInZone(bool in_zone) {_operable[1] = in_zone; }

    private:      // tree params
        friend class SprIterator;
        friend class SprLayer;
        friend class ColSprLayer;

        /**
         * [0] active true/false. Default true
         * [1] in zone true/false. Default true
         * [2] remove_from_tree true/false. Default false
         */
        std::bitset<3>  _operable{0b011};
        virtual void    removeMeFromTree();

        SpriteObject*     _parent{nullptr};
        spSpriteObject    _child{nullptr};    // left
        spSpriteObject    _next_sibling{nullptr};  // right
        SpriteObject*     _prev_sibling{nullptr};

    };

}

