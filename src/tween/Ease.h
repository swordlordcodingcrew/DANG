// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <vector>

namespace dang
{
    class Ease
    {
    public:
        Ease();
        virtual ~Ease();
        virtual float calc(float x) = 0;
        virtual Ease* clone() const = 0;
    };

    class EaseLinear : public Ease
    {
    public:
        virtual float calc(float x);
        virtual EaseLinear* clone() const override;
    };

    class EaseInQuad : public Ease
    {
    public:
        float calc(float x);
        virtual EaseInQuad* clone() const override;
    };

    class EaseOutQuad : public Ease
    {
    public:
        float calc(float x);
        virtual EaseOutQuad* clone() const override;
    };

    class EaseInOutQuad : public Ease
    {
    public:
        float calc(float x);
        virtual EaseInOutQuad* clone() const override;
    };

    class EaseStep : public Ease
    {
    public:
        EaseStep(const std::vector<uint32_t>& steps_ms);
        float calc(float x);
        virtual EaseStep* clone() const override;
    protected:
        std::vector<float> _steps;
        std::vector<float> _values;
        uint32_t _pos{0};
    };
}

/*
  // no easing, no acceleration
  linear: function (t) { return t },
  // accelerating from zero velocity
  easeInQuad: function (t) { return t*t },
  // decelerating to zero velocity
  easeOutQuad: function (t) { return t*(2-t) },
  // acceleration until halfway, then deceleration
  easeInOutQuad: function (t) { return t<.5 ? 2*t*t : -1+(4-2*t)*t },
  // accelerating from zero velocity

  easeInCubic: function (t) { return t*t*t },
  // decelerating to zero velocity
  easeOutCubic: function (t) { return (--t)*t*t+1 },
  // acceleration until halfway, then deceleration
  easeInOutCubic: function (t) { return t<.5 ? 4*t*t*t : (t-1)*(2*t-2)*(2*t-2)+1 },
  // accelerating from zero velocity

  easeInQuart: function (t) { return t*t*t*t },
  // decelerating to zero velocity
  easeOutQuart: function (t) { return 1-(--t)*t*t*t },
  // acceleration until halfway, then deceleration
  easeInOutQuart: function (t) { return t<.5 ? 8*t*t*t*t : 1-8*(--t)*t*t*t },
  // accelerating from zero velocity

  easeInQuint: function (t) { return t*t*t*t*t },
  // decelerating to zero velocity
  easeOutQuint: function (t) { return 1+(--t)*t*t*t*t },
  // acceleration until halfway, then deceleration
  easeInOutQuint: function (t) { return t<.5 ? 16*t*t*t*t*t : 1+16*(--t)*t*t*t*t }


  function easeOutElastic(t) {
    var p = 0.3;
    return Math.pow(2,-10*t) * Math.sin((t-p/4)*(2*Math.PI)/p) + 1;
}
 */