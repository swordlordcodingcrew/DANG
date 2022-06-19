// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Layer.hpp"

#include <list>

namespace dang
{
    class ImgSpr;
    using spImgSpr = std::shared_ptr<ImgSpr>;
    class SprIterator;

    class ImgSprLayer : public Layer
    {
    public:
        ImgSprLayer();
        explicit ImgSprLayer(const tmx_layer* l);
        ~ImgSprLayer() override = default;

        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

        virtual spImgSpr    getSpriteByTypeNum(uint8_t type_num);
        virtual spImgSpr    getSpriteById(uint16_t id);
        virtual void        addSprite(spImgSpr s);
        virtual void        markRemoveSpritesByTypeNum(uint8_t type_num);
        virtual void        cleanSpritelist();

    private:
        explicit ImgSprLayer(Layer::E_TYPE type) : Layer(type) {};

        virtual void coreUpdate(uint32_t dt, const Gear& gear);

        virtual void        _removeSprite(spImgSpr s);

        spImgSpr    _root{nullptr};

    public:         // tree
        SprIterator  begin();
        SprIterator  end();
        SprIterator  erase(SprIterator pos);

    };

}

