// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "SpriteObject.hpp"
#include "CollisionObject.hpp"

namespace dang
{
    class ColSpr;
    using spColSpr = std::shared_ptr<ColSpr>;

    class ColSpr : public SpriteObject, public CollisionObject
    {
    public:
        ColSpr();
        ColSpr(const ColSpr& cs);
        explicit ColSpr(const tmx_spriteobject* so);
        ~ColSpr() override;

        // overrides from SpriteObject
        void    markRemove() override;
        void    addSpriteObject(spSpriteObject s) override;
        void    setPos(const Vector2F& pos) override;
        void    setPos(float x, float y) override;
        void    setPosX(float x) override;
        void    setPosY(float y) override;
        void coreUpdate(uint32_t dt) override {};
        void update(uint32_t dt) override {};
        void render(int32_t vpx, int32_t vpy) override;

        // override from CollisionObject
        void preSolve() override;
        void postSolve() override;
        void collide(const manifold &mf) override {};

        spColSpr getOther(const manifold& mf, const ColSpr* me);

        RectF       getHotrect() const;
        RectF       getHotrectL() const;
        RectF       getHotrectG() const;

    };

}

