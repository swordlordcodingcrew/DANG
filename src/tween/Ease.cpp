// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 25.12.19.
//

#include <cmath>
#include "Ease.h"

namespace dang
{
    Ease::Ease()
    {

    }

    Ease::~Ease()
    {

    }

    /**
     * Default Ease. Equivalent with EaseLinear
     * @param x input value between 0 and 1
     * @return calculated value between 0 and 1
     */
    float Ease::calc(float x)
    {
        return x;
    }

    /**
     * Linear ease
     * @param x input value between 0 and 1
     * @return calculated value between 0 and 1
     */
    float EaseLinear::calc(float x)
    {
        return Ease::calc(x);
    }

    /**
     * Accelerating from zero with x*x
     * @param x input value between 0 and 1
     * @return calculated value between 0 and 1
     */
    float EaseInQuad::calc(float x)
    {
        return x*x;
    }

    /**
     * decelerating to zero with x*x
     * @param x input value between 0 and 1
     * @return calculated value between 0 and 1
     */
    float EaseOutQuad::calc(float x)
    {
        return x * (2 - x);
    }

    /**
     * acceleration until halfway, then deceleration with x*x
     * @param x input value between 0 and 1
     * @return calculated value between 0 and 1
     */
    float EaseInOutQuad::calc(float x)
    {
        return x < 0.5 ? (2 * x * x) : (-1 + (4 - 2 * x) * x);
    }
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
