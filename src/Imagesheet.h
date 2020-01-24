// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 30.12.19.
//

#pragma once

#include "../../../../../32blit/32blit.hpp"

namespace dang
{
    class Imagesheet : public blit::spritesheet
    {
    public:
        Imagesheet(uint8_t *data, blit::pixel_format format, const blit::packed_image *image, const uint16_t cols = 1, const uint16_t rows = 1);
        void setCols(uint16_t const cols);
        void setRows(uint16_t const rows);
        void setColsAndRows(uint16_t const cols, uint16_t const rows);

        static Imagesheet *load(const uint8_t *data, uint8_t *buffer = nullptr, const uint16_t cols = 1, const uint16_t rows = 1);
        static Imagesheet *load(const blit::packed_image *image, uint8_t *buffer = nullptr, const uint16_t cols = 1, const uint16_t rows = 1);

        static std::shared_ptr<Imagesheet> load_sp(const uint8_t *data, uint8_t *buffer = nullptr, const uint16_t cols = 1, const uint16_t rows = 1);
        static std::shared_ptr<Imagesheet> load_sp(const blit::packed_image *image, uint8_t *buffer = nullptr, const uint16_t cols = 1, const uint16_t rows = 1);

        bool getRect(blit::rect &r, const uint16_t col, const uint16_t row);
        bool getRect(blit::rect &r, const uint16_t index = 0);
        blit::rect getRect(const uint16_t &index);
        blit::rect getRect(const blit::point &p);
        blit::rect getRect(const blit::rect &r);

        void setBits(const uint16_t bits, const uint16_t col, const uint16_t row);
        void setBits(const uint16_t bits, const uint16_t index = 0);

    protected:
        uint16_t    _cols{1};
        uint16_t    _rows{1};
        blit::size  _img_size{0,0};

        // TODO: should be BitField
        std::vector<uint16_t>  _bits{0};

        void        update_image_size();
    };

}


