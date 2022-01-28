// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "../Vector2T.hpp"

namespace dang
{
    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;
    class Wavepoint;

    class TwPosSpline : public Tweenable
    {
    public:
        TwPosSpline() = default;
        TwPosSpline(const TwPosSpline& tw) = default;
        TwPosSpline(const std::vector<Vector2F>& spline_points, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        TwPosSpline(const Wavepoint* wp, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void    init(void* obj) override;
        void    update(void* obj, uint32_t dt) override;

    protected:
        std::vector<Vector2F>   _spline_points;
        std::vector<float>      _spline_length;
        float _length{0};
        float _tension{1};
        constexpr static uint32_t _num_steps{20};

        Vector2F interpolate(const float t, const Vector2F& start, const Vector2F& end, const Vector2F& prev, const Vector2F& next) const;
        void addPoint(const Vector2F& p);
    };

}

