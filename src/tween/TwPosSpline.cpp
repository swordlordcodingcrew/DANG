// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "TwPosSpline.h"
#include "sprite/SpriteObject.hpp"
#include "path/Wavepoint.hpp"

#include <cassert>

namespace dang
{
    /**
     * @param spline_points first point is position of the sprite, last point is the point the sprite moves to
     * @param duration duration of loop.
     * @param ease_cb ease function
     * @param loops number of loops
     * @param alternating if true, the animation will reverse for every second loop
     * @param delay delay until loop shall start. Is applied for each loop
     */
    TwPosSpline::TwPosSpline(const std::vector<Vector2F> &spline_points, uint32_t duration, EaseFn ease_cb, int32_t loops, bool alternating, uint32_t delay)
        : Tweenable(duration, ease_cb, loops, alternating, delay)
    {
        assert(spline_points.size() > 1);
        for (const auto& p : spline_points)
        {
            addPoint(p);
        }
    }

    TwPosSpline::TwPosSpline(const Wavepoint *wp, uint32_t duration, EaseFn ease_cb, int32_t loops, bool alternating, uint32_t delay)
            : Tweenable(duration, ease_cb, loops, alternating, delay)
    {
        assert(wp != nullptr);
        assert(wp->next() != nullptr);

        addPoint(wp->pos());
        while (!wp->endPoint())
        {
            wp = wp->next();
            addPoint(wp->pos());
        }
    }

    void TwPosSpline::init(void* obj)
    {
        assert(obj != nullptr);
        SpriteObject* spr = static_cast<SpriteObject*>(obj);
        spr->setPos(_spline_points[0]);
    }

    /**
     * This function updates pos of the sprite
     * Spline-algo: https://andrewhungblog.wordpress.com/2017/03/03/catmull-rom-splines-in-plain-english/
     *
     * @param time needed for updating the tween
     */
    void TwPosSpline::update(void* obj, uint32_t dt)
    {
        assert(obj != nullptr);
        SpriteObject* spr = static_cast<SpriteObject*>(obj);

        float fx = calc(dt);

        float dist = fx * _length;

        size_t i{1};
        for (i = 1; i < _spline_length.size(); ++i)
        {
            if (_spline_length[i] >= dist)
            {
                break;
            }
        }
        float t = (dist - _spline_length[i-1]) / (_spline_length[i] - _spline_length[i-1]);

//        assert(t >= 0 && t <= 1);
        if (t < 0 || t > 1)
        {
            D_DEBUG_PRINT("t=%i\n");
        }
        Vector2F prev = (i - 1 == 0 ? _spline_points[i] : _spline_points[i - 2]);
        Vector2F start = _spline_points[i - 1];
        Vector2F end = _spline_points[i];
        Vector2F next = (i == _spline_points.size() - 1 ? _spline_points[i] : _spline_points[i + 1]);

        Vector2F p = interpolate(t, start, end, prev, next);
        spr->setPos(p);
    }

    Vector2F TwPosSpline::interpolate(const float t, const Vector2F &start, const Vector2F &end, const Vector2F &prev,
                                      const Vector2F &next) const
    {
        float tSquared = t * t;
        float tCubed = tSquared * t;

        Vector2F interpolatedPoint =
                (-.5f * _tension * tCubed + _tension * tSquared - .5f * _tension * t) * prev +
                (1 + .5f * tSquared * (_tension - 6) + .5f * tCubed * (4 - _tension)) * start +
                (.5f * tCubed * (_tension - 4) + .5f * _tension * t - (_tension - 3) * tSquared) * end +
                (-.5f * _tension * tSquared + .5f * _tension * tCubed) * next;

        return interpolatedPoint;
    }

    void TwPosSpline::addPoint(const Vector2F &p)
    {
        if (_spline_points.empty())
        {
            _spline_points.push_back(p);
            _spline_length.push_back(0);
        }
        else
        {
            _spline_points.push_back(p);

            size_t i = _spline_points.size() - 2;

            Vector2F prev = (i == 0 ? _spline_points[i] : _spline_points[i - 1]);
            Vector2F start = _spline_points[i];
            Vector2F end = _spline_points[i + 1];
            Vector2F next = (i == _spline_points.size() - 2 ? _spline_points[i + 1] : _spline_points[i + 2]);

            Vector2F p1 = start;
            Vector2F p2{0,0};
            float seg_length{0};

            // calculate length
            for (size_t ti = 1; ti < _num_steps; ++ti)
            {
                float t = float(ti)/float(_num_steps);
                p2 = interpolate(t, start, end, prev, next);
                seg_length += p1.distance(p2);
                p2 = p1;
            }

            _length += seg_length;
            _spline_length.push_back(_length);

        }
    }

}

