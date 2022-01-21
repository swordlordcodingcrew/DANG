// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

/**
 * The collision detection algorithm was inspired largely by the following sources
 * ===========================================================================
 * - https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282#565282
 *   post of Gareth
 * - https://gist.github.com/hamaluik/e69f96e253a190273bf0
 *   AABB.hx of Kenton Hamaluik
 * - https://github.com/kikito/bump.lua
 *   bump.lua, copyright (c) 2014 Enrique García Cota
 * - https://box2d.org/
 *   (c) by Erin Catto
*/

#pragma once

#include <forward_list>
#include <unordered_set>

#include "Vector2T.hpp"
#include "RectT.hpp"
#include "SpriteLayer.hpp"

namespace dang
{
    class CollisionSprite;
    using spCollisionSprite = std::shared_ptr<CollisionSprite>;

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
         * COT_RIGID: should not move, Rigid object will not collide with other rigid object. Example: Wall
         * COT_DYNAMIC: moving object. Hero, enemies
         */
        enum eCollObjectType{COT_RIGID, COT_DYNAMIC};


    public:
        CollisionSpriteLayer();
        ~CollisionSpriteLayer() override = default;

        void    addCollisionSprite(spCollisionSprite cspr);

        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

        float   aaLoSH(const spCollisionSprite me, const spCollisionSprite target);
        float   loS(const spCollisionSprite me, const spCollisionSprite target);

    protected:

        void handleCollisionDetection(const Gear& gear);

        void    projectCollisions(const spCollisionSprite& me, std::forward_list<manifold>& mf_list);
        bool    getRayIntersectionFraction(const Vector2F& origin, const Vector2F& direction, const RectF& aabb, float& ti, Vector2F& normal);
        float   getRayIntersectionFractionOfFirstRay(const Vector2F &originA, const Vector2F &endA, const Vector2F &originB, const Vector2F &endB);
//        static void    slide(manifold& mf, bool for_me);
//        static void    touch(manifold& mf, bool for_me);
//        static void    bounce(manifold& mf, bool for_me);


        std::unordered_set<spSprite> _handled;
        uint16_t                    _iteration{3};       // number of collision solving cycles
        bool                        _iterate{false};    // internal use

    private:
        // may not be used in this layer type
        void    addSprite(spSprite spr) override {};
#ifdef DANG_DEBUG_DRAW
        int _dbg_mem{0};
#endif
    };

}


