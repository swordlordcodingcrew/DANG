// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include <iostream>
#include <unordered_set>
#include "SweptAABBCollision.h"
#include "Sprite.h"

namespace dang
{

    // called on every move of every sprite
    void SweptAABBCollision::handleSprite(spSprite me, const std::forward_list<spSprite>& sprites)
    {
        // possible collisions
        std::forward_list<collision_object> proj_coll_o;
        //
//        std::unordered_set<spSprite> visited;

//        visited.insert(me);
        project(me, sprites, proj_coll_o);

        while (!proj_coll_o.empty())
        {
            collision_object coll_o = proj_coll_o.front();
//            std::cout << me->_type << " has collision. ti=" << coll_o.ti << std::endl;
//            std::cout << "pos me=" << me->getPos().x << ", " << me->getPos().y << std::endl;
//            std::cout << "pos other=" << coll_o.other->getPos().x << ", " << coll_o.other->getPos().y << std::endl;

//            visited.insert(coll_o.other);

            if (coll_o.overlaps)
            {
                me->setPos(coll_o.touch);
            }
            else
            {
                Vector2F deltaMe = (me->getPos() - me->getLastPos());
                Vector2F deltaOther = (coll_o.other->getPos() - coll_o.other->getLastPos());

                me->setPos(me->getLastPos() + deltaMe * coll_o.ti);
                if (coll_o.other->_coll_object_type != COT_RIGID)
                {
                    coll_o.other->setPos(me->getLastPos() + deltaOther * coll_o.ti);
                }

            }
//            std::cout << "pos me=" << me->getPos().x << ", " << me->getPos().y << std::endl;
//            std::cout << "pos other=" << coll_o.other->getPos().x << ", " << coll_o.other->getPos().y << std::endl;

            proj_coll_o.pop_front();

        }

    }

    void SweptAABBCollision::project(const spSprite& me, const std::forward_list<spSprite>& sprites, std::forward_list<collision_object>& collision_objects)
    {
        for (const spSprite& other : sprites)
        {
            if (me == other)
            {
                continue;
            }

            if (me->_coll_response == CR_NONE || other->_coll_response == CR_NONE)
            {
                continue;
            }

            collision_object col = {};
            RectF meRect = me->getHotrect();
            meRect.x += me->getLastPos().x;
            meRect.y += me->getLastPos().y;
            Vector2F deltaMe = me->getPos() - me->getLastPos();
            RectF otherRect = other->getHotrect();
            otherRect.x += other->getLastPos().x;
            otherRect.y += other->getLastPos().y;
            Vector2F deltaOther = other->getPos() - other->getLastPos();
            Vector2F delta = deltaMe - deltaOther;
            if (detectCollision(meRect, otherRect, delta, col))
            {
//                col.type = me->_coll_response;
                col.me = me;
                col.other = other;
//                me->_is_hit = true;
//                other->_is_hit = true;
                collision_objects.push_front(col);
            }
        }

        // sort by ti
        collision_objects.sort([] (const collision_object &first, const collision_object &second)
            {
                return first.ti < second.ti;
            }
        );

    }

    /**
     * check if two rects collide
     * @param meRect rect of the first sprite
     * @param otherRect rect of the second sprite
     * @param delta displacement of the rects. I.e. relative velocity (me->_vel - other->vel) * dt
     * @param coll_o collision object to be filled up
     * @return true if a collision happened
     */
    bool SweptAABBCollision::detectCollision(const RectF& meRect, const RectF& otherRect, const Vector2F& delta, collision_object& coll_o)
    {
        RectF rMink = meRect.minkowskiDiff(otherRect);

        if (rMink.contains(Vector2F(0,0)))    // intersection of the two rects without any velocity
        {
            // get the nearest side
            Vector2F nearestBound = rMink.getNearestCorner(Vector2F(0,0));

            // special case if some line are co-linear
            if (nearestBound.x == 0)
            {
                if (meRect.x <= otherRect.x)
                {
                    nearestBound.x -= TINY_DELTA;
                }
                else
                {
                    nearestBound.x += TINY_DELTA;
                }
            }

            if (nearestBound.y == 0)
            {
                if (meRect.y <= otherRect.y)
                {
                    nearestBound.y -= TINY_DELTA;
                }
                else
                {
                    nearestBound.y += TINY_DELTA;
                }
            }

            if (std::abs(nearestBound.x) < std::abs(nearestBound.y))
            {
                nearestBound.y = 0;
            }
            else
            {
                nearestBound.x = 0;
            }
            coll_o.touch = meRect.tl() + nearestBound;
            coll_o.overlaps = true,
            coll_o.ti = 0;
            nearestBound.normalize();
            coll_o.normal = nearestBound;

            return true;
        }
        else
        {
            if (getRayIntersectionFraction(Vector2F(0,0), delta, rMink, coll_o))
            {
                coll_o.touch = meRect.tl() + coll_o.ti * delta;
                coll_o.overlaps = false;
                // ti and normal is already set

                return true;
            }
        }

        return false;
    }

    /**
     * according to https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282#565282
     * post of Gareth
     * p and p + r
     * q and q + s
     * p + t * r = q + u * s
     * t = (q − p) × s / (r × s)
     * u = (q − p) × r / (r × s)
     * if r × s = 0 and (q − p) × r = 0, then the two lines are collinear.
     *
     * Now there are four cases:
     * 1. If r × s = 0 and (q − p) × r = 0, then the two lines are collinear.
     *    In this case, express the endpoints of the second segment (q and q + s) in terms of the equation of the first line segment (p + t r):
     *       t0 = (q − p) · r / (r · r)
     *       t1 = (q + s − p) · r / (r · r) = t0 + s · r / (r · r)
     *    If the interval between t0 and t1 intersects the interval [0, 1] then the line segments are collinear and overlapping; otherwise they are collinear and disjoint.
     *    Note that if s and r point in opposite directions, then s · r < 0 and so the interval to be checked is [t1, t0] rather than [t0, t1].
     * 2. If r × s = 0 and (q − p) × r ≠ 0, then the two lines are parallel and non-intersecting.
     * 3. If r × s ≠ 0 and 0 ≤ t ≤ 1 and 0 ≤ u ≤ 1, the two line segments meet at the point p + t r = q + u s.
     * 4. Otherwise, the two line segments are not parallel but do not intersect.
     */

    float SweptAABBCollision::getRayIntersectionFractionOfFirstRay(const Vector2F& originA, const Vector2F& endA, const Vector2F& originB, const Vector2F& endB)
    {
        // p is originA
        // q is originB
        Vector2F r = endA - originA;
        Vector2F s = endB - originB;

        // q minus p
        Vector2F q_m_p = originB - originA;
        // (q minus p) cross s
        float q_m_p_c_s = q_m_p.cross(s);
        // (q minus p) cross r
        float q_m_p_c_r = q_m_p.cross(r);
        // r cross s
        float r_c_s = r.cross(s);

        if (r_c_s == 0)
        {
            if (q_m_p_c_r == 0)     // co-linear
            {
                float r_d_r = r.dot(r);
                if (r_d_r != 0)
                {
                    float t0 = (originB - originA).dot(r) / r_d_r;
                    float t1 = t0 + s.dot(r) / r_d_r;
                    if ((t0 > 0 && t0 < 1) || (t1 > 0 && t1 < 1))   // co-linear and intersecting
                    {
                        // correct?
                        return t0;
                    }
                }
            }
        }
        else
        {
            float t = q_m_p_c_s / r_c_s;
            float u = q_m_p_c_r / r_c_s;

            if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
            {
                return t;
            }
        }

        // parallel - not intersecting
        // or co-linear and not intersecting
        // or not intersecting
        return std::numeric_limits<float>::infinity();

    }


    bool SweptAABBCollision::getRayIntersectionFraction(const Vector2F& pos, const Vector2F& goal, const RectF& aabb, collision_object& coll_o)
    {
        // end = goal, origin = pos, direction = goal - pos

        // left side
        float min_t = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.tl(), aabb.bl());
        float x;
        Vector2F vNormal = {-1,0};
        //bottom side
        x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.bl(), aabb.br());
        if (x < min_t)
        {
            min_t = x;
            vNormal = {0, 1};
        }
        // right side
        x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.br(), aabb.tr());
        if (x < min_t)
        {
            min_t = x;
            vNormal = {1, 0};
        }
        // top side
        x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.tr(), aabb.tl());
        if (x < min_t)
        {
            min_t = x;
            vNormal = {-1, 0};
        }

        // ok, now we should have found the fractional component along the ray where we collided
        if (min_t != std::numeric_limits<float>::infinity())
        {
            coll_o.ti = min_t;
            coll_o.normal = vNormal;
            return true;
        }

        return false;

    }

/*    bool SweptAABBCollision::detectCollision_(const RectF& meRect, const RectF& otherRect, const Vector2F& goal, collision_object& coll_o)
    {
        coll_o.delta = goal - meRect.tl();
        RectF goalRect = meRect;
        goalRect.x = goal.x;
        goalRect.y = goal.y;

        // TODO check if meRect + goal is correct
        RectF rMink = goalRect.minkowskiDiff(otherRect);

//        bool bOverlaps{false};
//        Vector2F vNormal{0,0};
        coll_o.overlaps = false;
        coll_o.normal = {0,0};

        // Minkowski Magic..
        if (rMink.contains(Vector2F(0,0)))    // intersection of the two rects
        {
            Vector2F vCorner = rMink.getNearestCorner(Vector2F(0,0));
            SizeF intersectionSize = {std::min(meRect.w, std::abs(vCorner.x)), std::min(meRect.h, std::abs(vCorner.y))};
            coll_o.ti = -vCorner.w * vCorner.h;
            coll_o.overlaps = true;
        }
        else
        {
            Vector2F seg_start{0,0};
            float relEntry{0};
            float relExit{0};
            Vector2F entryNormal{0,0};
            Vector2F exitNormal{0,0};

            if (getSegmentIntersectionRel(rMink, seg_start, coll_o.delta, relEntry, relExit, entryNormal, exitNormal))
            {
//            -- item tunnels into other
                if (relEntry < 1
                    && (std::abs(relEntry - relExit >= 1e-10))    //-- special case for rect going through another rect's corner
                    && (0 < relEntry + 1e-10 || 0 == relEntry && relExit > 0))
                {
                    coll_o.ti = relEntry;
                    coll_o.normal = entryNormal;
                    coll_o.overlaps = false;
                }
            }
            else
            {
                return false;
            }
        }

        if (coll_o.overlaps)
        {
            // intersecting and not moving - use minimum displacement vector
            if (coll_o.delta.empty())
            {
                Vector2F vec = rMink.getNearestCorner(Vector2F(0,0));
                if (std::abs(vec.x) < std::abs(vec.y))
                {
                    vec.y = 0;
                }
                else
                {
                    vec.x = 0;
                }

                // no displacement..
                if (vec.empty())
                {
                    return false;
                }

                coll_o.touch = { meRect.x + vec.x, meRect.y + vec.y };
                vec.normalize();
                coll_o.normal = vec;
            }
            else
            {
                // intersecting and moving - move in the opposite direction
                float relEntry1{0};
                float relExit1{0};
                Vector2F entryNormal1{0,0};
                Vector2F exitNormal1{0,0};

                if (getSegmentIntersectionRel(rMink, Vector2F(0,0), coll_o.delta, relEntry1, relExit1, entryNormal1, exitNormal1))
                {
                    coll_o.touch = meRect.tl() + coll_o.delta * relEntry1;
                    coll_o.normal = entryNormal1;
                }
                else
                {
                    return false;
                }

            }
        }
        else    // tunnel
        {
            coll_o.touch = meRect.tl() + coll_o.delta * coll_o.ti;
        }

        return true;
    }
*/
/*
// Liang-Barsky function by Daniel White @ http://www.skytopia.com/project/articles/compsci/clipping.html
// This function inputs 8 numbers, and outputs 4 new numbers (plus a boolean value to say whether the clipped line is drawn at all).
//
//    -- This is a generalized implementation of the liang-barsky algorithm, which also returns
//    -- the normals of the sides where the segment intersects.
//    -- Returns false if the segment never touches the rect
//    -- Notice that normals are only guaranteed to be accurate when initially ti1, ti2 == -math.huge, math.huge
//    local function rect_getSegmentIntersectionIndices(x,y,w,h, x1,y1,x2,y2, ti1,ti2)
    bool SweptAABBCollision::getSegmentIntersectionRel(const RectF& rect, const Vector2F& start_seg, const Vector2F& end_seg,                // input values
                                                       float& relEntry, float& relExit, Vector2F& entryNormal, Vector2F& exitNormal)     // return values
    {
        relEntry = -std::numeric_limits<float>::infinity();
        relExit = std::numeric_limits<float>::infinity();

        Vector2F vDelta = end_seg - start_seg;
        Vector2F vNormal{0,0};

        float p, q, r;

        for (uint8_t side = 0; side < 4; ++side)
        {
            if (side == 0) // left
            {
                vNormal = {-1, 0};
                p = -vDelta.x;
                q = start_seg.x - rect.x;
            }
            else if (side == 1) // right
            {
                vNormal = {1, 0};
                p = vDelta.x;
                q = rect.x + rect.w - start_seg.x;
            }
            else if (side == 2) // top
            {
                vNormal = {0, -1};
                p = -vDelta.y;
                q = start_seg.y - rect.y;
            }
            else // bottom
            {
                vNormal = {0, 1};
                p = vDelta.y;
                q = rect.y + rect.h - start_seg.y;

            }

            // segment is horizontal or vertical
            if (p == 0)
            {
                if (q <= 0)
                {
                    return false;
                }
            }
            else
            {
                r = p / q;
                if (p < 0)
                {
                    if (r > relExit)
                    {
                        return false;
                    }
                    else if (r > relEntry)
                    {
                        relEntry = r;
                        entryNormal = vNormal;
                    }
                }
                else
                {
                    if (r < relEntry)
                    {
                        return false;
                    }
                    else if (r < relExit)
                    {
                        relExit = r;
                        exitNormal = vNormal;
                    }
                }
            }
        }
        return true;
    }
*/

/*
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
*/

/*    void SweptAABBCollision::check(spSprite me, std::vector<collision_object> &collisions)
    {
//        std::vector<spSprite> visitedSprites;
//        visitedSprites.push_back(me);

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
    // get intersection of vector A (i.e. first ray)
/*    float SweptAABBCollision::getRayIntersectionFractionOfFirstRay_(const Vector2F& originA, const Vector2F& endA, const Vector2F& originB, const Vector2F& endB)
    {
        Vector2F r = endA - originA;
        Vector2F s = endB - originB;

        float numerator = (originB - originA).dot(r);
        float denominator = r.dot(s);

        if (numerator == 0 && denominator == 0)
        {
            // the lines are co-linear
            // check if they overlap
            /*return	((originB.x - originA.x < 0) != (originB.x - endA.x < 0) != (endB.x - originA.x < 0) != (endB.x - endA.x < 0)) ||
                    ((originB.y - originA.y < 0) != (originB.y - endA.y < 0) != (endB.y - originA.y < 0) != (endB.y - endA.y < 0));*//*
            return std::numeric_limits<float>::infinity();
        }

        if (denominator == 0)
        {
            // lines are parallel
            return std::numeric_limits<float>::infinity();
        }

        float u = numerator / denominator;
        float t = ((originB - originA).dot(s)) / denominator;
        if ((t >= 0) && (t <= 1) && (u >= 0) && (u <= 1))
        {
//            return originA + (r * t);
            return t;
        }

        return std::numeric_limits<float>::infinity();
    }

*/
}