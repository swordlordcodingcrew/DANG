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
        ~SpriteLayer() override = default;

        void    update(uint32_t dt, const Gear& gear);
        void    render(const Gear& gear);

        virtual spSprite    getSpriteByType(const std::string& name);

        virtual void    addSprite(spSprite s);
        virtual void    removeSprite(spSprite s);
        virtual void    removeSpriteById(uint16_t id);
        virtual spSprite    getSpriteById(uint16_t id);
        virtual void    removeSpritesByTypeNum(uint8_t type_num);
//        void            sortSprites();


    protected:
        explicit SpriteLayer(Layer::E_TYPE type) : Layer(type) {};

        virtual void coreUpdate(uint32_t dt, const Gear& gear);
        // sprites inside active area
//        std::list<spSprite> _active_sprites;
        spSprite    _root{nullptr};

        // inactive sprites
        std::list<spSprite> _inactive_sprites;

    public:         // tree
        SpriteIterator  begin();
        SpriteIterator  end();
        SpriteIterator  erase(SpriteIterator pos);

    };

}

