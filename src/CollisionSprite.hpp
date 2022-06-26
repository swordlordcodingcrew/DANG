// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "DangFwdDecl.h"
#include "Sprite.hpp"
#include "CollisionObject.hpp"
#include "Vector2T.hpp"
#include "RectT.hpp"
//#include "CollisionSpriteLayer.hpp"
#include "dang.hpp"

#include <list>

namespace dang
{
    class CollisionSprite : public Sprite, public CollisionObject
    {
    public: // functions
        using spCollisionSprite = std::shared_ptr<CollisionSprite>;
        CollisionSprite();
        CollisionSprite(const CollisionSprite& cs);
        CollisionSprite(const tmx_spriteobject* so, const spImagesheet& is);
        ~CollisionSprite() override;

        // overrides from sprite
        void    markRemove() override;
        void    update(uint32_t dt) override;
        void    addSprite(spSprite s) override;
        void    setPos(const Vector2F& pos) override;
        void    setPosX(float x) override;
        void    setPosY(float y) override;

        // override from CollisionObject
        void preSolve() override;
        void postSolve() override;
        void collide(const manifold &mf) override;

        spCollisionSprite getOther(const manifold& mf, const CollisionSprite* me);

        RectF       getHotrect() const;
        RectF       getHotrectL() const;
        RectF       getHotrectG() const;

        // ts pointer will get moved
        void setNTreeState(spNTreeState ts);
        spNTreeState& getNTreeState();


    protected:  // variables
        // behaviour tree is implemented in the collision layer
        spNTreeState      _nTreeState{nullptr};
    };

}

