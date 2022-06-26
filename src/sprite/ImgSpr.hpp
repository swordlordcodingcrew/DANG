// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "SpriteObject.hpp"
#include "ImageObject.hpp"
#include "RectT.hpp"

#include <list>

namespace dang
{
    struct tmx_spriteobject;

    class Tweenable;
    using spTweenable = std::shared_ptr<Tweenable>;

    class ImgSpr : public SpriteObject, public ImageObject
    {
    public:
        ImgSpr();
        ImgSpr(const ImgSpr& s);
        ImgSpr(const tmx_spriteobject* so, const spImagesheet& is);
        virtual ~ImgSpr();

        void coreUpdate(uint32_t dt) override;
        void update(uint32_t dt) override {};
        void render(int32_t vpx, int32_t vpy) override;

        // tween stuff
        void addTween(spTweenable tw);
        void removeTween(const spTweenable& tw, bool suppressCB);
        void removeTweens(bool suppressCB);
        bool tweenActive(const spTweenable& tw);

        void updateTweens(uint32_t dt);


    private:
        std::list<spTweenable> _tweens;

    };

}

