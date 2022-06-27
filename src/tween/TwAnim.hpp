// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Tweenable.hpp"
#include "DangFwdDecl.h"

namespace dang
{
    class TwAnim : public Tweenable
    {
    public:
        TwAnim() = default;
        TwAnim(const TwAnim& tw) = default;
        TwAnim(const std::vector<uint16_t>& indices, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        TwAnim(const spImagesheet is, const std::vector<uint16_t>& indices, uint32_t duration, EaseFn ease_cb, int32_t loops = 1, bool alternating = false, uint32_t delay = 0);
        void    update(void* obj, uint32_t dt) override;
        void    init(void* obj) override;

    protected:
        std::vector<uint16_t>   _indices{0};
        spImagesheet            _imagesheet{nullptr};
    };

}


