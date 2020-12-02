// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "dang.hpp"
#include "RectT.h"

#include <string>

namespace dang
{
    class Imagesheet
    {
    public:
        Imagesheet();
        Imagesheet(const std::string& name, const uint8_t *data, SizeU& imgsheet_size, uint16_t cols = 1, uint16_t rows = 1);
        ~Imagesheet();

        void setCols(uint16_t cols);
        void setRows(uint16_t rows);
        void setColsAndRows(uint16_t cols, uint16_t rows);

        bool getRect(RectU &r, uint16_t col, uint16_t row);
        bool getRect(RectU &r, uint16_t index = 0);
        RectU getRect(const uint16_t index);

        const uint8_t* getData() { return _data; }

    protected:
        std::string _name{};
        const uint8_t*    _data{nullptr};
        uint16_t    _cols{1};
        uint16_t    _rows{1};
        SizeU      _img_size{0,0};
        SizeU      _imgsheet_size{0,0};

        void        update_image_size();
    };

}


