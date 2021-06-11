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
                : _duration(tw._duration), _loops(tw._loops), _alternating(tw._alternating),
                _delay(tw._delay), _loop(tw._loop), _finished(tw._finished), _ease_cb(tw._ease_cb)
    {
        // the callback is not copied since it may point to a function in an potentially deleted object.
        _finishedCB = nullptr;
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
                        // WARNING: Sometimes it would make sense to kill the Callback here. Especially in
                        // use-once scenarios where the callback and the tweenable as a whole only is needed
                        // for a single animation or something.
                        // But this has the side effect, that recurring animations wont probably play anymore
                        // since the call next animation in the callback won't be triggered again.
                        // Just as a warning to the future-me for the case I am having memory leaks because
                        // Sprites won't be removed since there is some shared pointer somewhere in a tweenable...
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
