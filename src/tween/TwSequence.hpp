// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include <vector>

namespace dang
{
    class TwSequence : public Tweenable
    {
    public:
        TwSequence();
        TwSequence(const TwSequence& tw);
        void    addTween(std::shared_ptr<Tweenable> tw);

        void        update(void* obj, uint32_t dt) override;

        void        finish(bool suppressCB = false) override;
        void        reset() override;
        bool        isFinished() override;


    protected:
        std::vector<std::shared_ptr<Tweenable>> _tw_seq;
        size_t _index{0};
    };
}


