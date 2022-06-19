// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 20.2.20.
//

#include "Layer.hpp"
#include "TmxExtruder.hpp"

namespace dang
{

    Layer::Layer(E_TYPE type) : _type(type)
    {

    }

    Layer::E_TYPE Layer::getType()
    {
        return _type;
    }

    Layer::Layer(E_TYPE type, const tmx_layer *l)
    {
        _tmx_layer = l;
        _name = l->name;
        _z_order = l->z_order;
        _visible = l->visible;
        _position = l->position;
        _active = true;
    }

    Layer::Layer(Layer::E_TYPE type, const PointF &position, uint8_t z_order, const std::string &name, bool visible,
                 bool active)
                 : _type(type), _position(position), _z_order(z_order), _name(name), _visible(visible), _active(active)
    {

    }

    void Layer::init(const tmx_layer* l)
    {
        _tmx_layer = l;
        _name = l->name;
        _z_order = l->z_order;
        _visible = l->visible;
        _position = l->position;
        _active = true;

    }

}

