// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Ease.hpp"

#include "dang.hpp"

#include <cstdint>
#include <memory>
#include <functional>


namespace dang
{
    class FullColSpr;
    class FullImgSpr;
    class Tweenable;

    class Tweenable
    {
    public:
        Tweenable();
        Tweenable(const Tweenable& tw);
        Tweenable(uint32_t duration, EaseFn& ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        virtual ~Tweenable();

        virtual void        duration(uint32_t d) {_duration = d; }
        virtual uint32_t    duration() const { return _duration; }
        virtual void        loops(int32_t l) { _loops = l; }
        virtual void        alternating(bool a) { _alternating = a; }
        virtual void        delay(uint32_t d) {_delay = d; }
        virtual void        easeFunction(EaseFn ease_cb) { _ease_cb = ease_cb; }

        virtual void        finish(bool suppressCB = false);
        virtual void        reset();
        virtual bool        isFinished();
        virtual void        setFinishedCallback(std::function<void(void)> finishedCB);

        /**
         * visitor functions (see visitor pattern)
         *
         */
        virtual void        init(FullColSpr& obj) {};
        virtual void        update(FullColSpr& obj, uint32_t dt) {};
        virtual void        init(FullImgSpr& obj) {};
        virtual void        update(FullImgSpr& obj, uint32_t dt) {};

    protected:
        EaseFn                      _ease_cb = Ease::Linear;
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



