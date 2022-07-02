// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "MotionObject.hpp"
#include "src/tween/Tweenable.hpp"

namespace dang
{
    // configures speed so that the sprite targets "target" (aka move directly to)
    /**
     *
     * @param direction target - pos of sprite
     */
    void MotionObject::setVelTowardsPoint(const Vector2F& direction)
    {
        setVelTowardsPoint(direction, _vel.x, _vel.y);
    }

    /**
     *
     * @param direction target - pos of sprite
     * @param speedx
     * @param speedy
     */
    void MotionObject::setVelTowardsPoint(const Vector2F& direction, const float speedx, const float speedy)
    {
        auto angle_rad = atan2(direction.x, direction.y);

        setVelRelToRadAngle(angle_rad, speedx, speedy);
    }

    void MotionObject::setVelRelToRadAngle(const double angle)
    {
        setVelRelToRadAngle(angle, _vel.x, _vel.y);
    }

    void MotionObject::setVelRelToRadAngle(const double angle, const float speedx, const float speedy)
    {
        setVelX(speedx * (float)sin(angle) * 1);
        setVelY(speedy * (float)cos(angle) * 1);
    }

    Vector2F MotionObject::updateMotion(uint32_t dt)
    {
        // dt in 10 ms
        float dt10ms = dt / 100.0f;
        _vel += (_gravity + _acc) * dt10ms;
        return _vel * dt10ms;
    }

}