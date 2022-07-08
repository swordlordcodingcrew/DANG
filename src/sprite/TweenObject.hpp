// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "DangFwdDecl.h"

#include <list>

namespace dang
{
    /**
     * implementation of the visitor pattern for the tweens
     */
    class TweenObject
    {
    public:
        TweenObject() = default;
        TweenObject(const TweenObject& s);
        virtual ~TweenObject() = default;

        // tween stuff
        void addTween(spTweenable tw);
        void removeTween(const spTweenable& tw, bool suppressCB);
        void removeTweens(bool suppressCB);
        bool tweenActive(const spTweenable& tw);
        void updateTweens(uint32_t dt);
        std::list<spTweenable>& getTweenList() { return _tweens; }

        // animation stuff (special tween)
        void setAnimation(spTweenable twa);
        spTweenable getAnimation() { return _animation; }
        spTweenable removeAnimation(bool suppressCB = true);
        spTweenable swapAnimation(spTweenable new_anim, bool suppressCB = true);
        void        updateAnimation(uint32_t dt);



    protected:
        virtual void visitInit(Tweenable& tw) = 0;
        virtual void visitUpdate(Tweenable& tw, uint32_t dt) = 0;


    private:
        spTweenable             _animation;
        std::list<spTweenable>  _tweens;


    };

}

