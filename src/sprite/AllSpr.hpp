// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "ColSpr.hpp"
#include "ImageObject.hpp"
#include "MotionObject.hpp"

namespace dang
{
    struct tmx_spriteobject;

    class Tweenable;
    using spTweenable = std::shared_ptr<Tweenable>;

    class AllSpr : public ColSpr, public ImageObject, public MotionObject
    {
    public:
        AllSpr();
        AllSpr(const AllSpr& s);
        AllSpr(const tmx_spriteobject* so, const spImagesheet& is);
        virtual ~AllSpr();

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

