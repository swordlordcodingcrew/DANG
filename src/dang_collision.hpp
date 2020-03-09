// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// based on sourcecode Copyright (c) 2014 Enrique García Cota

#pragma once

#include "dang_globals.hpp"
#include <float.h>
#include <set>
#include <iostream>

using namespace dang;

namespace dang_collision {

    struct indices {
        float_t t1;
        float_t t2;
        point n1;
        point n2;
    };

    static const float DELTA = 1e-10;

    struct collision
    {
        std::shared_ptr<sprite>         me;
        std::shared_ptr<sprite>         other;
        bool            overlaps;
        float_t         ti ;
        point           move;
        point           normal;
        point           touch;
        point           slide;
        point           bounce;
        uint8_t         type;       // used with const above: ResponseTouch, ResponseCross, etc

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

        void assignMe(std::shared_ptr<sprite> p)
        {
            me = p;
        }

        void assignOther(std::shared_ptr<sprite> p)
        {
            other = p;
        }
    };

    bool hasIntersection(rect* A, rect* B)
    {
        int Amin, Amax, Bmin, Bmax;

        if (!A) {
            // SDL_InvalidParamError("A");
            return false;
        }

        if (!B) {
            //SDL_InvalidParamError("B");
            return false;
        }

        /* Special cases for empty rects */
        //if (SDL_RectEmpty(A) || SDL_RectEmpty(B)) {
        //    return SDL_FALSE;
        //}

        /* Horizontal intersection */
        Amin = A->x;
        Amax = Amin + A->w;
        Bmin = B->x;
        Bmax = Bmin + B->w;
        if (Bmin > Amin){
            Amin = Bmin;
        }
        if (Bmax < Amax){
            Amax = Bmax;
        }
        if (Amax <= Amin){
            return false;
        }
        /* Vertical intersection */
        Amin = A->y;
        Amax = Amin + A->h;
        Bmin = B->y;
        Bmax = Bmin + B->h;
        if (Bmin > Amin) {
            Amin = Bmin;
        }
        if (Bmax < Amax) {
            Amax = Bmax;
        }

        if (Amax <= Amin){
            return false;
        }

        return true;
    }

    /* Use the Cohen-Sutherland algorithm for line clipping */
    #define CODE_BOTTOM 1
    #define CODE_TOP    2
    #define CODE_LEFT   4
    #define CODE_RIGHT  8

    static int ComputeOutCode(rect* rect, int x, int y)
    {
        int code = 0;
        if (y < 0)
        {
            code |= CODE_TOP;
        } else if (y >= rect->y + rect->h)
        {
            code |= CODE_BOTTOM;
        }
        if (x < 0)
        {
            code |= CODE_LEFT;
        } else if (x >= rect->x + rect->w)
        {
            code |= CODE_RIGHT;
        }

        return code;
    }

    bool intersectRectAndLine(rect* rect, int* X1, int* Y1, int* X2, int* Y2)
    {
       int x = 0;
       int y = 0;
       int x1, y1;
       int x2, y2;
       int rectx1;
       int recty1;
       int rectx2;
       int recty2;
       int outcode1, outcode2;

       if (!rect) {
           //SDL_InvalidParamError("rect");
           return false;
       }

       if (!X1) {
           //SDL_InvalidParamError("X1");
           return false;
       }

       if (!Y1) {
           //SDL_InvalidParamError("Y1");
           return false;
       }

       if (!X2) {
           //SDL_InvalidParamError("X2");
           return false;
       }

       if (!Y2) {
           //SDL_InvalidParamError("Y2");
           return false;
       }

       // Special case for empty rect
       //if (SDL_RectEmpty(rect)) {
       //    return SDL_FALSE;
       //}

        x1 = *X1;
        y1 = *Y1;
        x2 = *X2;
        y2 = *Y2;
        rectx1 = rect->x;
        recty1 = rect->y;
        rectx2 = rect->x + rect->w - 1;
        recty2 = rect->y + rect->h - 1;

         // Check to see if entire line is inside rect
         if (x1 >= rectx1 && x1 <= rectx2 && x2 >= rectx1 && x2 <= rectx2 &&
             y1 >= recty1 && y1 <= recty2 && y2 >= recty1 && y2 <= recty2) {
             return true;
         }

         // Check to see if entire line is to one side of rect
         if ((x1 < rectx1 && x2 < rectx1) || (x1 > rectx2 && x2 > rectx2) ||
             (y1 < recty1 && y2 < recty1) || (y1 > recty2 && y2 > recty2)) {
             return false;
         }

         if (y1 == y2)
         {
             // Horizontal line, easy to clip
             if (x1 < rectx1)
             {
                 *X1 = rectx1;
             } else if (x1 > rectx2)
             {
                 *X1 = rectx2;
             }

             if (x2 < rectx1)
             {
                 *X2 = rectx1;
             } else if (x2 > rectx2)
             {
                 *X2 = rectx2;
             }
             return true;
         }

         if (x1 == x2)
         {
             // Vertical line, easy to clip
             if (y1 < recty1)
             {
                 *Y1 = recty1;
             } else if (y1 > recty2)
             {
                 *Y1 = recty2;
             }

             if (y2 < recty1)
             {
                 *Y2 = recty1;
             } else if (y2 > recty2)
             {
                 *Y2 = recty2;
             }
             return true;
         }

         // More complicated Cohen-Sutherland algorithm
         outcode1 = ComputeOutCode(rect, x1, y1);
         outcode2 = ComputeOutCode(rect, x2, y2);
         while (outcode1 || outcode2)
         {
             if (outcode1 & outcode2)
             {
                 return false;
             }

             if (outcode1)
             {
                 if (outcode1 & CODE_TOP) {
                     y = recty1;
                     x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
                 } else if (outcode1 & CODE_BOTTOM) {
                     y = recty2;
                     x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
                 } else if (outcode1 & CODE_LEFT) {
                     x = rectx1;
                     y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
                 } else if (outcode1 & CODE_RIGHT) {
                     x = rectx2;
                     y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
                 }
                 x1 = x;
                 y1 = y;
                 outcode1 = ComputeOutCode(rect, x, y);
             } else {
                 if (outcode2 & CODE_TOP) {
                     y = recty1;
                     x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
                 } else if (outcode2 & CODE_BOTTOM) {
                     y = recty2;
                     x = x1 + ((x2 - x1) * (y - y1)) / (y2 - y1);
                 } else if (outcode2 & CODE_LEFT) {
                     x = rectx1;
                     y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
                 } else if (outcode2 & CODE_RIGHT) {
                     x = rectx2;
                     y = y1 + ((y2 - y1) * (x - x1)) / (x2 - x1);
                 }
                 x2 = x;
                 y2 = y;
                 outcode2 = ComputeOutCode(rect, x, y);
             }
         }

         *X1 = x1;
         *Y1 = y1;
         *X2 = x2;
         *Y2 = y2;
         return true;
     }

    // This is a generalized implementation of the liang-barsky algorithm, which also returns
    // the normals of the sides where the segment intersects.
    // Returns false if the segment never touches the rect
    // Notice that normals are only guaranteed to be accurate when initially ti1, ti2 == -math.huge, math.huge
    bool getSegmentIntersectionIndices(rect* r, point* p1, point* p2, float_t ti1, float_t ti2, indices* indOut)
    {
        // TODO is t11 and ti2 always set?
        // do ti1 = 0, ti2 = 1 otherwise

        int32_t dx = p2->x - p1->x;
        int32_t dy = p2->y - p1->y;

        point n = {0, 0};

        point n1 = {0, 0};
        point n2 = {0, 0};

        int32_t p = 0;
        int32_t q = 0;

        float_t fR = 0.0f;

        for (uint8_t side = 1; side <= 4; ++side)
        {
            if (side == 1) // left
            {
                n = {-1, 0};
                p = -dx;
                q = p1->x - r->x;
            }
            else if (side == 2) // right
            {
                n = {1, 0};
                p = dx;
                q = r->x + r->w - p1->x;
            }
            else if (side == 3) // top
            {
                n = {0, -1};
                p = -dy;
                q = p1->y - r->y;
            }
            else if (side == 4) // bottom
            {
                n = {0, 1};
                p = dy;
                q = r->y + r->h - p1->y;
            }

            if (p == 0)
            {
                if (q <= 0)
                {
                    return false;
                }
            }
            else
            {
                fR = static_cast<float>(q) / static_cast<float>(p);
                if (p < 0)
                {
                    if (fR > ti2)
                    {
                        return false;

                    }
                    else if (fR > ti1)
                    {
                        ti1 = fR;
                        n1.x = n.x;
                        n1.y = n.y;
                    }
                }
                else
                { // TODO p >= 0, although == does never happen?

                    if (fR < ti1)
                    {
                        return false;
                    }
                    else if (fR < ti2)
                    {
                        ti2 = fR;
                        n2 = {};
                        n2.x = n.x;
                        n2.y = n.y;
                    }
                }
            }
        }

        indOut->n1 = n1;
        indOut->n2 = n2;
        indOut->t1 = ti1;
        indOut->t2 = ti2;

        return true;
    }

    int32_t sign(int32_t x)
    {
        if(x > 0)
        {
            return 1;
        } else if( x == 0) {
            return 0;
        } else {
            return -1;
        }
    }

    int32_t nearest(int32_t x, int32_t a, int32_t b)
    {
        if(std::abs(a - x) < std::abs(b - x))
        {
            return a;
        } else {
            return b;
        }
    }

    bool rectContainsPoint(int32_t x, int32_t y, int32_t w, int32_t h, int32_t px, int32_t py)
    {
        /*
        // TODO DELTA?
        int32_t dx = (px-x) > DELTA;
        int32_t dy = (py-y) > DELTA;
        int32_t dw = (x+w-px) > DELTA;
        int32_t dh = (y+h-py) > DELTA;

        return dx && dy && dw && dh;
         */

        return ( (px >= x) && (px < (x + w)) && (py >= y) && (py < (y + h)) ) ? true : false;
    }

    bool rectContainsPoint(rect* r, int32_t px, int32_t py)
    {
        return rectContainsPoint(r->x, r->y, r->w, r->h, px, py);
    }

    // TODO reconsider return values
    point rectGetNearestCorner(int32_t x, int32_t y, int32_t w, int32_t h, int32_t px, int32_t py)
    {
        point p = {nearest(px, x, x+w), nearest(py, y, y+h)};

        return p;
    }

    point rectGetNearestCorner(rect* r, int32_t px, int32_t py)
    {
        point p = {nearest(px, r->x, r->x + r->w), nearest(py, r->y, r->y + r->h)};

        return p;
    }

    // calculates the minkowsky difference between 2 rects, which is another rect
    rect getDiff(int32_t x1, int32_t y1, int32_t w1, int32_t h1, int32_t x2, int32_t y2, int32_t w2, int32_t h2)
    {
        rect r = {x2 - x1 - w1,
                  y2 - y1 - h1,
                  w1 + w2,
                  h1 + h2};

        return r;
    }

    rect getDiff(rect* r, std::shared_ptr<sprite> other) {

        return getDiff(r->x, r->y, r->w, r->h, other->x, other->y, other->width, other->height);
    }

    rect getDiff(std::shared_ptr<sprite> me, std::shared_ptr<sprite> other) {

        return getDiff(me->x, me->y, me->width, me->height, other->x, other->y, other->width, other->height);
    }

    bool detectCollision(std::shared_ptr<sprite> me, rect* r, std::shared_ptr<sprite> other, point* goal, collision* col)
    {
        point difference = {goal->x - r->x, goal->y - r->y};

        // Calculate the minkowski difference between 2 elements (me and other)
        //x, y, w, h := GetDiffByItems(item, other) -> original, but wrong IMHO
        //x, y, w, h := GetDiff(goalX, goalY, item.W, item.H, other.X, other.Y, other.W, other.H)
        //rect rm = {goal->x, goal->y, r->w, r->h};
        rect rMnkwsk = getDiff(r, other);

        //std::cout << " mnk:" << rMnkwsk.x << "-" << rMnkwsk.y << "-" << rMnkwsk.y << "-" << rMnkwsk.h ;

        bool overlaps = false;
        float_t ti = 0.0f;

        // todo unsure if int8 is better
        point normal = {0,0};

        rect ro = {other->x, other->y, other->width, other->height};
        rect rm = {goal->x, goal->y, r->w, r->h};

        // is me intersecting with other?
        if(hasIntersection(&ro, &rm))
        {
        //if(rectContainsPoint(&rMnkwsk, 0, 0))
        //{
            point p = rectGetNearestCorner(&rMnkwsk, 0, 0);

            // area of intersection
            int32_t wi = std::min(r->w, std::abs(p.x));
            int32_t hi = std::min(r->h, std::abs(p.y));

            // ti is the negative area of intersection
            ti = -wi * hi;

            overlaps = true;

        } else {

            indices indOut;
            point p1 = {0,0};
            point p2 = {difference.x, difference.y};

            // bool getSegmentIntersectionIndices(rect* r, point* p1, point* p2, float_t ti1, float_t ti2, indices* indOut)
            // TODO replace by SDL IntersectRectAndLine -> currentxy -> goalxy as line, other as rect

            int x1 = r->x;
            int y1 = r->y;

            int x2 = goal->x;
            int y2 = goal->y;

            // does item tunnel through other?
            bool doesIntersect = intersectRectAndLine(&ro, &x1, &y1, &x2, &y2);
            if(doesIntersect)
            {
                // find out where it hits
                // handle (depending on strategy, move back, kill, ?)
                std::cout << " tunneling!" << std::endl;

            }
            else
            {
                return false; // no collision whatsoever..
            }

            /*
             * Was:
            bool hasResult = getSegmentIntersectionIndices(&rMnkwsk, &p1, &p2, -FLT_MAX, FLT_MAX, &indOut);
            if(hasResult)
            {
                // item tunnels into other
                // not sure if group in the OR is correct, there was none in the lua version
                if(indOut.t1 > 0 && indOut.t1 < 1 && std::abs(indOut.t1 - indOut.t2) >= DELTA && (0 < indOut.t1 + DELTA || (indOut.t1 == 0 && indOut.t2 > 0)))
                {
                    ti = indOut.t1;
                    normal.x = indOut.n1.x;
                    normal.y = indOut.n1.y;
                    overlaps = false;
                }
            } else {
                return false; // no collision whatsoever.. could be handled differently?
            }
             */
        }

        // todo: unsure, no movement? -> probably not needed since hasResult = false results in return as well
        /*
        if(ti == 0)
        {
            return false;
        }*/

        point touch = {0,0};

        // both rects overlap
        if(overlaps)
        {
            // No movement at all
            if(difference.x == 0 && difference.y == 0)
            {
                // intersecting and not moving - use minimum displacement vector
                point p = rectGetNearestCorner(rMnkwsk.x, rMnkwsk.y, rMnkwsk.w, rMnkwsk.h, 0, 0);
                if(std::abs(p.x) < std::abs(p.y))
                {
                    p.y = 0;
                } else {
                    p.x = 0;
                }

                normal.x = sign(p.x);
                normal.y = sign(p.y);
                touch.x = r->x + p.x;
                touch.y = r->y + p.y;

            } else {
                // intersecting and moving  move in the opposite direction
                indices indOut;
                point p1 = {0,0};
                point p2 = {difference.x, difference.y};

                // bool getSegmentIntersectionIndices(rect* r, point* p1, point* p2, float_t ti1, float_t ti2, indices* indOut)
                bool hasResult = getSegmentIntersectionIndices(&rMnkwsk, &p1, &p2, -FLT_MAX, 1.0f, &indOut);
                if(hasResult)
                {
                    // todo not correct
                    /*if(indOut.t1 == 0)
                    {
                        return false;
                    }*/
                    // in the original these are not shadowed...
                    normal.x = indOut.n1.x;
                    normal.y = indOut.n1.y;

                    // there were no parantheses in the original, according to
                    // manual LUA uses PEMDAS which should equal following code.
                    touch.x = r->x + (difference.x * indOut.t1);
                    touch.y = r->y + (difference.y * indOut.t1);

                } else {
                    // TODO check: not sure, but probably what is needed
                    return false;
                }
            }
        } else { // tunnel

            // there were no parantheses in the original, according to
            // manual LUA uses PEMDAS which should equal following code.
            touch.x = r->x + (difference.x * ti);
            touch.y = r->y + (difference.y * ti);
        }

        col->assignMe(me);
        col->assignOther(other);
        col->overlaps = overlaps;
        col->ti = ti;
        col->move = difference;
        col->normal = normal;
        col->touch = touch;

        if(col->overlaps){
            std::cout << "ti: " << col->ti;
            std::cout << " | mv: " << col->move.x << "," << col->move.y;
            std::cout << " | n: " << col->normal.x << "," << col->normal.y;
            std::cout << " | t: " << col->touch.x << "," << col->touch.y << std::endl;
        }

        //return true;
        return overlaps;
    }

    bool detectCollision(std::shared_ptr<sprite> me, std::shared_ptr<sprite> other, point* goal, collision* col)
    {
        rect r = {me->x, me->y, me->width, me->height};
        return detectCollision(me, &r, other, goal, col);
    }

    void projectCollisions(dang::level* lvl, std::shared_ptr<sprite> me, rect* r, point* goal, std::vector<collision>* projectedCollisions)
    {
        // TODO assert that r.x,y,w,h is a rect
        std::set<std::shared_ptr<sprite>> visitedSprites;
        visitedSprites.insert(me);

        int32_t tl = std::min(goal->x, r->x); // left
        int32_t tt = std::min(goal->y, r->y); // top

        int32_t tr = std::max(goal->x + r->w, r->x + r->w); // right
        int32_t tb = std::max(goal->y + r->h, r->y + r->h); // bottom

        int32_t tw = tr - tl; // width
        int32_t th = tb - tt; // height

        // TODO get all items instead of items from cells
        // TODO add cell check if working with cells
        for(auto i = lvl->sprites.begin(); i != lvl->sprites.end(); i++)
        {
            std::shared_ptr<sprite> other = *i;

            // only handle the ones we did not already handle
            bool exists = (visitedSprites.find(other) != visitedSprites.end());
            if(!exists)
            {
                visitedSprites.insert(other);

                // ask other item if it wants to collide
                u_int8_t collisionResponse = other->wantToCollideWith(me);
                if(collisionResponse > 0)
                {
                    collision col = {};
                    //bool doesCollide = detectCollision(me, other, goal, &col);
                    bool doesCollide = detectCollision(me, r, other, goal, &col);
                    if(doesCollide)
                    {
                        // fmt.Printf("We wanna collide: item:%v other:%v goalX:%v goalX:%v \n", item, other, goalX, goalY)
                        col.type = collisionResponse;
                        projectedCollisions->push_back(col);

                    } else {
                        //fmt.Printf("No collision: item:%v other:%v goalX:%v goalX:%v\n", item, other, goalX, goalY)
                    }
                }
            }
        }

        // TODO sort collisions sortByTiAndDistance
    }

    void projectCollisions(dang::level* lvl, std::shared_ptr<sprite> me, point* goal, std::vector<collision>* projectedCollisions)
    {
        rect r = {me->x, me->y, me->width, me->height};
        return projectCollisions(lvl, me, &r, goal, projectedCollisions);
    }

    // Type: Arrow
    void touch(collision* col, std::shared_ptr<sprite> me, point* goal)
    {
        goal->x = col->touch.x;
        goal->y = col->touch.y;
    }

    // Type: Coin
    void cross(dang::level* lvl, collision* col, std::shared_ptr<sprite> me, point* goal, std::vector<collision>* projectedCollisions)
    {
        // TODO something is missing
        projectCollisions(lvl, me, goal, projectedCollisions);
    }

    // Type: Mario
    void slide(dang::level* lvl, collision* col, std::shared_ptr<sprite> me, point* goal, std::vector<collision>* projectedCollisions)
    {
        if(col->move.x != 0 || col->move.y != 0)
        {
            // TODO wrong? because too simple?
            // if movement on the x axis
            if(col->normal.x != 0)
            {
                goal->x = col->touch.x;
            }
            if (col->normal.y != 0) {
                goal->y = col->touch.y;
            }
        }

        col->slide.x = goal->x;
        col->slide.y = goal->y;

        //rect r = {col->touch.x, col->touch.y, me->width, me->height};
        rect r = {goal->x, goal->y, me->width, me->height};

        std::cout << " - SLIDE " << col->touch.x << "," << col->touch.y << " ";

        projectCollisions(lvl, me, &r, goal, projectedCollisions);
    }

    // TODO has some edge case where it bounces too much
    // Type: Arkanoid
    void bounce(dang::level* lvl, collision* col, std::shared_ptr<sprite> me, point* goal, std::vector<collision>* projectedCollisions)
    {
        int32_t bx = 0;
        int32_t by = 0;

        if(col->move.x != 0 || col->move.y != 0)
        {

            int32_t bnx = goal->x - col->touch.x;
            int32_t bny = goal->y - col->touch.y;

            if(col->normal.x == 0)
            {
                bny = -bny;
            } else {
                bnx = -bnx;
            }

            bx = col->touch.x + bnx;
            by = col->touch.y + bny;
        }

        col->bounce.x = bx;
        col->bounce.y = by;

        goal->x = bx;
        goal->y = by;

        rect r = {col->touch.x, col->touch.y, me->width, me->height};
        projectCollisions(lvl, me, &r, goal, projectedCollisions);
        //    return goalX, goalY, collisions
    }

    void check(dang::level* lvl, std::shared_ptr<sprite> me, point* goal, std::vector<collision>* collisions)
    {
        std::vector<std::shared_ptr<sprite>> visitedSprites;
        visitedSprites.push_back(me);

        std::vector<collision> projectedCollisions;

        projectCollisions(lvl, me, goal, &projectedCollisions);

        // TODO uhm?
        //moveto->x = goal->x;
        //moveto->y = goal->y;

        std::cout << "check id: " << me->id << " is: " << me->x << "," << me->y << " goal:" << goal->x << "," << goal->y;

        // check projected colls if they really collide, generate a new list and return that one
        for (auto i =projectedCollisions.begin(); i!=projectedCollisions.end(); ++i)
        {
            collision col = *i;

            collisions->push_back(col);

            col.me->isHit = true;
            col.other->isHit = true;

            std::cout << "check id: " << me->id << " is: " << me->x << "," << me->y << " her:" << col.other->x << "+" << col.other->width << " - ";

            std::vector<collision> lC;

            switch(col.type)
            {
                case touch_me:
                    touch(&col, me, goal);
                    break;

                case bounce_off_me:
                    bounce(lvl, &col, me, goal, &lC);
                    //TODO goalX, goalY, _ = bounce(collision, item, goalX, goalY)
                    break;

                case cross_me:
                    cross(lvl, &col, me, goal, &lC);
                    //TODO goalX, goalY, _ = cross(collision, item, goalX, goalY)
                    break;

                case slide_over_me:
                    slide(lvl, &col, me, goal, &lC);
                    //TODO goalX, goalY, _ = slide(collision, item, goalX, goalY)
                    break;
            }
        }

        std::cout << " - after processing goal.x " << goal->x << " goal.y " << goal->y << std::endl;

    }

    // all collisions worked through, lets update the sprite
    void update(std::shared_ptr<sprite> me, point* moveto) {

        // only needed if using cells...
        // rect rMoveto = {moveto->x, moveto->y, me->width, me->height};
        // TODO assert is rect rMoveTo
        // TODO if working with cells, you should update the cells here...

        // now lets set the value on the item
        // not setting w and h since these did not change...
        if(me->x != moveto->x)
        {
            me->x = moveto->x;
        }
        if(me->y != moveto->y)
        {
            me->y = moveto->y;
        }

        std::cout << "id: " << me->id << " pos: " << me->x << "," << me->y << std::endl;
    }

    // called on every move of every sprite
    void move(dang::level* lvl, std::shared_ptr<sprite> me, point* goal, std::vector<collision>* collisions)
    {
        //fmt.Printf("Moving: item:%v goalX:%v goalY:%v\n", item, goalX, goalY)

        check(lvl, me, goal, collisions);

        update(me, goal);
    }
} // End Of Namespace DANG_COLLISION