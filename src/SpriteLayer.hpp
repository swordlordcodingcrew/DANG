// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <list>

#include "Layer.hpp"

namespace dang
{
    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;
    class SpriteIterator;

    class SpriteLayer : public Layer
    {
    public:
        SpriteLayer();
        explicit SpriteLayer(const tmx_layer* l);
        ~SpriteLayer() override = default;

        void    update(uint32_t dt, const Gear& gear);
        void    render(const Gear& gear);

        virtual spSprite    getSpriteByType(const std::string& name);
        virtual spSprite    getSpriteById(uint16_t id);
        virtual void        addSprite(spSprite s);
        virtual void        _removeSprite(spSprite s);
        virtual void        markRemoveSpritesByTypeNum(uint8_t type_num);
        virtual void        cleanSpritelist();

    protected:
        explicit SpriteLayer(Layer::E_TYPE type) : Layer(type) {};
        SpriteLayer(Layer::E_TYPE type, const tmx_layer* l) : Layer(type, l) {};

        virtual void coreUpdate(uint32_t dt, const Gear& gear);
        spSprite    _root{nullptr};

    public:         // tree
        SpriteIterator  begin();
        SpriteIterator  end();
        SpriteIterator  erase(SpriteIterator pos);

    };

}

