// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "ImgSpr.hpp"
#include "MotionObject.hpp"
#include "TweenObject.hpp"
#include "DangFwdDecl.h"

namespace dang
{

    class FullImgSpr : public ImgSpr, public MotionObject, public TweenObject
    {
    public:
        FullImgSpr();
        FullImgSpr(const FullImgSpr& s);
        FullImgSpr(const tmx_spriteobject* so, const spImagesheet& is);
        ~FullImgSpr() override = default;

        // override from ImgSpr
        void coreUpdate(uint32_t dt) override;

        /**
        * visitor pattern of Tweenable due to the split architecture of sprites
        */
    protected:
        void visitInit(Tweenable& tw) override;
        void visitUpdate(Tweenable& tw, uint32_t dt) override;

    };
}
