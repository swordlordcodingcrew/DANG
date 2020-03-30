// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include <iostream>
#include <set>
#include "Collision.h"
#include "Sprite.h"

namespace dang
{

    // called on every move of every sprite
    void Collision::handle(spSprite me, std::vector<collision_object> &collisions)
    {
        if (me->getLastPos() == me->getPos())
        {
            return;
        }

//        check(me, collisions);

//        update(me, goal);

    }

/*
    void Collision::check(spSprite me, std::vector<collision_object> &collisions)
    {
        std::vector<spSprite> visitedSprites;
        visitedSprites.push_back(me);

        std::vector<collision_object> projectedCollisions;

        projectCollisions(lvl, me, goal, &projectedCollisions);

        // TODO uhm?
        //moveto->x = goal->x;
        //moveto->y = goal->y;

        std::cout << "check id: " << me->_id << " lastpos: " << me->getLastPos().x << "," << me->getLastPos().y << " pos:" << me->getPos().x << "," << me->getPos().y;

        // check projected colls if they really collide, generate a new list and return that one
        for (auto i =projectedCollisions.begin(); i!=projectedCollisions.end(); ++i)
        {
            collision_object col = *i;

            collisions.push_back(col);

            col.me->_is_hit = true;
            col.other->_is_hit = true;

//            std::cout << "check id: " << me->_id << " lastpos: " << me->getLastPos().x << "," << me->getLastPos().y << " other lastpos:" << col.other->getLastPos().x << "+" << col.other->getSizeRect().w << " - ";

            std::vector<collision_object> lC;

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
*/
    // all collisions worked through, lets update the sprite
/*    void update(std::shared_ptr<sprite> me, point* moveto) {

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
*/

/*    void Collision::projectCollisions(spSprite me, blit::Rect* r, blit::Point* goal, std::vector<collision_object>* projectedCollisions)
    {
        // TODO assert that r.x,y,w,h is a rect
        std::set<spSprite> visitedSprites;
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
*/

/*    bool detectCollision(spSprite me, blit::Rect* r, spSprite other, blit::Point* goal, collision_object* col)
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
             *//*
        }

        // todo: unsure, no movement? -> probably not needed since hasResult = false results in return as well

//        if(ti == 0)
//        {
//            return false;
//        }

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
//                    if(indOut.t1 == 0)
//                    {
//                        return false;
//                    }
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
*/

}