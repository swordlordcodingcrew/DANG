// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "CollisionSolver.hpp"
#include "RectT.hpp"

namespace dang
{
    class CollisionObject
    {
    public:
        CollisionObject() = default;
        CollisionObject(const CollisionObject& co) = default;
        virtual ~CollisionObject() = default;

        void        remove() { _remove_from_cs = true; };
        void        setRigid(bool rigid) { _rigid = rigid; }
        bool        isRigid() const { return _rigid; }
        void        setGoal(const Vector2F& g) { _goal = g; };
        Vector2F    getCSPosition() const { return _co_pos; };
        void        setCSPosition(const Vector2F& pos) { _co_pos = pos; };

        virtual uint8_t  getCollisionResponse(const CollisionObject* other) const { return _cr; };
        void             setCollisionResponse(uint8_t cr) { _cr = cr; };

        /**
         * this function will be called from the CollisionSOlver before the collision solving starts.
         * It is intended to set _goal.
         */
        virtual void preSolve() = 0;

        /**
         * after collision resolving this function is called by CollisionResolver
         * It is intended to update the position of the CollisionObject represantation.
         */
        virtual void postSolve() = 0;

        virtual void collide(const manifold &mf) = 0;

    protected:
        friend CollisionSolver;

        /**
         * loosely based on box2d
         * rigid: should not move, Rigid object will not collide with other rigid objects. Example: Wall
         * not rigid, i.e. dynamic: moving object. Hero, enemies
         */
        bool                _rigid{false};

        /**
         * collision response
         * based on bump.lua
         */
        uint8_t             _cr{CR_SLIDE};

        /**
         * x, y: global position
         */
        Vector2F            _co_pos{0, 0};

        /**
         * x, y: position of hotrect relative to _co_pos
         * w, h: width and height
         */
        RectF               _hotrect{0,0,0,0};

        /**
         * goal position which will be collision solved
         */
        Vector2F            _goal{0,0};

        bool                _remove_from_cs{false};

    };

}



