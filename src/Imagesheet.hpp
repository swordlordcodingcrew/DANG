// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "RectT.hpp"

#include <string>

namespace blit
{
    struct Surface;
    struct Rect;
}

namespace dang
{
    struct image_import;

    class Imagesheet
    {
    public:
        Imagesheet() = delete;
        Imagesheet(const std::string& name, image_import* ii, uint16_t cols = 1, uint16_t rows = 1);
        ~Imagesheet();

        void setCols(uint16_t cols);
        void setRows(uint16_t rows);
        void setColsAndRows(uint16_t cols, uint16_t rows);

        bool getRect(RectU &r, uint16_t col, uint16_t row);
        bool getRect(RectU &r, uint16_t index = 0);
        RectU getRect(uint16_t index);
        blit::Rect getBlitRect(const uint16_t index);

        SizeU getImagesheetSize();
        uint32_t getImagesheetSizeW() const;
        uint32_t getImagesheetSizeH() const;

        const std::string&  getName() { return _name; }

        blit::Surface* getSurface() { return _surface; }

    protected:
        const std::string   _name{};
        image_import*       _image_import{nullptr};
        blit::Surface*      _surface{nullptr};
        uint16_t            _cols{1};
        uint16_t            _rows{1};
        SizeU               _img_size{0,0};

        void        update_image_size();
    };

}


