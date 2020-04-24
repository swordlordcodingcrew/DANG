// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#pragma once

#include <Vector2T.h>
#include <RectT.h>
#include <forward_list>
#include "dang_globals.hpp"

namespace dang
{
    class Sprite;

    struct collision_object
    {
        spSprite me;
        spSprite other;
        bool overlaps;
        float ti;
        Vector2F normal;
        Vector2F delta;
        Vector2F touch;
        uint16_t type;       // used with const above: ResponseTouch, ResponseCross, etc

        /*
         *
         * cols[i] = {
              item  = the item being moved / checked
              other = an item colliding with the item being moved
              type  = the result of `filter(other)`. It's usually "touch", "cross", "slide" or "bounce"
              overlaps  = boolean. True if item "was overlapping" other when the collision started.
                          False if it didn't but "tunneled" through other
              ti        = Number between 0 and 1. How far along the movement to the goal did the collision occur>
              normal    = Vector({x=number,y=number}). The collision normal; usually -1,0 or 1 in `x` and `y`
              touch     = Vector({x=number,y=number}). The coordinates where item started touching other
            }
*/
    };

    class SweptAABBCollision
    {
    public:
        enum eCollisionResponse{CR_NONE, CR_TOUCH, CR_CROSS, CR_SLIDE, CR_BOUNCE};
        /**
         * loosely based on box2d
         * OT_RIGID: should not move, Rigid object will not collide with oder rigid object or sensor. Example: Wall
         * OT_DYNAMIC: moving object. Hero, enemies
         */
        enum eCollObjectType{COT_RIGID, COT_DYNAMIC};

        void handleSprite(spSprite me, const std::forward_list<spSprite>& sprites);

        void project(const spSprite& me, const std::forward_list<spSprite>& sprites, std::forward_list<collision_object>& collision_objects);
//        bool getSegmentIntersectionRel(const RectF& rect, const Vector2F& start_seg, const Vector2F& end_seg,
//                                       float& relEntry, float& relExit, Vector2F& entryNormal, Vector2F& exitNormal);
//        bool detectCollision_(const RectF& meRect, const RectF& otherRect, const Vector2F& goal, collision_object& coll_o);

        bool detectCollision(const RectF& meRect, const RectF& otherRect, const Vector2F& delta, collision_object& coll_o);
        bool getRayIntersectionFraction(const Vector2F& origin, const Vector2F& direction, const RectF& aabb, collision_object& coll_o);

    protected:

//        float   getRayIntersectionFractionOfFirstRay_(const Vector2F &originA, const Vector2F &endA, const Vector2F &originB, const Vector2F &endB);
        float   getRayIntersectionFractionOfFirstRay(const Vector2F &originA, const Vector2F &endA, const Vector2F &originB, const Vector2F &endB);

        const float TINY_DELTA{0.01};
    };

}


