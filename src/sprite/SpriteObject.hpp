// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Vector2T.hpp"
#include "RectT.hpp"

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
        virtual void        setPos(float x, float y) { _pos.x = x; _pos.y = y; }
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

        bool    isActive() const { return _operable[0]; }
        void    setActive(bool active) {_operable[0] = active;}

        bool        inZone() const { return _operable[1]; }
        void        setInZone(bool in_zone) {_operable[1] = in_zone; }

        uint8_t     typeNum() const { return _type_num; }
        void        setTypeNum(uint8_t typenum) { _type_num = typenum; }

        uint16_t    id() const { return _id; }

        void            setSize(SizeF& s) {_size = s; }
        void            setSize(float w, float h) {_size.w = w; _size.h = h; }
        const Vector2F& getSize() const { return _size; }
        RectF           getSizeRect();      // return size of sprite
        RectF           getSizeRectG();      // return size of sprite in global coords

        void            setZOrder(uint8_t z_order) { _z_order = z_order; }
        uint8_t         zOrder() const { return _z_order; }

    private:                                     // params
        uint16_t        _id{0};                  // global id copied from Tiled
        uint8_t         _type_num{0};            // 0 == undefined. type of sprite (to be set acc. to type of tiled)
        uint8_t         _z_order{0};             // 0 == back, 255 == front
        Vector2F     	_pos{0,0};      // position
        Vector2F        _size{0,0};     // size


        // to save memory these params are not implemented
        //std::string   _type_name{""};
        //std::string   _name{""};

        /**
         * [0] active true/false. Default true
         * [1] in zone true/false. Default true
         * [2] remove_from_tree true/false. Default false
         */
        std::bitset<3>  _operable{0b011};

    private:
        friend class SprLayer;
        void    removeMeFromTree();

    private:      // tree params
        friend class SprIterator;

        SpriteObject*     _parent{nullptr};
        spSpriteObject    _child{nullptr};    // left
        spSpriteObject    _next_sibling{nullptr};  // right
        SpriteObject*     _prev_sibling{nullptr};

    };

}

