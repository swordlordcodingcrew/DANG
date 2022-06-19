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
        Layer(E_TYPE type, const tmx_layer* l);
        Layer(E_TYPE type, const PointF& position, uint8_t z_order, const std::string& name, bool visible, bool active);
        virtual ~Layer() = default;

        void init(const tmx_layer* l);

        virtual void    update(uint32_t dt, const Gear& gear) = 0;
        virtual void    render(const Gear& gear) = 0;

        E_TYPE getType();


        void setVisible(bool visible) { _visible = visible; }
        void setActive(bool active) { _active = active; }

        const tmx_layer*    tmxLayer() const { return _tmx_layer; }
        uint8_t             zOrder() const { return _z_order; }
        const std::string&  name() const { return _name; }
        bool                visible() const { return _visible; }
        bool                active() const { return _active; }

    private:
        E_TYPE              _type{LT_UNDEFINED};
        PointF              _position{0,0};
        uint8_t             _z_order{0};
        std::string         _name{""};
        bool                _visible{true};
        bool                _active{true};
        const tmx_layer*    _tmx_layer{nullptr};
    };

}



