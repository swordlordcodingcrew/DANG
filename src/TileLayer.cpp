// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include <iostream>
#include <assert.h>
#include "dang.hpp"
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
     * @param worldsize  size of layer
     * @param tiles list of tiles
     * @param worldsize_tu in tile units
     * @param is imagesheet containing all tile images. Multiple imagesheets is not supported
     */
    TileLayer::TileLayer(const SizeU& tilesize_px, const SizeF& worldsize, const std::vector<tmx_tile>& tiles, const SizeU& worldsize_tu, std::shared_ptr<Imagesheet> is, const RectF& vp)
            : Layer(Layer::LT_TILELAYER), _tilesize(tilesize_px), _tiles(tiles), _worldsize_tu(worldsize_tu),
              _imagesheet(is), _worldsize(worldsize)
    {
        _vpsize_tu.w = vp.w / _tilesize.w;
        _vpsize_tu.h = vp.h / _tilesize.h;
    }

    /**
     *
     * @param layerpos in world coordinates
     * @param tileset from the tmx level
     * @param tilelayer from the tmx level
     * @param is imagesheet containing all tile images. Multiple imagesheets is not supported
     */
    TileLayer::TileLayer(const tmx_tileset& tileset, const tmx_tilelayer& tilelayer, std::shared_ptr<Imagesheet> is, const RectF& vp)
            : Layer(Layer::LT_TILELAYER)
    {
        _tiles = tilelayer.tiles;
        _imagesheet = is;

        _tilesize.w = tileset.tileWidth;
        _tilesize.h = tileset.tileHeight;

        _worldsize_tu.w = tilelayer.width;
        _worldsize_tu.h = tilelayer.height;
        _worldsize.w = _worldsize_tu.w * _tilesize.w;
        _worldsize.h = _worldsize_tu.h * _tilesize.h;

        _vpsize_tu.w = std::ceil(vp.w / _tilesize.w);
        _vpsize_tu.h = std::ceil(vp.h / _tilesize.h);

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

        PointF vp = gear.getViewport().tl();
        PointU vp_tu{0,0};
        vp_tu.x = int32_t(vp.x) / _tilesize.x;
        vp_tu.y = int32_t(vp.y) / _tilesize.y;

        PointU vp_end_tu = vp_tu + _vpsize_tu;
        vp_end_tu.x = std::min(vp_end_tu.x, _worldsize_tu.w-1);
        vp_end_tu.y = std::min(vp_end_tu.y, _worldsize_tu.h-1);

        for (int32_t x = vp_tu.x; x <= vp_end_tu.x; x++)
        {
            for (int32_t y = vp_tu.y; y <= vp_end_tu.y; y++)
            {
                size_t index = x + (y * _worldsize_tu.w);
                dang::tmx_tile t = _tiles.at(index);

                uint8_t transform = (t.isFlippedHorizontally ? blit::SpriteTransform::HORIZONTAL : uint8_t(0)) |
                                    (t.isFlippedVertically ? blit::SpriteTransform::VERTICAL : uint8_t(0)) |
                                    (t.isFlippedAntiDiagonally ? blit::SpriteTransform::XYSWAP : uint8_t(0));

                blit::Point p(x * _tilesize.w - int32_t(vp.x), y * _tilesize.h - int32_t(vp.y));
                blit::screen.blit_sprite(_imagesheet->getRect(t.id), p, transform);
            }
        }
    }
}