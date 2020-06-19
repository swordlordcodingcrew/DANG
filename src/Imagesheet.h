// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "../../../../../32blit/32blit.hpp"

namespace dang
{
    class Imagesheet : public blit::SpriteSheet
    {
    public:
        Imagesheet(uint8_t *data, blit::PixelFormat format, const blit::packed_image *image, uint16_t cols = 1, uint16_t rows = 1);
        ~Imagesheet();
        void setCols(uint16_t cols);
        void setRows(uint16_t rows);
        void setColsAndRows(uint16_t cols, uint16_t rows);

        static Imagesheet *load(const uint8_t *data, uint8_t *buffer = nullptr, uint16_t cols = 1, uint16_t rows = 1);
        static Imagesheet *load(const blit::packed_image *image, uint8_t *buffer = nullptr, uint16_t cols = 1, uint16_t rows = 1);

        static std::shared_ptr<Imagesheet> loadShared(const uint8_t *data, uint8_t *buffer = nullptr, const uint16_t cols = 1, const uint16_t rows = 1);
        static std::shared_ptr<Imagesheet> loadShared(const blit::packed_image *image, uint8_t *buffer = nullptr, const uint16_t cols = 1, const uint16_t rows = 1);

        bool getRect(blit::Rect &r, uint16_t col, uint16_t row);
        bool getRect(blit::Rect &r, uint16_t index = 0);
        blit::Rect getRect(const uint16_t &index);
        blit::Rect getRect(const blit::Point &p);
        blit::Rect getRect(const blit::Rect &r);

        std::string _name{};

    protected:
        uint16_t    _cols{1};
        uint16_t    _rows{1};
        blit::Size  _img_size{0,0};

        void        update_image_size();
    };

}


