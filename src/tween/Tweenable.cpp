// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 24.12.19.
//

#include <assert.h>
#include <iostream>
#include "Tweenable.h"

namespace dang
{
    Tweenable::Tweenable() : _the_object(nullptr), _loops(1), _alternating(false), _duration(100), _delay(0), _ease(EaseLinear())
    {
    }

    Tweenable::~Tweenable()
    {
    }

    Tweenable::Tweenable(std::shared_ptr<void> the_object, uint32_t duration, Ease ease, int32_t loops, bool alternating, uint32_t delay) : _the_object(the_object), _duration(duration), _ease(ease), _loops(loops), _alternating(alternating), _delay(delay)
    {
    }

    void Tweenable::pause_tw(uint32_t time)
    {
        _state = TW_PAUSED;
        _pause_time = time;
    }

    void Tweenable::continue_tw(uint32_t time)
    {
        if (_state == TW_FINISHED || _state == TW_RUNNING) return;

        _start_time += time - _pause_time;
        _state = TW_RUNNING;
    }

    void Tweenable::start_tw(uint32_t start_time)
    {
        if (_state == TW_READY)
        {
            _start_time = start_time;
            _pause_time = 0;
            _loop = 0;
            _state = TW_RUNNING;
        }
    }

    void Tweenable::reset_tw()
    {
        _start_time = 0;
        _pause_time = 0;
        _loop = 0;
        _state = TW_READY;

    }

    void Tweenable::finish_tw(bool suppressCB)
    {
        _state = TW_FINISHED;
        if (!suppressCB && _finishedCB != nullptr)
        {
            _finishedCB();
        }

    }

    bool Tweenable::is_tw_finished()
    {
        return _state == TW_FINISHED;
    }

    float Tweenable::calc(uint32_t time)
    {
        if (_state == TW_FINISHED)
        {
            return _ease.calc(_alternating && _loop % 2 == 1 ? 0 : 1);
        }

        if (_state == TW_READY)
        {
            start_tw(time);
        }

        uint32_t st = _start_time;

        if (_state == TW_PAUSED)
        {
            st += (time - _pause_time);
        }

        // during delay
        if (time < st + _delay)
        {
            return _ease.calc(_alternating && _loop % 2 == 1 ? 1 : 0);
        }

        // during ease
        if (time < st + _delay + _duration)
        {
            float x = (time - (st + _delay)) / (float)_duration;
//            std::cout << "x=" << x << " , _start_time=" << st << " , time=" << time << std::endl;
            assert(x >= 0 || x <= 1);

            return _ease.calc(_alternating && _loop % 2 == 1 ? 1 - x : x);

        }
        else    // ease finished
        {
            if (_loops < 0)     // endless loops
            {
                _loop = (_loop + 1) % 2;
                _start_time = time;
            }
            else
            {
                if (_loop + 1 < _loops)
                {
                    _loop++;
                    _start_time = time;
                    return _ease.calc(_alternating && _loop % 2 == 1 ? 1 : 0);
                }
                else    // tween finished
                {
                    _state = TW_FINISHED;
                    if (_finishedCB != nullptr)
                    {
                        _finishedCB();
                    }
                    return _ease.calc(_alternating && _loop % 2 == 1 ? 0 : 1);
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
