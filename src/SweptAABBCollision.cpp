// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include <iostream>
#include "SweptAABBCollision.h"
#include "Sprite.h"

namespace dang
{

    // called on every move of every sprite
    void SweptAABBCollision::handleCollisionDetection(std::forward_list<spSprite>& sprites)
    {
        _handled.clear();

        for (const spSprite& me : sprites)
        {
            if (me->_coll_object_type == SweptAABBCollision::COT_RIGID)
            {
                continue;
            }

            // possible collisions
            std::forward_list<manifold> projected_mfs;
            projectCollisions(me, sprites, projected_mfs);

            while (!projected_mfs.empty())
            {
                manifold mf = projected_mfs.front();
                //            std::cout << me->_type << " has collision. ti=" << coll_o.ti << std::endl;
                //            std::cout << "pos me=" << me->getPos().x << ", " << me->getPos().y << std::endl;
                //            std::cout << "pos other=" << coll_o.other->getPos().x << ", " << coll_o.other->getPos().y << std::endl;

                if (mf.overlaps)
                {
                    me->setPos(mf.touchMe);
                    //                std::cout << "collision: overlaps" << std::endl;
                }
                else
                {
                    if (me->_coll_response == CR_TOUCH || mf.other->_coll_response == CR_TOUCH)
                    {
                        Vector2F deltaMe = me->getPosDelta();
                        Vector2F deltaOther = mf.other->getPosDelta();

                        me->setPos(me->getLastPos() + deltaMe * mf.ti);
                        if (mf.other->_coll_object_type != COT_RIGID)
                        {
                            mf.other->setPos(me->getLastPos() + deltaOther * mf.ti);
                        }
                    }
                    else if (me->_coll_response == CR_CROSS || mf.other->_coll_response == CR_CROSS)
                    {
                        // do nothing, pos already set
                    }
                    else if (me->_coll_response == CR_SLIDE && mf.other->_coll_response == CR_SLIDE)
                    {
                        if ((mf.normalMe.x > 0 && me->getPosDelta().x > 0) || (mf.normalMe.x < 0 && me->getPosDelta().x < 0))
                        {
                            me->_pos.x = mf.touchMe.x;
                            if (mf.other->_coll_object_type != COT_RIGID)
                            {
                                mf.other->_pos.x = mf.touchOther.x;
                            }
                        }
                        else if ((mf.normalMe.y > 0 && me->getPosDelta().y > 0) || (mf.normalMe.y < 0 && me->getPosDelta().y < 0))
                        {
                            me->_pos.y = mf.touchMe.y;
                            if (mf.other->_coll_object_type != COT_RIGID)
                            {
                                mf.other->_pos.y = mf.touchOther.y;
                            }
                        }

                    }
                    else if (me->_coll_response == CR_BOUNCE && mf.other->_coll_response == CR_BOUNCE)
                    {
                        // TODO: clean bouncing
                        if ((mf.normalMe.x > 0 && me->getPosDelta().x > 0) || (mf.normalMe.x < 0 && me->getPosDelta().x < 0))
                        {
                            me->_pos.x -= (me->_pos.x - mf.touchMe.x);
                            if (mf.other->_coll_object_type != COT_RIGID)
                            {
                                mf.other->_pos.x -= (mf.other->_pos.x - mf.touchOther.x);
                            }
                        }
                        else if ((mf.normalMe.y > 0 && me->getPosDelta().y > 0) || (mf.normalMe.y < 0 && me->getPosDelta().y < 0))
                        {
                            me->_pos.y -= (me->_pos.y - mf.touchMe.y);
                            if (mf.other->_coll_object_type != COT_RIGID)
                            {
                                mf.other->_pos.y -= (mf.other->_pos.y - mf.touchOther.y);
                            }
                        }

                    }
                }

                //            std::cout << "pos me=" << me->getPos().x << ", " << me->getPos().y << std::endl;
                //            std::cout << "pos other=" << coll_o.other->getPos().x << ", " << coll_o.other->getPos().y << std::endl;

                me->collide(mf);
                projected_mfs.pop_front();

            }

            // this sprite is handled
            _handled.insert(me);

        }
    }

    void SweptAABBCollision::projectCollisions(const spSprite& me, const std::forward_list<spSprite>& sprites, std::forward_list<manifold>& mf_list)
    {
        for (const spSprite& other : sprites)
        {
            if ((me == other)
                || (me->_coll_response == CR_NONE || other->_coll_response == CR_NONE)
                || (_handled.count(other) != 0))
            {
                continue;
            }

            manifold mf = {};

            RectF meRect = me->getHotrect();
            meRect.x += me->getLastPos().x;
            meRect.y += me->getLastPos().y;
            Vector2F deltaMe = me->getPosDelta();

            RectF otherRect = other->getHotrect();
            otherRect.x += other->getLastPos().x;
            otherRect.y += other->getLastPos().y;
            Vector2F deltaOther = other->getPosDelta();

            RectF rMink = otherRect.minkowskiDiff(meRect);
//            RectF rMink = meRect.minkowskiDiff(otherRect);

            if (rMink.containsWOBounds(Vector2F(0, 0)))    // intersection of the two rects without any velocity
            {
                // get the nearest side
                Vector2F nearestBound = rMink.getNearestCorner(Vector2F(0,0));

                // actually we are looking for the nearest edge
                if (std::abs(nearestBound.x) < std::abs(nearestBound.y))
                {
                    nearestBound.y = 0;
                }
                else
                {
                    nearestBound.x = 0;
                }

                mf.other = other;
                mf.overlaps = true;
                mf.deltaMe = nearestBound;
                nearestBound.normalize();
                mf.normalMe = nearestBound;
                mf.normalOther = -nearestBound;
                mf.touchMe = meRect.tl() + nearestBound;
//          these variables are not valid in this context since there is no delta movement. All information is given in the *me variables
                mf.ti = 0;
                mf.deltaOther = {0, 0};
                mf.touchOther = {0, 0};

                mf_list.push_front(mf);
            }
            else
            {
                Vector2F delta = deltaMe - deltaOther;
                if (getRayIntersectionFraction(Vector2F(0,0), delta, rMink, mf.ti, mf.normalOther))
                {
                    mf.other = other;

                    mf.overlaps = false;
                    mf.deltaMe = deltaMe * mf.ti;
                    mf.touchMe = meRect.tl() + mf.deltaMe;
                    mf.normalMe = -mf.normalOther;

                    mf.deltaOther = deltaOther * mf.ti;
                    mf.touchOther = otherRect.tl() + mf.deltaOther;

                    mf_list.push_front(mf);
                }
            }

        }

        // sort by ti
        mf_list.sort([] (const manifold &first, const manifold &second)
            {
                return first.ti < second.ti;
            }
        );

    }

    bool SweptAABBCollision::getRayIntersectionFraction(const Vector2F& pos, const Vector2F& goal, const RectF& aabb, float& ti, Vector2F& normal)
    {
        // end = goal, origin = pos, direction = goal - pos

        // left side
        ti = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.tl(), aabb.bl());
        normal = {-1,0};
        //bottom side
        float x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.bl(), aabb.br());
        if (x < ti)
        {
            ti = x;
            normal = {0, 1};
        }
        // right side
        x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.br(), aabb.tr());
        if (x < ti)
        {
            ti = x;
            normal = {1, 0};
        }
        // top side
        x = getRayIntersectionFractionOfFirstRay(pos, goal, aabb.tr(), aabb.tl());
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

}