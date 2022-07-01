// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "src/Vector2T.hpp"

#include <memory>
#include <list>

namespace dang
{
    struct tmx_spriteobject;

    class Tweenable;
    using spTweenable = std::shared_ptr<Tweenable>;

    class MotionObject
    {
    public:
        MotionObject() = default;
        MotionObject(const MotionObject& sp) = default;
        virtual ~MotionObject() = default;

        // vel, acc, gravitiy manipulation
        Vector2F    getVel() const { return _vel; }
        void        setVel(const Vector2F& vel) { _vel = vel; }
        void        setVel(float x, float y) { _vel.x = x; _vel.y = y; }
        void        setVelX(float x) {_vel.x = x; }
        void        setVelY(float y) {_vel.y = y; }
        void        setVelTowardsPoint(const Vector2F& direction);
        void        setVelTowardsPoint(const Vector2F& direction, const float speedx, const float speedy);
        void        setVelRelToRadAngle(const double angle);
        void        setVelRelToRadAngle(const double angle, const float speedx, const float speedy);
        Vector2F    getAcc() { return _acc; }
        void        setAcc(const Vector2F& acc) { _acc = acc; }
        void        setAccX(float x) {_acc.x = x; }
        void        setAccY(float y) {_acc.y = y; }
        Vector2F    getGravity() { return _gravity; }
        void        setGravity(const Vector2F& g) {_gravity = g; };

        virtual void        updateMotion(uint32_t dt);

    private:
        Vector2F     _vel{0,0};
        Vector2F     _acc{0,0};
        Vector2F     _gravity{0,0};

    };

}