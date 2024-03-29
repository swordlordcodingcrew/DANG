// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "CollisionSolver.hpp"
#include "src/sprite/CollisionObject.hpp"

#include <cassert>
#include <iostream>
#include <src/sprite/SpriteObject.hpp>

namespace dang
{

    void CollisionSolver::addCObject(spCollisionObject co)
    {
        assert(co != nullptr);
        _co_list.push_back(co);
    }

    void CollisionSolver::removeCObject(spCollisionObject &co)
    {
//        printf("CSolver: size of list before removal = %u - ", _co_list.size());
//        assert(std::find(_co_list.begin(), _co_list.end(), co) != _co_list.end());
        co->_remove_from_cs = false;
        _co_list.remove(co);
//        printf("after = %u\n", _co_list.size());
    }

    // solve the collisions
    void CollisionSolver::solve()
    {
//        _handled.clear();
//        _iteration = 3;
//        _iterate = false;

        // prepare the collision objects for collision solving
        auto coit = _co_list.begin();
        while (coit != _co_list.end())
        {
            if ((*coit)->_remove_from_cs)
            {
                (*coit)->_remove_from_cs = false;
                coit = _co_list.erase(coit);
            }
            else
            {
//                (*coit)->_handled = false;
                (*coit)->preSolve();
                coit++;
            }
        }

        for (spCollisionObject& co : _co_list)
        {
            assert(co != nullptr);

            // if collision object type is RIGID no active collision detection is processed
            if (co->_rigid)
            {
                continue;
            }

            bool b_reproject = false;
            uint8_t num_reprojections = 3;  // after 3 times a reprojection break the loop
            _handled.clear();
            projectCollisions(co);

            while (!_projected_mfs.empty() && num_reprojections > 0)
            {
                manifold mf = _projected_mfs.front();
                _projected_mfs.pop_front();

                if (std::find(_handled.begin(), _handled.end(), mf.other.get()) != _handled.end())
                {
                    continue;
                }
//                if (mf.overlaps)
//                {
//                    std::cout << "spr with pos (" << co->_cs_pos.x << "," << co->_cs_pos.y << ") overlaps" << std::endl;
//                }

                uint8_t cr_me = co->getCollisionResponse(mf.other.get());
                uint8_t cr_other = mf.other->getCollisionResponse(co.get());
                // false = 0
                if (!((cr_me & CR_NONE) || (cr_other & CR_NONE)))
                {
                    switch (cr_me)
                    {
                        case CR_TOUCH:
                        {
                            co->_goal = mf.touchMe;
                            break;
                        }
                        case CR_SLIDE:
                        {
                            if (co->_co_pos.x - co->_goal.x != 0 || (co->_co_pos.y - co->_goal.y != 0))
                            {
                                if (mf.normalMe.x != 0)
                                {
                                    co->_goal.x = mf.touchMe.x;
                                }
                                else if (mf.normalMe.y != 0)
                                {
                                    co->_goal.y = mf.touchMe.y;
                                }
                                b_reproject = true;
                            }
                            break;
                        }
                        case CR_BOUNCE:
                        {
                            if (co->_co_pos.x - co->_goal.x != 0 || co->_co_pos.y - co->_goal.y != 0)
                            {
                                if (mf.normalMe.x != 0)
                                {
                                    float d_bounce = co->_goal.x - co->_co_pos.x - mf.deltaMe.x;
                                    co->_goal.x = mf.touchMe.x - d_bounce;
                                }
                                else
                                {
                                    float d_bounce = co->_goal.y - co->_co_pos.y - mf.deltaMe.y;
                                    co->_goal.y = mf.touchMe.y - d_bounce;
                                }
                            }
                            b_reproject = true;
                            break;
                        }
                        case CR_CROSS:
                        default:
                            break;

                    }


                    if (!mf.other->_rigid)
                    {
                        switch (cr_other)
                        {
                            case CR_TOUCH:
                            {
                                mf.other->_goal = mf.touchOther;
                                break;
                            }
                            case CR_SLIDE:
                            {
                                if (mf.other->_co_pos.x - mf.other->_goal.x != 0 || mf.other->_co_pos.y - mf.other->_goal.y != 0)
                                {
                                    if (mf.normalOther.x != 0)
                                    {
                                        mf.other->_goal.x = mf.touchOther.x;
                                    }
                                    else if (mf.normalOther.y != 0)
                                    {
                                        mf.other->_goal.y = mf.touchOther.y;
                                    }
                                }
                                break;
                            }
                            case CR_BOUNCE:
                            {
                                if (mf.other->_co_pos.x - mf.other->_goal.x != 0 || mf.other->_co_pos.y - mf.other->_goal.y != 0)
                                {
                                    if (mf.normalOther.x != 0)
                                    {
                                        float d_bounce = mf.other->_goal.x - mf.other->_co_pos.x - mf.deltaOther.x;
                                        mf.other->_goal.x = mf.touchOther.x - d_bounce;
                                    }
                                    else
                                    {
                                        float d_bounce = mf.other->_goal.y - mf.other->_co_pos.y - mf.deltaOther.y;
                                        mf.other->_goal.y = mf.touchOther.y - d_bounce;
                                    }
                                }
                                break;
                            }
                            case CR_CROSS:
                            default:
                                break;
                        }
                    }

                    co->collide(mf);
                    mf.other->collide(mf);
                }


                if (b_reproject)
                {
                    b_reproject = false;
                    _handled.push_back(mf.other.get());
                    num_reprojections--;
                    _projected_mfs.clear();
                    projectCollisions(co);
                }
            }   // while mf not empty loop (and less than 5 iterations)

            if (num_reprojections == 0)
            {
                std::cout << "max reprojections" << std::endl;
            }

            _projected_mfs.clear();
            _handled.clear();
            co->_co_pos = co->_goal;

        }   // for loop


        for (spCollisionObject& co : _co_list)
        {
            co->postSolve();
        }
    }

    void CollisionSolver::projectCollisions(spCollisionObject& me)
    {
        for (spCollisionObject& other : _co_list)
        {
            if (me == other ||
                me->getCollisionResponse(other.get()) == CR_NONE ||
                other->getCollisionResponse(me.get()) == CR_NONE)
            {
                continue;
            }

            if (std::find(_handled.begin(), _handled.end(), other.get()) != _handled.end())
            {
                continue;
            }


            Vector2F deltaMe = me->_goal - me->_co_pos;
            Vector2F deltaOther = other->_goal - other->_co_pos;
            Vector2F delta = deltaMe - deltaOther;

            RectF rMink = {(other->_co_pos.x + other->_hotrect.x) - (me->_co_pos.x + me->_hotrect.x) - me->_hotrect.w,
                           (other->_co_pos.y + other->_hotrect.y) - (me->_co_pos.y + me->_hotrect.y) - me->_hotrect.h,
                    other->_hotrect.w + me->_hotrect.w,
                    other->_hotrect.h + me->_hotrect.h};

            manifold mf = {};

            if (rMink.containsZeroPoint())    // intersection of the two rects without any velocity
            {
//                std::cout << "overlap" << std::endl;

                mf.me = me;
                mf.other = other;
                mf.overlaps = true;

                // get the nearest side
                Vector2F nearestBound = rMink.getNearestCorner(Vector2F(0,0));

                // actually we are looking for the nearest edge
                if (std::abs(nearestBound.x) < std::abs(nearestBound.y))
                {
                    nearestBound.y = 0;
                    mf.deltaMe.y = 0;
                    mf.deltaOther.y = 0;
                    if (delta.x != 0)
                    {
                        mf.deltaMe.x = nearestBound.x * deltaMe.x / delta.x;
                        mf.deltaOther.x = nearestBound.x * deltaOther.x / delta.x;
                    }
                    else
                    {
                        mf.deltaMe.x = nearestBound.x;
                        mf.deltaOther.x = 0;
                    }
                    mf.normalMe = {(nearestBound.x == rMink.x ? 1.0f : -1.0f), 0};
                    mf.normalOther = -mf.normalMe;
                }
                else
                {
                    nearestBound.x = 0;
                    mf.deltaMe.x = 0;
                    mf.deltaOther.x = 0;
                    if (delta.y != 0)
                    {
                        mf.deltaMe.y = nearestBound.y * deltaMe.y / delta.y;
                        mf.deltaOther.y = nearestBound.y * deltaOther.y / delta.y;
                    }
                    else
                    {
                        mf.deltaMe.y = nearestBound.y;
                        mf.deltaOther.y = 0;
                    }
                    mf.normalMe = {0, (nearestBound.y == rMink.y ? 1.0f : -1.0f)};
                    mf.normalOther = -mf.normalMe;
                }

                mf.touchMe = me->_co_pos + mf.deltaMe;
                mf.touchOther = other->_co_pos + mf.deltaOther;

                // ti is not valid in this context since the rects overlap already
                mf.ti = 0;

                _projected_mfs.push_front(mf);
            }
            else
            {
                if (getRayIntersectionFractionP0(delta, rMink, mf.ti, mf.normalOther))
                {
                    mf.me = me;
                    mf.other = other;

                    mf.overlaps = false;
                    mf.deltaMe = deltaMe * mf.ti;
                    mf.touchMe = me->_co_pos + mf.deltaMe;
                    mf.normalMe = -mf.normalOther;

                    mf.deltaOther = deltaOther * mf.ti;
                    mf.touchOther = other->_co_pos + mf.deltaOther;

                    _projected_mfs.push_front(mf);
                }
            }

        }

        // sort by ti
        _projected_mfs.sort([] (const manifold &first, const manifold &second)
             {
                 return first.ti < second.ti;
             }
        );

    }

    bool CollisionSolver::getRayIntersectionFraction(const Vector2F& pos, const Vector2F& goal, const RectF& aabb, float& ti, Vector2F& normal)
    {
        // end = goal, origin = pos, direction = goal - pos

        // left side
        ti = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.tl(), aabb.bl());
        normal = {-1,0};
        if (ti == 0 && goal.x < 0)
        {
            ti = std::numeric_limits<float>::infinity();
        }
        //bottom side
        float x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.bl(), aabb.br());
        if (x == 0 && goal.y > 0)
        {
            x = std::numeric_limits<float>::infinity();
        }
        if (x < ti)
        {
            ti = x;
            normal = {0, 1};
        }
        // right side
        x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.br(), aabb.tr());
        if (x == 0 && goal.x > 0)
        {
            x = std::numeric_limits<float>::infinity();
        }
        if (x < ti)
        {
            ti = x;
            normal = {1, 0};
        }
        // top side
        x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.tr(), aabb.tl());
        if (x == 0 && goal.y < 0)
        {
            x = std::numeric_limits<float>::infinity();
        }
        if (x < ti)
        {
            ti = x;
            normal = {0, -1};
        }

        // ok, now we should have found the fractional component along the ray where we collided
        return ti != std::numeric_limits<float>::infinity();
    }

    bool CollisionSolver::getRayIntersectionFractionP0(const Vector2F& goal, const RectF& aabb, float& ti, Vector2F& normal)
    {
        // end = goal, origin = pos = 0,0, direction = goal - pos

        // left side
        ti = getRayIntersectionFractionOfFirstRayP0(goal, aabb.tl(), aabb.bl());
        normal = {-1,0};
        if (ti == 0 && goal.x < 0)
        {
            ti = std::numeric_limits<float>::infinity();
        }
        //bottom side
        float x = getRayIntersectionFractionOfFirstRayP0(goal, aabb.bl(), aabb.br());
        if (x == 0 && goal.y > 0)
        {
            x = std::numeric_limits<float>::infinity();
        }
        if (x < ti)
        {
            ti = x;
            normal = {0, 1};
        }
        // right side
        x = getRayIntersectionFractionOfFirstRayP0(goal, aabb.br(), aabb.tr());
        if (x == 0 && goal.x > 0)
        {
            x = std::numeric_limits<float>::infinity();
        }
        if (x < ti)
        {
            ti = x;
            normal = {1, 0};
        }
        // top side
        x = getRayIntersectionFractionOfFirstRayP0(goal, aabb.tr(), aabb.tl());
        if (x == 0 && goal.y < 0)
        {
            x = std::numeric_limits<float>::infinity();
        }
        if (x < ti)
        {
            ti = x;
            normal = {0, -1};
        }

        // ok, now we should have found the fractional component along the ray where we collided
        return ti != std::numeric_limits<float>::infinity();
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

    float CollisionSolver::getRayIntersectionFractionOfFirstRay(const Vector2F& originA, const Vector2F& endA, const Vector2F& originB, const Vector2F& endB)
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
                    if (t0 > 0 && t0 < 1)  // co-linear and intersecting
                    {
//                        std::cout << "co-linear, ti=" << t0 << std::endl;
                        // correct?
                        return t0;

                    }
                    else if (t1 > 0 && t1 < 1)   // co-linear and intersecting
                    {
//                        std::cout << "co-linear, ti=" << t1 << std::endl;
                        // correct?
                        return t1;
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

    float CollisionSolver::getRayIntersectionFractionOfFirstRayP0(const Vector2F& endA, const Vector2F& originB, const Vector2F& endB)
    {
        // p is originA is zero
        // q is originB
        // r is endA - originA which is zero -> r = endA
//        Vector2F r = endA;
        Vector2F s = endB - originB;

        // q minus p
//        Vector2F q_m_p = originB - originA;
//      originA is zero -> q_m_p is originB
//        Vector2F q_m_p = originB;
        // (q minus p) cross s
        float q_m_p_c_s = originB.cross(s);
        // (q minus p) cross r
        float q_m_p_c_r = originB.cross(endA);
        // r cross s
        float r_c_s = endA.cross(s);

        if (r_c_s == 0)
        {
            if (q_m_p_c_r == 0)     // co-linear
            {
                float r_d_r = endA.dot(endA);
                if (r_d_r != 0)
                {
                    float t0 = originB.dot(endA) / r_d_r;
                    float t1 = t0 + s.dot(endA) / r_d_r;
                    if (t0 > 0 && t0 < 1)  // co-linear and intersecting
                    {
//                        std::cout << "co-linear, ti=" << t0 << std::endl;
                        // correct?
                        return t0;

                    }
                    else if (t1 > 0 && t1 < 1)   // co-linear and intersecting
                    {
//                        std::cout << "co-linear, ti=" << t1 << std::endl;
                        // correct?
                        return t1;
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

    /**
     * Axis Aligned Line-of-Sight (aaLoS). The calculation is done on the horizontal axis (the H).
     * as eyes the center of the viewer's hotrect is used
     * @param the viewer
     * @param target the point to be seen or not
     * @return 0 for not visible. Else the distance (>0 visible on the right; <0 visible on the left)
     */
    float CollisionSolver::aaLoSH(const CollisionObject* me, const CollisionObject* target)
    {
        Vector2F me_p = me->_co_pos + me->_hotrect.center();
        RectF target_hr = target->_hotrect;
        target_hr.x += target->_co_pos.x;
        target_hr.y += target->_co_pos.y;

        // target too high / too low, ergo not visible
        if (me_p.y > target_hr.bottom() || me_p.y < target_hr.top())
        {
            return 0;
        }

        float dx_target = std::min(me_p.x - target_hr.left(), me_p.x - target_hr.right());

        for (spCollisionObject& obst : _co_list)
//        for (SpriteIterator it = begin(); it != end(); it++)
        {
            // me and target are not obstacles per se
            if ((me == obst.get())
                || (target == obst.get())
                || (me->getCollisionResponse(obst.get()) == CR_NONE || obst->getCollisionResponse(me) == CR_NONE))
            {
                continue;
            }

/*            if (obst.get() == me || obst.get() == target)
            {
                continue;
            }

            // this is rather a philosophical condition. Might be removed
            if (obst->getCollisionResponse(me) == CR_NONE)
            {
                continue;
            }
*/
            RectF obst_hr = obst->_hotrect;
            obst_hr.x += obst->_co_pos.x;
            obst_hr.y += obst->_co_pos.y;

            // obstacle too high / too low, ergo not an obstacle
            if (me_p.y > obst_hr.bottom() || me_p.y < obst_hr.top())
            {
                continue;
            }

            float dx_obst = std::min(me_p.x - obst_hr.left(), me_p.x - obst_hr.right());

            if (dx_obst * dx_target < 0) // if not the same sign (not both obstacle and target on the left or both on the right), no obstacle
            {
                continue;
            }
            else if (dx_obst < 0) // both on the left side
            {
                if (dx_obst > dx_target)    // negative numbers: distance to obstacle greater than targer -> obstacle
                {
                    return 0;
                }
            }
            else   // both on the rigt side
            {
                if (dx_obst < dx_target)    // distance to obstacle smaller than targer -> obstacle
                {
                    return 0;
                }
            }

        }
        return dx_target;
    }

    /**
     * General Line-of-Sight (LoS).
     * As eyes the center of the viewer's hotrect is used. To keep the algo simple, the destination is also
     * a point (center of hotrect). Like that the calulation is reduced to check if the line segment me - target
     * intersects with another hotrect.
     * See answer of Alejo in https://stackoverflow.com/questions/99353/how-to-test-if-a-line-segment-intersects-an-axis-aligned-rectange-in-2d
     * meaning:
     * The original poster wanted to DETECT an intersection between a line segment and a polygon. There was no need to LOCATE the intersection, if there is one. If that's how you meant it, you can do less work than Liang-Barsky or Cohen-Sutherland:
     *
     * Let the segment endpoints be p1=(x1 y1) and p2=(x2 y2).
     * Let the rectangle's corners be (xBL yBL) and (xTR yTR).
     *
     * Then all you have to do is
     *
     * A. Check if all four corners of the rectangle are on the same side of the line. The implicit equation for a line through p1 and p2 is:
     *
     * F(x y) = (y2-y1)*x + (x1-x2)*y + (x2*y1-x1*y2)
     *
     * If F(x y) = 0, (x y) is ON the line.
     * If F(x y) > 0, (x y) is "above" the line.
     * If F(x y) < 0, (x y) is "below" the line.
     *
     * Substitute all four corners into F(x y). If they're all negative or all positive, there is no intersection. If some are positive and some negative, go to step B.
     *
     * B. Project the endpoint onto the x axis, and check if the segment's shadow intersects the polygon's shadow. Repeat on the y axis:
     *
     * If (x1 > xTR and x2 > xTR), no intersection (line is to right of rectangle).
     * If (x1 < xBL and x2 < xBL), no intersection (line is to left of rectangle).
     * If (y1 > yTR and y2 > yTR), no intersection (line is above rectangle).
     * If (y1 < yBL and y2 < yBL), no intersection (line is below rectangle).
     * else, there is an intersection. Do Cohen-Sutherland or whatever code was mentioned in the other answers to your question.
     *
     * You can, of course, do B first, then A.
     *
     * @param me the viewer (center of hotrect)
     * @param target the point to be seen or not (hotrect)
     * @return 0 for not visible. Else the distance (>0 visible on the right; <0 visible on the left)
     */
    float CollisionSolver::loS(const CollisionObject* me, const CollisionObject* target)
    {
        Vector2F p1 = me->_co_pos + me->_hotrect.center();
        Vector2F p2 = target->_co_pos + target->_hotrect.center();

        for (const spCollisionObject& other : _co_list)
        {
            if (other == nullptr)
            {
                continue;
            }

            if ((me == other.get())
                || (target == other.get())
                || (me->getCollisionResponse(other.get()) == CR_NONE || other->getCollisionResponse(me) == CR_NONE))
            {
                continue;
            }

            RectF r = other->_hotrect;
            r.x += other->_co_pos.x;
            r.y += other->_co_pos.y;
            float f_tl = (p2.y - p1.y) * r.tl().x + (p1.x - p2.x) * r.tl().y + (p2.x *p1.y - p1.x * p2.y);
            float f_tr = (p2.y - p1.y) * r.tr().x + (p1.x - p2.x) * r.tr().y + (p2.x *p1.y - p1.x * p2.y);
            float f_bl = (p2.y - p1.y) * r.bl().x + (p1.x - p2.x) * r.bl().y + (p2.x *p1.y - p1.x * p2.y);
            float f_br = (p2.y - p1.y) * r.br().x + (p1.x - p2.x) * r.br().y + (p2.x *p1.y - p1.x * p2.y);

            if ((f_tl > 0 && f_tr > 0 && f_bl > 0 && f_br > 0) || (f_tl < 0 && f_tr < 0 && f_bl < 0 && f_br < 0))
            {
                // no intersection, go to next sprite
                continue;
            }

            if (p1.x > r.right() && p2.x > r.right()) { continue; }   // no intersection (line is to the right of rectangle).
            if (p1.x < r.left() && p2.x < r.left()) { continue; }   // no intersection (line is to the left of rectangle).
            if (p1.y > r.bottom() && p2.y > r.bottom()) { continue; }   // no intersection (line is below rectangle).
            if (p1.y < r.top() && p2.y < r.top()) { continue; }     // no intersection (line is above rectangle).

            // there is an intersection -> target not visible
            return 0;
        }

        float dist = p1.distance(p2);
        return p2.x >= p1.x ? -dist : dist;

    }

    /**
     * like loS, but the target is not a point but a hotrect
     * algo: see loS
     */
    float CollisionSolver::loSHR(const CollisionObject* me, const CollisionObject* target)
    {
        Vector2F p1 = me->_co_pos + me->_hotrect.center();

        Vector2F pts[4];
        pts[0] = target->_co_pos + target->_hotrect.tl();
        pts[1] = target->_co_pos + target->_hotrect.bl();
        pts[2] = target->_co_pos + target->_hotrect.br();
        pts[3] = target->_co_pos + target->_hotrect.tr();

        std::bitset<4> pts_visible{0b1111};
        for (const spCollisionObject& other : _co_list)
        {
            if (other == nullptr)
            {
                continue;
            }

            if ((me == other.get())
                || (target == other.get())
                || (me->getCollisionResponse(other.get()) == CR_NONE || other->getCollisionResponse(me) == CR_NONE))
            {
                continue;
            }

            RectF r = other->_hotrect;
            r.x += other->_co_pos.x;
            r.y += other->_co_pos.y;

            for (uint8_t i = 0; i < 4; ++i)
            {
                // if the point is still marked as visible, check the visibility
                if (pts_visible[i])
                {
                    Vector2F& pt = pts[i];
                    float f_tl = (pt.y - p1.y) * r.tl().x + (p1.x - pt.x) * r.tl().y + (pt.x *p1.y - p1.x * pt.y);
                    float f_tr = (pt.y - p1.y) * r.tr().x + (p1.x - pt.x) * r.tr().y + (pt.x *p1.y - p1.x * pt.y);
                    float f_bl = (pt.y - p1.y) * r.bl().x + (p1.x - pt.x) * r.bl().y + (pt.x *p1.y - p1.x * pt.y);
                    float f_br = (pt.y - p1.y) * r.br().x + (p1.x - pt.x) * r.br().y + (pt.x *p1.y - p1.x * pt.y);

                    if ((f_tl > 0 && f_tr > 0 && f_bl > 0 && f_br > 0) || (f_tl < 0 && f_tr < 0 && f_bl < 0 && f_br < 0))
                    {
                        // no intersection, go to next point
                        continue;
                    }

                    if (p1.x > r.right() && pt.x > r.right()) { continue; }   // no intersection (line is to the right of rectangle).
                    if (p1.x < r.left() && pt.x < r.left()) { continue; }   // no intersection (line is to the left of rectangle).
                    if (p1.y > r.bottom() && pt.y > r.bottom()) { continue; }   // no intersection (line is below rectangle).
                    if (p1.y < r.top() && pt.y < r.top()) { continue; }     // no intersection (line is above rectangle).

                    // there is an intersection
                    pts_visible[i] = false;

                }
            }

            // intersection of all 4 points -> target not visible
            if (pts_visible.none())
            {
                return 0;
            }
        }

        Vector2F p2 = target->_co_pos + target->_hotrect.center();
        float dist = p1.distance(p2);
        return p2.x >= p1.x ? -dist : dist;

    }

}
