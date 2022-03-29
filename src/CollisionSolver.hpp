// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

/**
 * The collision detection algorithm was inspired largely by the following sources
 * ===========================================================================
 * - https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/565282#565282
 *   post of Gareth
 * - https://gist.github.com/hamaluik/e69f96e253a190273bf0
 *   AABB.hx of Kenton Hamaluik
 * - https://github.com/kikito/bump.lua
 *   bump.lua, copyright (c) 2014 Enrique García Cota
 * - https://box2d.org/
 *   (c) by Erin Catto
*/

#pragma once

#include "Vector2T.hpp"
#include "RectT.hpp"

#include <list>
#include <forward_list>
//#include <unordered_set>
#include <cstdint>
#include <memory>

namespace dang
{
    /**
     * forward declarations
     */
    class CollisionObject;
    using spCollisionObject = std::shared_ptr<CollisionObject>;

    /**
     * collision object
     */
    struct manifold
    {
        bool overlaps;      // true if sprites overlap even before moving. N.B. ti is in that case not valid
        float ti;           // between 0 and 1. How far along the delta to the goal did the collision occur

        spCollisionObject me;     // the sprite
        Vector2F deltaMe;     // relative movement of sprite me
        Vector2F normalMe;  // collision vector for sprite me; -1,0 or 1 in `x` and `y`
        Vector2F touchMe;   // touch position for sprite me

        spCollisionObject other;     // the other sprite
        Vector2F deltaOther;     // relative movement of sprite me
        Vector2F normalOther;   // collision vector for sprite other; -1,0 or 1 in `x` and `y`
        Vector2F touchOther;    // touch position for sprite other

    };


    /**
    * Collision response
    * how a collision shall be resolved
    */
    enum : uint8_t
    {
        CR_NONE = 0x01,
        CR_TOUCH = 0x02,
        CR_CROSS = 0x04,
        CR_SLIDE = 0x08,
        CR_BOUNCE = 0x10
    };

    class CollisionSolver
    {
    public:
        CollisionSolver() = default;
        CollisionSolver(CollisionSolver& cs) = delete;
        ~CollisionSolver() = default;

        void addCObject(spCollisionObject co);

        void removeCObject(spCollisionObject& co);

        void    solve();
        float   aaLoSH(const spCollisionObject& me, const spCollisionObject& target);
        float   loS(const spCollisionObject& me, const spCollisionObject& target);



    protected:
        void    projectCollisions(spCollisionObject& me);
        bool    getRayIntersectionFraction(const Vector2F& origin, const Vector2F& direction, const RectF& aabb, float& ti, Vector2F& normal);
        float   getRayIntersectionFractionOfFirstRay(const Vector2F &originA, const Vector2F &endA, const Vector2F &originB, const Vector2F &endB);

        std::forward_list<manifold>             _projected_mfs;
        std::list<const CollisionObject*>       _handled;       // used in solve() function

//        std::unordered_set<CollisionObject*>    _handled;
        std::list<spCollisionObject>              _co_list;
//        uint16_t                                _iteration{3};       // number of collision solving cycles
//        bool                                    _iterate{false};    // internal use


    };

}