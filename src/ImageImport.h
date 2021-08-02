// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <types/size.hpp>
#include <graphics/surface.hpp>

namespace dang
{
    struct image_import
    {
        const uint8_t alpha{255};
        const blit::Size bounds{0,0};
        const blit::Pen* palette;
        const uint8_t* data;
    };

}