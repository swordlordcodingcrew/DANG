// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include <iostream>
#include "dang_globals.hpp"
#include "TileLayer.h"
#include "Gear.h"
#include "Imagesheet.h"
#include "TmxExtruder.h"

namespace dang
{

    TileLayer::TileLayer() : Layer(Layer::LT_TILELAYER)
    {

    }

    TileLayer::~TileLayer()
    {

    }


    /**
     * Constructor of TileLayer
     * @param tilesize_px in pixels
     * @param layersize  size of layer
     * @param tiles list of tiles
     * @param worldsize_tu in tile units
     * @param is imagesheet containing all tile images. Multiple imagesheets is not supported
     */
    TileLayer::TileLayer(const SizeU& tilesize_px, const SizeF& layersize, const std::vector<tmx_tile>& tiles, const SizeU& worldsize_tu, std::shared_ptr<Imagesheet> is)
            : Layer(Layer::LT_TILELAYER), _tilesize(tilesize_px), _tiles(tiles), _worldsize_tu(worldsize_tu),
              _imagesheet(is), _size(layersize)
    {

    }

    /**
     *
     * @param layerpos in world coordinates
     * @param tileset from the tmx level
     * @param tilelayer from the tmx level
     * @param is imagesheet containing all tile images. Multiple imagesheets is not supported
     */
    TileLayer::TileLayer(const tmx_tileset& tileset, const tmx_tilelayer& tilelayer, std::shared_ptr<Imagesheet> is)
//    TileLayer::TileLayer(const blit::Point& layerpos_px, const tmx_tileset &tileset, const tmx_tilelayer &tilelayer, std::shared_ptr<Imagesheet> is)
            : Layer(Layer::LT_TILELAYER)
    {
        _tilesize.w = tileset.tileWidth;
        _tilesize.h = tileset.tileHeight;
        _tiles = tilelayer.tiles;
        _worldsize_tu.w = tilelayer.width;
        _worldsize_tu.h = tilelayer.height;
        _imagesheet = is;

        _size.w = _worldsize_tu.w * _tilesize.w;
        _size.h = _worldsize_tu.h * _tilesize.h;

    }

    void TileLayer::update(uint32_t time, const Gear& gear)
    {

    }

    void TileLayer::render(const Gear& gear)
    {
        if (_imagesheet == nullptr)
        {
            return;
        }

        if (blit::screen.sprites != _imagesheet.get())
        {
            blit::screen.sprites = _imagesheet.get();
        }

        blit::Rect vp(int32_t(gear.getViewport().x),
                      int32_t(gear.getViewport().y),
                      int32_t(gear.getViewport().w),
                      int32_t(gear.getViewport().h));
        blit::Rect vp_tu{0,0,0,0};
        vp_tu.x = int32_t(vp.x / _tilesize.w);
        vp_tu.w = int32_t(vp.w / _tilesize.w);
        vp_tu.y = int32_t(vp.y / _tilesize.h);
        vp_tu.h = int32_t(vp.h / _tilesize.h);
        int32_t offset_x = int32_t(vp.x) % _tilesize.w;
        int32_t offset_y = int32_t(vp.y) % _tilesize.h;

        if (vp_tu.x + vp_tu.w > _worldsize_tu.w)
        {
            vp_tu.x = _worldsize_tu.w - vp_tu.w;
        }

        if (vp_tu.y + vp_tu.h > _worldsize_tu.h)
        {
            vp_tu.y = _worldsize_tu.h - vp_tu.h;
        }

        for (int32_t x = vp_tu.x; x < vp_tu.x + vp_tu.w; x++)
        {
            for (int32_t y = vp_tu.y; y < vp_tu.y + vp_tu.h; y++)
            {
                dang::tmx_tile t = _tiles[x + (y * _worldsize_tu.w)];

                uint8_t transform = (t.isFlippedHorizontally ? blit::SpriteTransform::HORIZONTAL : uint8_t(0)) |
                                    (t.isFlippedVertically ? blit::SpriteTransform::VERTICAL : uint8_t(0)) |
                                    (t.isFlippedAntiDiagonally ? blit::SpriteTransform::XYSWAP : uint8_t(0));

//                blit::Point p(x * _tilesize.w - offset_x, y * _tilesize.h - offset_y);
                blit::Point p(x * _tilesize.w - vp.x, y * _tilesize.h - vp.y);
                blit::screen.blit_sprite(_imagesheet->getRect(t.id), p, transform);
            }
        }
    }
}