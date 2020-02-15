// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 31.1.20.
//

#pragma once

#include "dang_globals.hpp"
#include <list>

namespace dang
{
    class Sprite
    {
    public: // functions
        Sprite();
        Sprite(const Sprite& sp);
        Sprite(const spriteobject& so);
        virtual ~Sprite();
        uint16_t wantToCollideWith(std::shared_ptr<sprite> other);
        void addTween(std::shared_ptr<Tweenable> tw);
        void updateTweens(uint32_t time);
        void removeTween(std::shared_ptr<Tweenable> tw, bool suppressCB);

    public: // variables
        bool        _visible;
        uint16_t    _img_index{0};  // index to the image of the imagesheet. (equals tile of spriteobject?)
//        uint16_t    _tile{0};
//        uint16_t    _tileset{0};  // is this needed?


    protected:  // variables
        //std::string name; // not needed, the reference contains the name
        uint16_t    _id; // global
        std::string _type;
        point       _pos{0,0};
        Size        _size{0,0};

        // tween depot
        std::list<std::shared_ptr<Tweenable>> _tweens;

        // less generic params. Might be subclassed at a later stage
        point       _vel{0,0};
        bool        _isHit{false};

    };

}

