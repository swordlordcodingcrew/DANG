// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 20.2.20.
//

#pragma once

#include "dang_globals.hpp"
#include "RectT.h"

namespace dang
{

    class Sprite;
    class Gear;

    class Layer
    {
    public:
        enum E_TYPE {
            LT_UNDEFINED = 0,
            LT_TILELAYER,
            LT_SPRITELAYER,
            LT_COLLISIONSPRITELAYER
        };

        explicit Layer(E_TYPE type);
        Layer(E_TYPE type, const RectF& size);
        virtual ~Layer() = default;

        virtual void    update(uint32_t time, const Gear& gear) = 0;
        virtual void    render(const Gear& gear) = 0;


    protected:
        RectF           _size{0, 0, 1, 1};
        E_TYPE         _type{LT_UNDEFINED};

    public:     // public variables
        int32_t         _z_order{0};
        std::string     _name{""};
        bool            _visible{true};
    };

}



