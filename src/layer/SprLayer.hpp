// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "src/layer/Layer.hpp"

namespace dang
{
    class SpriteObject;
    using spSprObj = std::shared_ptr<SpriteObject>;
    class SprIterator;

    class SprLayer : public Layer
    {
    public:
        SprLayer();
        explicit SprLayer(const tmx_layer* l);
        ~SprLayer() override = default;

        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

        virtual spSprObj    getSpriteByTypeNum(uint8_t type_num);
        virtual spSprObj    getSpriteById(uint16_t id);
        virtual void        addSprite(spSprObj s);
        virtual void        markRemoveSpritesByTypeNum(uint8_t type_num);
        virtual void        cleanSpritelist();
        void                resetZoneBit(const RectF& vp);


    protected:
        explicit SprLayer(Layer::E_TYPE type) : Layer(type) {};
        SprLayer(Layer::E_TYPE type, const tmx_layer* l) : Layer(type, l) {};

        virtual void coreUpdate(uint32_t dt, const Gear& gear);
        void            setRoot(spSprObj root);
        spSprObj&       getRoot();
        bool            isRoot(const spSprObj& s) { return s == _root; }

    private:
        virtual void _removeSpriteFromTree(spSprObj s);

        spSprObj    _root{nullptr};

    public:         // tree
        SprIterator  begin();
        SprIterator  end();
        SprIterator  erase(SprIterator pos);

    };

}

