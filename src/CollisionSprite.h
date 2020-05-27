// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Sprite.h"
#include <list>
#include <Vector2T.h>
#include <RectT.h>
#include "dang_globals.hpp"
#include "CollisionSpriteLayer.h"

namespace dang
{
    struct tmx_spriteobject;
    class Imagesheet;
    class CollisionSpriteLayer;

    class CollisionSprite : public Sprite
    {
    public: // functions
        CollisionSprite();
        CollisionSprite(const tmx_spriteobject &so, std::shared_ptr<Imagesheet> is);
        ~CollisionSprite() override;

        RectF       getHotrect() const { return _hotrect; };
        RectF       getHotrectAbs() const;
        virtual void collide(const CollisionSpriteLayer::manifold &mf);
        virtual CollisionSpriteLayer::eCollisionResponse    getCollisionResponse(spSprite other);
        void                                        setCollisionResponse(CollisionSpriteLayer::eCollisionResponse response) { _coll_response = response; };

        virtual CollisionSpriteLayer::eCollObjectType       getCOType() const { return _coll_object_type; }
        virtual void                                        setCOType(CollisionSpriteLayer::eCollObjectType type) { _coll_object_type = type; }

    protected:  // variables
        CollisionSpriteLayer::eCollObjectType       _coll_object_type{CollisionSpriteLayer::COT_DYNAMIC};
        RectF                                       _hotrect{0,0,0,0};
        CollisionSpriteLayer::eCollisionResponse    _coll_response{CollisionSpriteLayer::CR_SLIDE};
    };

}

