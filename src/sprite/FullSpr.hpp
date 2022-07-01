// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "ColSpr.hpp"
#include "ImageObject.hpp"
#include "MotionObject.hpp"
#include "DangFwdDecl.h"

namespace dang
{

    class FullSpr : public ColSpr, public ImageObject, public MotionObject
    {
    public:
        FullSpr();
        FullSpr(const FullSpr& s);
        FullSpr(const tmx_spriteobject* so, const spImagesheet& is);
        virtual ~FullSpr();

        // inherited key funtions
        void coreUpdate(uint32_t dt) override;
        void update(uint32_t dt) override {};
        void render(int32_t vpx, int32_t vpy) override;

        void collide(const manifold &mf) override;


        // tween stuff
        void addTween(spTweenable tw);
        void removeTween(const spTweenable& tw, bool suppressCB);
        void removeTweens(bool suppressCB);
        bool tweenActive(const spTweenable& tw);

        void updateTweens(uint32_t dt);

        // tree stuff. ts pointer will get moved
        void setNTreeState(spNTreeState ts);
        spNTreeState& getNTreeState();

    protected:
        // behaviour tree is intended for fully featured sprites
        spNTreeState      _nTreeState{nullptr};

    private:
        std::list<spTweenable> _tweens;

    };

}

