// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "src/layer/SprLayer.hpp"

#include <list>

namespace dang
{
    class ImgSpr;
    using spImgSpr = std::shared_ptr<ImgSpr>;
    class SprIterator;

    class ImgSprLayer : public SprLayer
    {
    public:
        ImgSprLayer();
        explicit ImgSprLayer(const tmx_layer* l);
        ~ImgSprLayer() override = default;

        spImgSpr            getImgSprByTypeNum(uint8_t type_num);
        virtual spImgSpr    getImgSprById(uint16_t id);
        virtual void        addSprite(spImgSpr s);

        // override from SprLayer
        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;


    protected:
        explicit ImgSprLayer(Layer::E_TYPE type) : SprLayer(type) {};
        ImgSprLayer(Layer::E_TYPE type, const tmx_layer* l) : SprLayer(type, l) {};

        virtual void coreUpdate(uint32_t dt, const Gear& gear);

    private:
        // may not be used in this layer type, use with param ImgSpr instead
        void        addSprite(spSprObj s) override {};

    };

}

