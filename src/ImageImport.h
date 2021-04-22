// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <types/size.hpp>
#include <graphics/surface.hpp>

#include <vector>

namespace dang
{
    struct image_import
    {
        uint8_t alpha{255};
        blit::Size bounds{0,0};
        std::vector<uint8_t> data;
        std::vector<blit::Pen> palette;
    };

}