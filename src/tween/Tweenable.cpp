// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 24.12.19.
//

#include <assert.h>
#include <iostream>
#include "Tweenable.hpp"

namespace dang
{
    Tweenable::Tweenable()
    {
    }

    Tweenable::Tweenable(const Tweenable &tw)
                : _the_object(tw._the_object), _duration(tw._duration), _loops(tw._loops), _alternating(tw._alternating),
                _delay(tw._delay), _finishedCB(tw._finishedCB), _loop(tw._loop), _finished(tw._finished), _ease_cb(tw._ease_cb)
    {
    }

    Tweenable::~Tweenable()
    {
    }

    Tweenable::Tweenable(uint32_t duration, EaseFn& ease_cb, int32_t loops, bool alternating, uint32_t delay)
                : _duration(duration), _loops(loops), _alternating(alternating), _delay(delay), _ease_cb(ease_cb)
    {
    }

    void Tweenable::reset()
    {
        _loop = 0;
        _progress = 0;
        _finished = false;
    }

    void Tweenable::finish(bool suppressCB)
    {
        _finished = true;
        if (!suppressCB && _finishedCB != nullptr)
        {
            _finishedCB();
        }
    }

    bool Tweenable::isFinished()
    {
        return _finished;
    }

    float Tweenable::calc(uint32_t dt)
    {
        assert(_ease_cb != nullptr);

        if (_finished)
        {
            return _ease_cb(_alternating && _loop % 2 == 1 ? 0 : 1);
        }

        _progress += dt;

        // during delay
        if (_progress < _delay)
        {
            return _ease_cb(_alternating && _loop % 2 == 1 ? 1 : 0);
        }

        // during ease
        if (_progress < _delay + _duration)
        {
            float x = (_progress - _delay) / (float)_duration;
//            std::cout << "x=" << x << " , _start_time=" << st << " , time=" << time << std::endl;
            assert(x >= 0 || x <= 1);

            return (_alternating && _loop % 2 == 1 ? 1 - x : x);
        }
        else    // ease finished
        {
            if (_loops < 0)     // endless loops
            {
                _loop = (_loop + 1) % 2;
                _progress = 0;
            }
            else
            {
                if (_loop + 1 < _loops)     // next loop
                {
                    _loop++;
                    _progress = 0;
                    return _ease_cb(_alternating && _loop % 2 == 1 ? 1 : 0);
                }
                else    // tween finished
                {
                    _finished = true;
                    if (_finishedCB != nullptr)
                    {
                        _finishedCB();
                    }
                    return _ease_cb(_alternating && _loop % 2 == 1 ? 0 : 1);
                }
            }
        }

        return 0;
    }

    void Tweenable::setFinishedCallback(std::function<void(void)> finishedCB)
    {
        _finishedCB = finishedCB;
    }

}
