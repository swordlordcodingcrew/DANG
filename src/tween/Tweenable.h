// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 24.12.19.
//
//#include <cstdint>

#pragma once

#include <cstdint>
#include <32blit.hpp>
#include "Ease.h"

namespace dang
{
    class Tweenable
    {
    public:
        Tweenable();
        Tweenable(std::shared_ptr<void> the_object, uint32_t duration, Ease ease = EaseLinear(), int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        virtual ~Tweenable();

        void            start_tw(uint32_t start_time);
        void            pause_tw(uint32_t start_time);
        void            continue_tw(uint32_t start_time);
        void            finish_tw(bool suppressCB = false);
        void            reset_tw();
        bool            is_tw_finished();
        uint32_t        getState() { return _state;}
        void            setFinishedCallback(std::function<void(void)> finishedCB);

        virtual void            update(uint32_t time) = 0;

        enum tw_state {
            TW_READY,
            TW_RUNNING,
            TW_PAUSED,
            TW_FINISHED
        };

    protected:
        // ugly hack with void*. Should be narrowed with a base class or template class
        std::shared_ptr<void>       _the_object{nullptr};

        uint32_t                    _duration{100};
        Ease                        _ease;
        int32_t                     _loops{1};
        bool                        _alternating{false};
        uint32_t                    _delay{0};
        std::function<void(void)>   _finishedCB{nullptr};

        uint32_t    _start_time{0};
        uint32_t    _pause_time{0};
        uint32_t    _loop{0};
        tw_state    _state{TW_READY};

        float        calc(uint32_t time);
    };
}



