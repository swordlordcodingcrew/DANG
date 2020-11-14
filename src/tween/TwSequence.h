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

        void        update(uint32_t dt) override;

        void        finish(bool suppressCB = false) override;
        void        reset() override;
        bool        isFinished() override;


    protected:
        std::vector<std::shared_ptr<Tweenable>> _tw_seq;
        size_t _index{0};
    };
}


