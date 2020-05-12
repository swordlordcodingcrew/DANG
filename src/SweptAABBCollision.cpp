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
        _iteraton = 3;
        _iterate = false;

        while (_iteraton > 0)
        {
            for (const spSprite &me : sprites)
            {
                if (me->_coll_object_type == SweptAABBCollision::COT_RIGID || _handled.count(me) > 0)
                {
                    continue;
                }

                // possible collisions
                std::forward_list<manifold> projected_mfs;
                projectCollisions(me, sprites, projected_mfs);

                while (!projected_mfs.empty())
                {
                    manifold mf = projected_mfs.front();

                    if (mf.overlaps)
                    {
//                        std::cout << "overlap" << std::endl;
                        //me->_pos = me->_last_pos + mf.deltaMe;
                        //me->_pos += mf.deltaMe;
                        //me->setPos(mf.touchMe);
                    }

                    eCollisionResponse cr_me = me->getCollisionResponse(mf.other);
                    eCollisionResponse cr_other = mf.other->getCollisionResponse(me);
                    if (cr_me != CR_NONE && cr_me != CR_CROSS && cr_other != CR_NONE && cr_other != CR_CROSS)
                    {
                        switch (cr_me)
                        {
                            case CR_TOUCH:
                                touch(mf, true);
                                break;
                            case CR_SLIDE:
                                slide(mf, true);
                                break;
                            case CR_BOUNCE:
                                bounce(mf, true);
                                break;
                            default:
                                break;
                        }

                        switch (cr_other)
                        {
                            case CR_TOUCH:
                                touch(mf, false);
                                break;
                            case CR_SLIDE:
                                slide(mf, false);
                                break;
                            case CR_BOUNCE:
                                bounce(mf, false);
                                break;
                            default:
                                break;
                        }
                    }

                    me->collide(mf);
                    mf.other->collide(mf);

                    projected_mfs.pop_front();
                    if (projected_mfs.empty())
                    {
                        // no more collisions, this sprite is handled
                        _handled.insert(me);
                    }
                    else
                    {
                        if (cr_me != CR_NONE && cr_me != CR_CROSS && cr_other != CR_NONE && cr_other != CR_CROSS)
                        {
                            // sprite position has changed which invalidates the remaining manifolds -> one more iteration
                            _iterate = true;
                            //  exit from while-loop
                            break;
                        }
                    }
                }
            }   // for loop

            if (_iterate)
            {
                _iterate = false;
                _iteraton--;
            }
            else
            {
                _iteraton = 0;
            }
        } // while loop
    }

    void SweptAABBCollision::slide(manifold &mf, bool for_me)
    {
        if (for_me)
        {
            if (mf.normalMe.x * mf.me->getPosDelta().x > 0)
            {
                mf.me->_pos.x = mf.touchMe.x;
            }
            else if (mf.normalMe.y * mf.me->getPosDelta().y > 0)
            {
                mf.me->_pos.y = mf.touchMe.y;
            }
        }

        else    // for the other
        {
            if (mf.other->_coll_object_type != COT_RIGID)
            {
                if (mf.normalOther.x * mf.other->getPosDelta().x > 0)
                {
                    mf.other->_pos.x = mf.touchOther.x;
                }
                else if (mf.normalOther.y * mf.other->getPosDelta().y > 0)
                {
                    mf.other->_pos.y = mf.touchOther.y;
                }
            }
        }
    }

    void SweptAABBCollision::touch(manifold &mf, bool for_me)
    {
        if (for_me)
        {
            mf.me->setPos(mf.touchMe);
        }
        else    // for the other
        {
            if (mf.other->_coll_object_type != COT_RIGID)
            {
                mf.other->setPos(mf.touchOther);
            }
        }
    }

    void SweptAABBCollision::bounce(manifold &mf, bool for_me)
    {
        if (for_me)
        {
            if (mf.normalMe.x != 0)
            {
                float d_bounce = mf.me->_pos.x - mf.me->_last_pos.x - mf.deltaMe.x;
                mf.me->_pos.x = mf.touchMe.x - d_bounce;
            }
            else
            {
                float d_bounce = mf.me->_pos.y - mf.me->_last_pos.y - mf.deltaMe.y;
                mf.me->_pos.y = mf.touchMe.y - d_bounce;
            }
        }
        else    // for the other
        {
            if (mf.other->_coll_object_type != COT_RIGID)
            {
                if (mf.normalOther.x != 0)
                {
                    float d_bounce = mf.other->_pos.x - mf.other->_last_pos.x - mf.deltaOther.x;
                    mf.other->_pos.x = mf.touchOther.x - d_bounce;
                }
                else
                {
                    float d_bounce = mf.other->_pos.y - mf.other->_last_pos.y - mf.deltaOther.y;
                    mf.other->_pos.y = mf.touchOther.y - d_bounce;
                }
            }
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

            Vector2F delta = deltaMe - deltaOther;
            RectF rMink = otherRect.minkowskiDiff(meRect);

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


                mf.touchMe = me->_last_pos + mf.deltaMe;
                mf.touchOther = other->_last_pos + mf.deltaOther;

                // ti is not valid in this context since the rect overlap already
                mf.ti = 0;

                mf_list.push_front(mf);
            }
            else
            {
                if (getRayIntersectionFraction(Vector2F(0,0), delta, rMink, mf.ti, mf.normalOther))
                {
                    mf.me = me;
                    mf.other = other;

                    mf.overlaps = false;
                    mf.deltaMe = deltaMe * mf.ti;
                    mf.touchMe = me->_last_pos + mf.deltaMe;
                    mf.normalMe = -mf.normalOther;

                    mf.deltaOther = deltaOther * mf.ti;
                    mf.touchOther = other->_last_pos + mf.deltaOther;

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
                    if (t0 > 0 && t0 < 1)  // co-linear and intersecting
                    {
                        std::cout << "co-linear, ti=" << t0 << std::endl;
                        // correct?
                        return t0;

                    }
                    else if (t1 > 0 && t1 < 1)   // co-linear and intersecting
                    {
                        std::cout << "co-linear, ti=" << t1 << std::endl;
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


}