// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework


#include "Imagesheet.hpp"
#include "ImageImport.h"
#include <graphics/surface.hpp>

#include <cassert>

namespace dang
{

    /**
     *
     * @param name name of spritesheet
     * @param data pointer to image_import struct (i.e. the image data)
     * @param cols number of columns
     * @param rows number of rows
     */
    Imagesheet::Imagesheet(const std::string& name, const image_import* ii, uint16_t cols, uint16_t rows)
    : _name(name), _image_import(ii), _cols(cols), _rows(rows)
    {
        assert (cols < UINT16_MAX);
        assert (rows < UINT16_MAX);
        assert(ii != nullptr);

        if (_cols == 0) _cols = 1;
        if (_rows == 0) _rows = 1;

        update_image_size();

//        _surface = new blit::Surface(ii->data.data(), blit::PixelFormat::P, ii->bounds);
        _surface = new blit::Surface(const_cast<uint8_t *>(ii->data), blit::PixelFormat::P, ii->bounds);
        _surface->alpha = ii->alpha;
//        _surface->palette = ii->palette.data();
        _surface->palette = const_cast<blit::Pen*>(ii->palette);

    }

    Imagesheet::~Imagesheet()
    {
        delete _surface;
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
        _img_size.h = _image_import->bounds.h / _rows;
        _img_size.w = _image_import->bounds.w / _cols;
    }

    /**
     * Get the source rect of the image
     *
     * @param r rect to be filled as source rect of the imagesheet (spritesheet)
     * @param col column index (beginning with 0)
     * @param row row index (beginning with 0)
     * @return true if success
     */
    bool Imagesheet::getRect(RectU &r, uint16_t col, uint16_t row)
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
     * @param index of image. Beginning with zero, counting up to the right (cols) and then down (rows)
     * @return true if success
     */
    bool Imagesheet::getRect(RectU &r, uint16_t index)
    {
        return getRect(r, index % _cols, index / _cols);
    }

    RectU Imagesheet::getRect(const uint16_t index)
    {
        RectU sr{0, 0, 0, 0};
        getRect(sr, index);
        return sr;
    }

    blit::Rect Imagesheet::getBlitRect(const uint16_t index)
    {
        blit::Rect sr{0, 0, 0, 0};
        uint16_t col = index % _cols;
        uint16_t row = index / _cols;

        if (col >= _cols || row >= _rows) return sr;

        sr.x = _img_size.w * col;
        sr.y = _img_size.h * row;
        sr.w = _img_size.w;
        sr.h = _img_size.h;

        return sr;
    }

    SizeU Imagesheet::getImagesheetSize()
    {
        return SizeU(_image_import->bounds.w, _image_import->bounds.h);
    }

    uint32_t Imagesheet::getImagesheetSizeW() const
    {
        return _image_import->bounds.w;
    }

    uint32_t Imagesheet::getImagesheetSizeH() const
    {
        return _image_import->bounds.h;
    }

}