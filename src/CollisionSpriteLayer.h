// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "dang_globals.hpp"
#include <Vector2T.h>
#include <RectT.h>
#include <forward_list>
#include <unordered_set>
#include "SpriteLayer.h"
//#include "CollisionSprite.h"

namespace dang
{
    class CollisionSprite;

    class CollisionSpriteLayer : public SpriteLayer
    {
    public:     // structs and enums

        /**
         * collision object
         */
        struct manifold
        {
            bool overlaps;      // true if sprites overlap even before moving. N.B. ti is in that case not valid
            float ti;           // between 0 and 1. How far along the delta to the goal did the collision occur

            std::shared_ptr<CollisionSprite> me;     // the sprite
            Vector2F deltaMe;     // relative movement of sprite me
            Vector2F normalMe;  // collision vector for sprite me; -1,0 or 1 in `x` and `y`
            Vector2F touchMe;   // touch position for sprite me

            std::shared_ptr<CollisionSprite> other;     // the other sprite
            Vector2F deltaOther;     // relative movement of sprite me
            Vector2F normalOther;   // collision vector for sprite other; -1,0 or 1 in `x` and `y`
            Vector2F touchOther;    // touch position for sprite other

        };

        /**
         * how a collision shall be resolved
         */
        enum eCollisionResponse
        {
            CR_NONE,
            CR_TOUCH,
            CR_CROSS,
            CR_SLIDE,
            CR_BOUNCE
        };

        /**
         * loosely based on box2d
         * COT_RIGID: should not move, Rigid object will not collide with oder rigid object. Example: Wall
         * COT_DYNAMIC: moving object. Hero, enemies
         */
        enum eCollObjectType{COT_RIGID, COT_DYNAMIC};


    public:
        CollisionSpriteLayer();
        explicit CollisionSpriteLayer(const RectF& layer_size_px);
        ~CollisionSpriteLayer() override = default;

        void    update(uint32_t time, const Gear& gear) override;
        void    render(const Gear& gear) override;

        void    addSprite(spCollisionSprite spr);
        void    removeSprite(spCollisionSprite spr);

    private:
        // hide these functions
        void    addSprite(spSprite spr) override {};
        void    removeSprite(spSprite spr) override {};


    protected:

        void handleCollisionDetection();

        void projectCollisions(const spCollisionSprite& me, const std::forward_list<spSprite>& sprites, std::forward_list<manifold>& mf_list);
        bool getRayIntersectionFraction(const Vector2F& origin, const Vector2F& direction, const RectF& aabb, float& ti, Vector2F& normal);
        float   getRayIntersectionFractionOfFirstRay(const Vector2F &originA, const Vector2F &endA, const Vector2F &originB, const Vector2F &endB);
        void    slide(manifold& mf, bool for_me);
        void    touch(manifold& mf, bool for_me);
        void    bounce(manifold& mf, bool for_me);


        std::unordered_set<spSprite> _handled;
        uint16_t                    _iteration{3};       // number of collision solving cycles
        bool                        _iterate{false};    // internal use
    };

}


