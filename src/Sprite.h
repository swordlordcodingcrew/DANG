// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 31.1.20.
//

#pragma once

#include "dang_globals.hpp"

namespace dang
{
    class Sprite
    {
    public:
        Sprite();
        virtual ~Sprite();

        // public variables
        uint16_t _id; // global
        //std::string name; // not needed, the reference contains the name
        std::string _type;

        point _pos{0,0};
        int32_t width;
        int32_t height;
        bool visible;
        uint16_t tileset;
        uint16_t tile;


        uint16_t    _img_index{0};

    };

}

