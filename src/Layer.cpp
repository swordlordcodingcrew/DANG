// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 20.2.20.
//

#include "Layer.h"
#include "Sprite.h"

namespace dang
{

    Layer::Layer(E_TYPE type) : _type(type)
    {

    }

    Layer::Layer(E_TYPE type, const RectF& size) :
            _type(type), _size(size)
    {

    }

}

