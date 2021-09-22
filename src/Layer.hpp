// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 20.2.20.
//

#pragma once

#include "RectT.hpp"
#include <memory>
#include <string>

namespace dang
{
    struct tmx_layer;
    class Sprite;
    class Gear;

    class Layer
    {
    public:
        enum E_TYPE {
            LT_UNDEFINED = 0,
            LT_TILELAYER,
            LT_SPRITELAYER,
            LT_COLLISIONSPRITELAYER,
            LT_HUDLAYER
        };

        explicit Layer(E_TYPE type);
        virtual ~Layer() = default;

        virtual void    update(uint32_t dt, const Gear& gear) = 0;
        virtual void    render(const Gear& gear) = 0;

        E_TYPE getType();

        bool visible() const { return _visible; }
        bool active() const { return _active; }

        void setVisibility(bool visible) { _visible = visible; }
        void setActive(bool active) { _active = active; }

    protected:
        E_TYPE  _type{LT_UNDEFINED};
//        RectF           _size{0, 0, 1, 1};

    public:     // public variables
        PointF              _position{0,0};
        int32_t             _z_order{0};
        std::string         _name{""};
        bool                _visible{true};
        bool                _active{true};
        const tmx_layer*    _tmx_layer{nullptr};
    };

}



