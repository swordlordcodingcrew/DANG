// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "../dang.hpp"
#include <cstdint>
#include <memory>
#include <functional>
#include "Ease.hpp"
//#include <cstdint>


namespace dang
{
    class Tweenable
    {
    public:
        Tweenable();
        Tweenable(const Tweenable& tw);
        Tweenable(uint32_t duration, EaseFn& ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        virtual ~Tweenable();

        virtual void        setObject(std::weak_ptr<void> obj) {_the_object = obj; }
        virtual void        clearObject() {_the_object.reset(); }
        virtual void        duration(uint32_t d) {_duration = d; }
        virtual void        loops(int32_t l) { _loops = l; }
        virtual void        alternating(bool a) { _alternating = a; }
        virtual void        delay(uint32_t d) {_delay = d; }


        virtual void        finish(bool suppressCB = false);
        virtual void        reset();
        virtual bool        isFinished();
        virtual void        setFinishedCallback(std::function<void(void)> finishedCB);

        virtual void            update(uint32_t dt) = 0;

    protected:
        // ugly hack with void*. Should be narrowed with a base class or template class
        std::weak_ptr<void>       _the_object;

        EaseFn&                     _ease_cb = Ease::Linear;
//        std::unique_ptr<Ease>       __ease{new EaseLinear()};

        uint32_t                    _duration{100};
        int32_t                     _loops{1};
        bool                        _alternating{false};
        uint32_t                    _delay{0};
        std::function<void(void)>   _finishedCB{nullptr};

        uint32_t    _loop{0};
        uint32_t    _progress{0};
        bool        _finished{false};

        float        calc(uint32_t dt);
    };
}



