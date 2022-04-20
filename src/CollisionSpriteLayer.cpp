// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <sstream>
#include <malloc.h>
#include <cassert>
#include "CollisionSpriteLayer.hpp"
#include "Sprite.hpp"
#include "SpriteIterator.hpp"
#include "CollisionSprite.hpp"
#include "Gear.hpp"


#ifdef DANG_DEBUG_DRAW
#ifdef TARGET_32BLIT_HW
#include "32blit.hpp"
#include <malloc.h>
#include "../../../fonts/hud_font_small.h"
extern char _sbss, _end, __ltdc_start;
#endif
#endif

namespace dang
{
    CollisionSpriteLayer::CollisionSpriteLayer()
    {
        _type = LT_COLLISIONSPRITELAYER;
        _root = std::make_shared<CollisionSprite>();
        _root->_visible = false;
        _root->setPos({0,0});
    }

    void CollisionSpriteLayer::addCollisionSprite(spCollisionSprite cspr)
    {
        SpriteLayer::addSprite(cspr);
        cspr->setCSPosition(cspr->local2Global(cspr->getPos()));
//        cspr->setCSPosition(cspr->_pos_g);
        if (cspr->_active)
        {
            _cs.addCObject(cspr);
        }
    }

    void CollisionSpriteLayer::coreUpdate(uint32_t dt, const Gear &gear)
    {
        // update active sprites (tree) or set to inactive if outside active world
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_remove_from_layer)
            {
                sti = erase(sti);
            }
            else
            {
                if ((*sti)->_parent == nullptr)
//                if ((*sti)->_parent.expired())
                {
                    // root element. Always in
                    // but not in collision solver
                    (*sti)->coreUpdate(dt);
                }
                else
                {
                    bool in_active_world = gear.getActiveWorld().intersects((*sti)->getSizeRectG());
                    if (in_active_world)
                    {
                        (*sti)->coreUpdate(dt);
                        if (!(*sti)->_active)
                        {
                            (*sti)->_active = true;
                            spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>((*sti));
                            co->setCSPosition((*sti)->local2Global((*sti)->getPos()));
//                            co->setCSPosition((*sti)->_pos_g);
                            _cs.addCObject(co);
                        }
                    }
                    else if ((*sti)->_active)
                    {
                        (*sti)->coreUpdate(dt);
                        (*sti)->_active = false;
                        spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>((*sti));
                        co->remove();
                    }
                }
                sti++;
            }
        }
    }

    void CollisionSpriteLayer::update(uint32_t dt, const Gear &gear)
    {
        coreUpdate(dt, gear);

        // collision resolution
        _cs.solve();

        // then call update
        for (SpriteIterator it = begin(); it != end(); it++)
        {
            spCollisionSprite cspr = std::dynamic_pointer_cast<CollisionSprite>((*it));
            if (cspr->_active)
            {
                if (cspr->getNTreeState() != nullptr)
                {
                    gear.runNTree(cspr);

#ifdef DANG_DEBUG
                    std::cout << "tree processed with status: " << +static_cast<std::underlying_type_t<dang::NTreeState::internal_state>>(cspr->getNTreeState()->_internal_state) << " and node position: " << cspr->getNTreeState()->_node << std::endl;
#endif
                }

                // call update of sprite
                cspr->update(dt);
            }
        }


    }

    void CollisionSpriteLayer::render(const Gear &gear)
    {
        SpriteLayer::render(gear);

#ifdef DANG_DEBUG_DRAW

    #ifdef TARGET_32BLIT_HW
        // show memory stats
        auto static_used = &_end - &_sbss;
        auto heap_total = &__ltdc_start - &_end;
        auto heap_used = mallinfo().uordblks;

        auto total_ram = static_used + heap_total;

        blit::screen.pen = blit::Pen(255, 0, 255, 255);
        char buf[100];
        snprintf(buf, sizeof(buf), "Mem: %i + %i / %i", static_used, heap_used, total_ram);
        blit::screen.text(buf, hud_font_small, { 5, 5 }, true, blit::TextAlign::top_left);
    #else

        // show amount of sprites and memory used
/*        std::stringstream stats;

        stats << "active: " << _active_sprites.size() << " inactive: " << _inactive_sprites.size() << " heap: " << mallinfo().uordblks;
        blit::screen.text(stats.str(), hud_font_small, { 5, 5 }, true, blit::TextAlign::top_left);

        if (_dbg_mem < mallinfo().uordblks)
        {
            _dbg_mem = mallinfo().uordblks;
            std::cout << "CSL.render() " << stats.str() << std::endl;
        }
*/
        // show hotrects
        RectF vp = gear.getViewport();

        blit::screen.pen = blit::Pen(0, 0, 255, 255);
        for (SpriteIterator it = begin(); it != end(); it++)
//        for (std::shared_ptr<Sprite>& spr : _active_sprites)
        {
            spCollisionSprite cspr = std::static_pointer_cast<CollisionSprite>(*it);
            RectF dr = vp.intersection(cspr->getSizeRect());

            if (dr.area() != 0)
            {
                RectF hr = cspr->getHotrectG();
                hr.x -= vp.tl().x;
                hr.y -= vp.tl().y;

                blit::Point tl(int32_t(hr.tl().x), int32_t(hr.tl().y));
                blit::Point bl(int32_t(hr.bl().x), int32_t(hr.bl().y));
                blit::Point br(int32_t(hr.br().x), int32_t(hr.br().y));
                blit::Point tr(int32_t(hr.tr().x), int32_t(hr.tr().y));

                blit::screen.line(tl, bl); // left -> bottom
                blit::screen.line(bl, br); // bottom -> right
                blit::screen.line(br, tr); // right -> top
                blit::screen.line(tr, tl); // top -> left
            }
        }
        #endif
#endif
    }

    void CollisionSpriteLayer::cleanSpritelist()
    {
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_remove_from_layer)
            {
                spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>(*sti);
                _cs.removeCObject(co);
                sti = erase(sti);
            }
            else
            {
                sti++;
            }
        }
    }

    void CollisionSpriteLayer::_removeSprite(spSprite s)
    {
        assert(s != nullptr);
        s->removeMeFromTree();
        spCollisionObject co = std::dynamic_pointer_cast<CollisionObject>(s);
        _cs.removeCObject(co);
    }

    float CollisionSpriteLayer::aaLoSH(const spCollisionSprite& me, const spCollisionSprite& target)
    {
        return _cs.aaLoSH(std::static_pointer_cast<CollisionObject>(me), std::static_pointer_cast<CollisionObject>(target));
    }

    float CollisionSpriteLayer::loS(const spCollisionSprite& me, const spCollisionSprite& target)
    {
        return _cs.loS(std::static_pointer_cast<CollisionObject>(me), std::static_pointer_cast<CollisionObject>(target));
    }

    // called on every move of every sprite
/*    void CollisionSpriteLayer::handleCollisionDetection(const Gear& gear)
    {
        _handled.clear();
        _iteration = 3;
        _iterate = false;

        while (_iteration > 0)
        {
            for (SpriteIterator it = begin(); it != end(); it++)
            {
                const spCollisionSprite me = std::dynamic_pointer_cast<CollisionSprite>((*it));

                // pointer anomaly, should not happen (but who knows..)
                if (me == nullptr)
                {
                    continue;
                }

                if (!me->_active)
                {
                    continue;
                }

                // if collisions sprite type RIGID no active collision detection is processed or sprite is already handled
                if (me->getCOType() == CollisionSpriteLayer::COT_RIGID || _handled.count(me) > 0)
                {
                    continue;
                }

                // find possible collisions
                std::forward_list<manifold> projected_mfs;
                projectCollisions(me, projected_mfs);

                while (!projected_mfs.empty())
                {
                    manifold mf = projected_mfs.front();

#ifdef DANG_DEBUG
                    if (mf.overlaps)
                    {
                        std::cout << "overlap" << std::endl;
                    }
#endif
                    eCollisionResponse cr_me = me->getCollisionResponse(mf.other);
                    eCollisionResponse cr_other = mf.other->getCollisionResponse(me);
                    if (cr_me != CR_NONE && cr_me != CR_CROSS && cr_other != CR_NONE && cr_other != CR_CROSS)
                    {
                        switch (cr_me)
                        {
                            case CR_TOUCH:
                                me->touch(mf);
                                break;
                            case CR_SLIDE:
                                me->slide(mf);
                                break;
                            case CR_BOUNCE:
                                me->bounce(mf);
                                break;
                            default:
                                break;
                        }

                        switch (cr_other)
                        {
                            case CR_TOUCH:
                                mf.other->touch(mf);
                                break;
                            case CR_SLIDE:
                                mf.other->slide(mf);
                                break;
                            case CR_BOUNCE:
                                mf.other->bounce(mf);
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
                _iteration--;
            }
            else
            {
                _iteration = 0;
            }
        } // while loop
    }
*/
/*    void CollisionSpriteLayer::slide(manifold &mf, bool for_me)
    {
        if (for_me)
        {
            if (mf.normalMe.x * mf.me->getPosDelta().x > 0)
            {
                mf.me->setPosX(mf.touchMe.x);
            }
            else if (mf.normalMe.y * mf.me->getPosDelta().y > 0)
            {
                mf.me->setPosY(mf.touchMe.y);
            }
        }

        else    // for the other
        {
            if (mf.other->getCOType() != COT_RIGID)
            {
                if (mf.normalOther.x * mf.other->getPosDelta().x > 0)
                {
                    mf.other->setPosX(mf.touchOther.x);
                }
                else if (mf.normalOther.y * mf.other->getPosDelta().y > 0)
                {
                    mf.other->setPosY(mf.touchOther.y);
                }
            }
        }
    }

    void CollisionSpriteLayer::touch(manifold &mf, bool for_me)
    {
        if (for_me)
        {
            mf.me->setPos(mf.touchMe);
        }
        else    // for the other
        {
            if (mf.other->getCOType() != COT_RIGID)
            {
                mf.other->setPos(mf.touchOther);
            }
        }
    }

    void CollisionSpriteLayer::bounce(manifold &mf, bool for_me)
    {
        if (for_me)
        {
            if (mf.normalMe.x != 0)
            {
                float d_bounce = mf.me->getPosX() - mf.me->getLastPosX() - mf.deltaMe.x;
                mf.me->setPosX(mf.touchMe.x - d_bounce);
            }
            else
            {
                float d_bounce = mf.me->getPosY() - mf.me->getLastPosY() - mf.deltaMe.y;
                mf.me->setPosY(mf.touchMe.y - d_bounce);
            }
        }
        else    // for the other
        {
            if (mf.other->getCOType() != COT_RIGID)
            {
                if (mf.normalOther.x != 0)
                {
                    float d_bounce = mf.other->getPosX() - mf.other->getLastPosX() - mf.deltaOther.x;
                    mf.other->setPosX(mf.touchOther.x - d_bounce);
                }
                else
                {
                    float d_bounce = mf.other->getPosY() - mf.other->getLastPosY() - mf.deltaOther.y;
                    mf.other->setPosY(mf.touchOther.y - d_bounce);
                }
            }
        }
    }
*/

/*    void CollisionSpriteLayer::projectCollisions(const spCollisionSprite& me, std::forward_list<manifold>& mf_list)
    {
        for (SpriteIterator it = begin(); it != end(); it++)
        {
            const spCollisionSprite other = std::dynamic_pointer_cast<CollisionSprite>((*it));

            if (other == nullptr)
            {
                continue;
            }

            if ((me == other)
                || (me->getCollisionResponse(other) == CR_NONE || other->getCollisionResponse(me) == CR_NONE)
                || (_handled.count(other) != 0))
            {
                continue;
            }

            manifold mf = {};

            RectF meRect = me->getHotrect();
//            meRect.x += me->getLastPos().x;
//            meRect.y += me->getLastPos().y;
//            meRect.x += me->getLastPosGX();
//            meRect.y += me->getLastPosGY();
            meRect.addToPos(me->getLastPosG());
//            Vector2F deltaMe = me->getPosDelta();
            Vector2F deltaMe = me->getPosG() - me->getLastPosG();

            RectF otherRect = other->getHotrect();
//            otherRect.x += other->getLastPos().x;
//            otherRect.y += other->getLastPos().y;
//            otherRect.x += other->getLastPosGX();
//            otherRect.y += other->getLastPosGY();
            otherRect.addToPos(other->getLastPosG());
//            Vector2F deltaOther = other->getPosDelta();
            Vector2F deltaOther = other->getPosG() - other->getLastPosG();

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


//                mf.touchMe = me->getLastPos() + mf.deltaMe;
//                mf.touchOther = other->getLastPos() + mf.deltaOther;
                mf.touchMe = me->getLastPosG() + mf.deltaMe;
                mf.touchOther = other->getLastPosG() + mf.deltaOther;

                // ti is not valid in this context since the rects overlap already
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
//                    mf.touchMe = me->getLastPos() + mf.deltaMe;
                    mf.touchMe = me->getLastPosG() + mf.deltaMe;
                    mf.normalMe = -mf.normalOther;

                    mf.deltaOther = deltaOther * mf.ti;
//                    mf.touchOther = other->getLastPos() + mf.deltaOther;
                    mf.touchOther = other->getLastPosG() + mf.deltaOther;

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
*/
/*    bool CollisionSpriteLayer::getRayIntersectionFraction(const Vector2F& pos, const Vector2F& goal, const RectF& aabb, float& ti, Vector2F& normal)
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
*/
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

/*    float CollisionSpriteLayer::getRayIntersectionFractionOfFirstRay(const Vector2F& originA, const Vector2F& endA, const Vector2F& originB, const Vector2F& endB)
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
*/
    /**
     * Axis Aligned Line-of-Sight (aaLoS). The calculation is done on the horizontal axis (the H).
     * as eyes the center of the viewer's hotrect is used
     * @param the viewer
     * @param target the point to be seen or not
     * @return 0 for not visible. Else the distance (>0 visible on the right; <0 visible on the left)
     */
/*    float CollisionSpriteLayer::aaLoSH(const spCollisionSprite me, const spCollisionSprite target)
    {
        Vector2F me_p = me->getHotrectG().center();
        RectF target_hr = target->getHotrectG();

        // target too high / too low, ergo not visible
        if (me_p.y > target_hr.bottom() || me_p.y < target_hr.top())
        {
            return 0;
        }

        float dx_target = std::min(me_p.x - target_hr.left(), me_p.x - target_hr.right());

        for (SpriteIterator it = begin(); it != end(); it++)
        {
            const spCollisionSprite obst = std::dynamic_pointer_cast<CollisionSprite>(*it);

            // me and target are not obstacles per se
            if (obst == me || obst == target)
            {
                continue;
            }

            // this is rather a philosophical condition. Might be removed
            if (obst->getCollisionResponse(me) == eCollisionResponse::CR_NONE)
            {
                continue;
            }

            // obstacle too high / too low, ergo not an obstacle
            if (me_p.y > obst->getHotrectG().bottom() || me_p.y < obst->getHotrectG().top())
            {
                continue;
            }

            float dx_obst = std::min(me_p.x - obst->getHotrectG().left(), me_p.x - obst->getHotrectG().right());

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
*/
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
/*    float CollisionSpriteLayer::loS(const spCollisionSprite me, const spCollisionSprite target)
    {
        Vector2F p1 = me->getHotrectG().center();
        Vector2F p2 = target->getHotrectG().center();

        for (SpriteIterator it = begin(); it != end(); it++)
        {
            const spCollisionSprite other = std::dynamic_pointer_cast<CollisionSprite>(*it);

            if (other == nullptr)
            {
                continue;
            }

            if ((me == other)
                || (target == other)
                || (me->getCollisionResponse(other) == CR_NONE || other->getCollisionResponse(me) == CR_NONE))
            {
                continue;
            }

            RectF r = other->getHotrectG();
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
*/

}