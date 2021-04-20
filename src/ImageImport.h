// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

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