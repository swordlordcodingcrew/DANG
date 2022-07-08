// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "ColSpr.hpp"
#include "ImageObject.hpp"
#include "MotionObject.hpp"
#include "TweenObject.hpp"
#include "DangFwdDecl.h"

namespace dang
{

    class FullColSpr : public ColSpr, public ImageObject, public MotionObject, public TweenObject
    {
    public:
        FullColSpr();
        FullColSpr(const FullColSpr& s);
        FullColSpr(const tmx_spriteobject* so, const spImagesheet& is);
        virtual ~FullColSpr();

        // inherited key funtions
        void coreUpdate(uint32_t dt) override;
        void update(uint32_t dt) override {};
        void render(int32_t vpx, int32_t vpy) override;

        void collide(const manifold &mf) override;

        // tree stuff. ts pointer will get moved
        void setNTreeState(spNTreeState ts);
        spNTreeState& getNTreeState();

    protected:
        // behaviour tree is intended for fully featured sprites
        spNTreeState      _nTreeState{nullptr};

    /**
    * visitor pattern of Tweenable due to the split architecture of sprites
    */
    protected:
        void visitInit(Tweenable& tw) override;
        void visitUpdate(Tweenable& tw, uint32_t dt) override;

    };

}

