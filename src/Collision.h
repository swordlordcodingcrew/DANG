// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#pragma once

#include <Vector2T.h>
#include <RectT.h>
#include "dang_globals.hpp"

namespace dang
{
    class Sprite;

    struct indices {
        float t1;
        float t2;
        Vector2F n1;
        Vector2F n2;
    };

    static const float DELTA = 1e-10;
    enum collisionResponse{none, touch_me, cross_me, slide_over_me, bounce_off_me};

    struct collision_object
    {
        spSprite me;
        spSprite other;
        bool overlaps;
        float ti;
        Vector2F move;
        Vector2F normal;
        Vector2F touch;
        Vector2F slide;
        Vector2F bounce;
        uint8_t type;       // used with const above: ResponseTouch, ResponseCross, etc

        /*
         *
         * cols[i] = {
              item  = the item being moved / checked
              other = an item colliding with the item being moved
              type  = the result of `filter(other)`. It's usually "touch", "cross", "slide" or "bounce"
              overlaps  = boolean. True if item "was overlapping" other when the collision started.
                          False if it didn't but "tunneled" through other
              ti        = Number between 0 and 1. How far along the movement to the goal did the collision occur>
              move      = Vector({x=number,y=number}). The difference between the original coordinates and the actual ones.
              normal    = Vector({x=number,y=number}). The collision normal; usually -1,0 or 1 in `x` and `y`
              touch     = Vector({x=number,y=number}). The coordinates where item started touching other
              itemRect  = The rectangle item occupied when the touch happened({x = N, y = N, w = N, h = N})
              otherRect = The rectangle other occupied when the touch happened({x = N, y = N, w = N, h = N})

            }
*/
    };

    class Collision
    {
    public:
        void handle(spSprite me, std::vector<collision_object> &collisions);

/*        std::vector<collision_object> collisions;

        // void move(sprite* me, point* goal, point* moveto, std::vector<collision>* collisions)
        move(&lvl, s, &goal, &collisions);
*/
    protected:
//        void check(spSprite me, std::vector<collision_object> &collisions);
//        void projectCollisions(spSprite me, RectF* r, blit::Point* goal, std::vector<collision_object>* projectedCollisions);


    };

}


