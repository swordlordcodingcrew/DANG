// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#pragma once

#include <Vector2T.h>
#include <RectT.h>
#include <forward_list>
#include <unordered_set>
#include "dang_globals.hpp"

namespace dang
{
    class Sprite;

    struct manifold
    {
        bool overlaps;      // true if sprites overlap even before moving. N.B. ti is in that case not valid
        float ti;           // between 0 and 1. How far along the delta to the goal did the collision occur

        spSprite me;     // the sprite
        Vector2F deltaMe;     // relative movement of sprite me
        Vector2F normalMe;  // collision vector for sprite me; -1,0 or 1 in `x` and `y`
        Vector2F touchMe;   // touch position for sprite me

        spSprite other;     // the other sprite
        Vector2F deltaOther;     // relative movement of sprite me
        Vector2F normalOther;   // collision vector for sprite other; -1,0 or 1 in `x` and `y`
        Vector2F touchOther;    // touch position for sprite other

    };

    class SweptAABBCollision
    {
    public:
        enum eCollisionResponse {
            CR_NONE = 0x00,
            CR_TOUCH = 0x01,
            CR_CROSS = 0x02,
            CR_SLIDE = 0x04,
            CR_BOUNCE = 0x08};

        /**
         * loosely based on box2d
         * OT_RIGID: should not move, Rigid object will not collide with oder rigid object. Example: Wall
         * OT_DYNAMIC: moving object. Hero, enemies
         */
        enum eCollObjectType{COT_RIGID, COT_DYNAMIC};

        void handleCollisionDetection(std::forward_list<spSprite>& sprites);

    protected:

        void projectCollisions(const spSprite& me, const std::forward_list<spSprite>& sprites, std::forward_list<manifold>& mf_list);
        bool getRayIntersectionFraction(const Vector2F& origin, const Vector2F& direction, const RectF& aabb, float& ti, Vector2F& normal);
        float   getRayIntersectionFractionOfFirstRay(const Vector2F &originA, const Vector2F &endA, const Vector2F &originB, const Vector2F &endB);
        void    slide(manifold& mf, bool for_me);
        void    touch(manifold& mf, bool for_me);
        void    bounce(manifold& mf, bool for_me);


        std::unordered_set<spSprite> _handled;
        uint16_t    _iteraton{3};
        bool        _iterate{false};
    };

}


