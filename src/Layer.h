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
        Layer(int32_t type);
        Layer(int32_t type, const RectF& size);
        virtual ~Layer();

        virtual void    update(uint32_t time, const Gear& gear) = 0;
        virtual void    render(const Gear& gear) = 0;

        enum TYPE {
            LT_UNDEFINED = 0,
            LT_TILELAYER,
            LT_SPRITELAYER
        };

    protected:
        RectF           _size{0, 0, 1, 1};
        int32_t         _type{LT_UNDEFINED};

    public:     // public variables
        int32_t         _z_order{0};
        std::string     _name{""};
        bool            _visible{true};
    };

}



