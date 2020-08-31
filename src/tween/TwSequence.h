// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.h"
#include <vector>

namespace dang
{
    class TwSequence : public Tweenable
    {
    public:
        TwSequence();
        void    addTween(std::shared_ptr<Tweenable> tw);
        void    setObject(std::weak_ptr<void> obj) override;

        void        update(uint32_t time) override;

        void        startTw(uint32_t start_time) override;
        void        pauseTw(uint32_t time) override;
        void        continueTw(uint32_t time) override;
        void        finishTw(bool suppressCB = false) override;
        void        resetTw() override;
        bool        isTwFinished() override;


    protected:
        std::vector<std::shared_ptr<Tweenable>> _tw_seq;
        size_t _index{0};
    };
}


