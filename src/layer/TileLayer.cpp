// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include <iostream>
#include <assert.h>
#include <32blit.hpp>
#include "dang.hpp"
#include "TileLayer.hpp"
#include "Gear.hpp"
#include "Imagesheet.hpp"
#include "TmxExtruder.hpp"

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
/*    TileLayer::TileLayer(const SizeU& tilesize_px, const SizeF& worldsize, const std::vector<tmx_tile>& tiles, const SizeU& worldsize_tu, std::shared_ptr<Imagesheet> is, const RectF& vp)
            : Layer(Layer::LT_TILELAYER), _tilesize(tilesize_px), _tiles(tiles), _worldsize_tu(worldsize_tu),
              _imagesheet(is), _worldsize(worldsize)
    {
        _vpsize_tu.w = vp.w / _tilesize.w;
        _vpsize_tu.h = vp.h / _tilesize.h;
    }
*/
    /**
     *
     * @param layerpos in world coordinates
     * @param tileset from the tmx level
     * @param tilelayer from the tmx level
     * @param is imagesheet containing all tile images. Multiple imagesheets is not supported
     */
    TileLayer::TileLayer(const tmx_tileset* tileset, const tmx_layer* ttl, std::shared_ptr<Imagesheet> is, const RectF &vp)
            : Layer(Layer::LT_TILELAYER, ttl)
    {
        // the tiles are copied to the vector
/*        for (size_t i = 0; i < ttl->tl_tiles_len; i++)
        {
            const tmx_tile* tile = ttl->tl_tiles + i;
            _tiles.emplace_back(*tile);
//            _tiles = ttl->tiles;
        }
*/
        _imagesheet = is;

        _tilesize.w = tileset->tileWidth;
        _tilesize.h = tileset->tileHeight;

        _worldsize_tu.w = ttl->tl_width;
        _worldsize_tu.h = ttl->tl_height;
        _worldsize.w = _worldsize_tu.w * _tilesize.w;
        _worldsize.h = _worldsize_tu.h * _tilesize.h;

        _vpsize_tu.w = std::ceil(vp.w / _tilesize.w);
        _vpsize_tu.h = std::ceil(vp.h / _tilesize.h);

    }


    void TileLayer::update(uint32_t dt, const Gear& gear)
    {

    }

    void TileLayer::render(const Gear& gear)
    {
        assert(_imagesheet != nullptr);

        PointF vp = gear.getViewport().tl();
        PointU vp_tu{0,0};
        vp_tu.x = int32_t(vp.x) / _tilesize.x;
        vp_tu.y = int32_t(vp.y) / _tilesize.y;

        PointU vp_end_tu = vp_tu + _vpsize_tu;
        vp_end_tu.x = std::min(vp_end_tu.x, _worldsize_tu.w-1);
        vp_end_tu.y = std::min(vp_end_tu.y, _worldsize_tu.h-1);

        for (uint32_t x = vp_tu.x; x <= vp_end_tu.x; x++)
        {
            for (uint32_t y = vp_tu.y; y <= vp_end_tu.y; y++)
            {
                size_t index = x + (y * _worldsize_tu.w);
                const dang::tmx_tile& t = tmxLayer()->tl_tiles[index];
//                dang::tmx_tile t = _tiles.at(index);

                blit::Point  dp(x * _tilesize.w - int32_t(std::floor(vp.x)), y * _tilesize.h - int32_t(std::floor(vp.y)));
                blit::screen.blit(_imagesheet->getSurface(), _imagesheet->getBlitRect(t.id), dp, t.transform);

            }
        }
    }

}