// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Imagesheet.h"
#include <cassert>

namespace dang
{

    /**
     * Create an Imagesheet from packaged data. Child of spritesheet (for compaitbility reasons)
     *
     * @param data
     * @param format
     * @param image
     * @param cols number of columns. Must be 1 or higher. 0 will be interpreted as 1. Default value is 1 if no value is given.
     * @param rows number of rows. Must be 1 or higher. 0 will be interpreted as 1. Default value is 1 if no value is given.
     */
    Imagesheet::Imagesheet(uint8_t *data, blit::PixelFormat format, const blit::packed_image *image, const uint16_t cols, const uint16_t rows) : SpriteSheet(data, format, image), _cols(cols), _rows(rows)
    {
        assert (cols < UINT16_MAX);
        assert (rows < UINT16_MAX);

        if (_cols == 0) _cols = 1;
        if (_rows == 0) _rows = 1;

        update_image_size();
    }

    Imagesheet::~Imagesheet()
    {
        // TODO: should we delete some data?
    }

    /**
     * Set the number of columns of imagesheet
     *
     * @param cols number of cols. Must be 1 or higher. 0 will be interpreted as 1
     */
    void Imagesheet::setCols(const uint16_t cols)
    {
        assert (cols < UINT16_MAX);
        _cols = cols;
        if (_cols == 0) _cols = 1;
        update_image_size();
    }

    /**
     * Set the number of rows of imagesheet
     *
     * @param rows number of rows. Must be 1 or higher. 0 will be interpreted as 1
     */
    void Imagesheet::setRows(const uint16_t rows)
    {
        assert (rows < UINT16_MAX);
        _rows = rows;
        if (_rows == 0) _rows = 1;
        update_image_size();
    }

    /**
     * Set the number of colums and rows
     *
     * @param cols number of cols. Must be 1 or higher. 0 will be interpreted as 1
     * @param rows number of rows. Must be 1 or higher. 0 will be interpreted as 1
     */
    void Imagesheet::setColsAndRows(uint16_t const cols, uint16_t const rows)
    {
        assert (cols < UINT16_MAX);
        assert (rows < UINT16_MAX);
        _cols = cols;
        _rows = rows;
        if (_cols == 0) _cols = 1;
        if (_rows == 0) _rows = 1;
        update_image_size();
    }

    /**
     * Internal function to update image size. The function is called, whenever cols or rows change
     *
     */
    void Imagesheet::update_image_size()
    {
        _img_size.h = bounds.h / _rows;
        _img_size.w = bounds.w / _cols;
    }

    /**
     * Get the source rect of the image
     *
     * @param r rect to be filled as source rect of the imagesheet (spritesheet)
     * @param col column index (beginning with 0)
     * @param row row index (beginning with 0)
     * @return true if success
     */
    bool Imagesheet::getRect(blit::Rect &r, const uint16_t col, const uint16_t row)
    {
        if (col >= _cols || row >= _rows) return false;

        r.x = _img_size.w * col;
        r.y = _img_size.h * row;
        r.w = _img_size.w;
        r.h = _img_size.h;

        return true;
    }

    /**
     * Get the source rect of the image
     *
     * @param r rect to be filled as source rect of the imagesheet (spritesheet)
     * @param index of image. Beginning with zero, counting up to the right (cols) and down (rows)
     * @return true if success
     */
    bool Imagesheet::getRect(blit::Rect &r, uint16_t index)
    {
        return getRect(r, index % _cols, index / _cols);
    }

    /**
     * Returns the source rect of given index. (spritesheet compatibility function)
     *
     * @param index of image. Beginning with zero, counting up to the right (cols) and down (rows)
     * @return source rect of image with index index
     */
    blit::Rect Imagesheet::getRect(const uint16_t &index)
    {
        blit::Rect sr{0, 0, 1, 1};
        getRect(sr, index);
        return sr;
    }

    /**
     * Returns the source rect of given cols/rows represented in a point struct. (spritesheet compatibility function)
     * @param p
     * @return
     */
    blit::Rect Imagesheet::getRect(const blit::Point &p)
    {
        assert (p.x < UINT16_MAX);
        assert (p.y < UINT16_MAX);

        blit::Rect sr{0, 0, 1, 1};
        getRect(sr, (uint16_t)p.x, (uint16_t)p.y);
        return sr;
    }

    /**
     * Returns the source rect of given cols/rows represented in a rect struct. (spritesheet compatibility function)
     * @param r
     * @return
     */
    blit::Rect Imagesheet::getRect(const blit::Rect &r)
    {
        assert (r.x < UINT16_MAX);
        assert (r.y < UINT16_MAX);

        blit::Rect sr{0, 0, 1, 1};
        getRect(sr, (uint16_t)r.x, (uint16_t)r.y);
        return sr;
    }

    /**
     * Static function to load an imagesheet. Copied from spritesheet
     *
     * @param data
     * @param buffer
     * @param cols
     * @param rows
     * @return
     */
    Imagesheet *Imagesheet::load(const uint8_t *data, uint8_t *buffer, const uint16_t cols, const uint16_t rows)
    {
        return load((blit::packed_image *)data, buffer, cols, rows);
    }

    /**
     * Static function to load an imagesheet. Copied from spritesheet
     *
     * @param image
     * @param buffer
     * @param cols
     * @param rows
     * @return
     */
    Imagesheet *Imagesheet::load(const blit::packed_image *image, uint8_t *buffer, const uint16_t cols, const uint16_t rows)
    {
        if (buffer == nullptr)
        {
            uint32_t a = blit::pixel_format_stride[image->format];
            uint32_t b = image->width;
            uint32_t c = image->height;
            uint32_t d = a * b * c;
            buffer = new uint8_t[blit::pixel_format_stride[image->format] * image->width * image->height];
        }

        return new Imagesheet(buffer, (blit::PixelFormat)image->format, image, cols, rows);
    }

    std::shared_ptr<Imagesheet> Imagesheet::load_sp(const uint8_t *data, uint8_t *buffer, const uint16_t cols, const uint16_t rows)
    {
        return load_sp((blit::packed_image *)data, buffer, cols, rows);
    }

    std::shared_ptr<Imagesheet> Imagesheet::load_sp(const blit::packed_image *image, uint8_t *buffer, const uint16_t cols, const uint16_t rows)
    {
        if (buffer == nullptr)
        {
            buffer = new uint8_t[blit::pixel_format_stride[image->format] * image->width * image->height];
        }

        return std::make_shared<Imagesheet>(buffer, (blit::PixelFormat)image->format, image, cols, rows);
    }

}