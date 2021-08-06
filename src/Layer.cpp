// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 20.2.20.
//

#include "Layer.hpp"
#include "Sprite.hpp"

namespace dang
{

    Layer::Layer(E_TYPE type) : _type(type)
    {

    }

    Layer::E_TYPE Layer::getType()
    {
        return _type;
    }
}

