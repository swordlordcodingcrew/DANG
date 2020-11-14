// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "Layer.h"
#include <vector>

namespace dang
{
    struct tmx_tile;
    struct tmx_tileset;
    struct tmx_tilelayer;
    class Imagesheet;

    class TileLayer : public Layer
    {
    public:
        TileLayer();
        TileLayer(const SizeU& tilesize_px, const SizeF& layersize, const std::vector<tmx_tile>& tiles, const SizeU& worldsize_tu, std::shared_ptr<Imagesheet> is, const RectF& vp);
        TileLayer(const tmx_tileset& tileset, const tmx_tilelayer& tilelayer, std::shared_ptr<Imagesheet> is, const RectF& vp);
        ~TileLayer() override;

        void    update(uint32_t dt, const Gear& gear) override;
        void    render(const Gear& gear) override;

    protected:
        // size of tile in pixel
        SizeU                       _tilesize{1,1};
        // size of world in tile units
        SizeU                       _worldsize_tu{1,1};
        // size of world in pixel
        SizeF                       _worldsize{1, 1};
        // size of vp in tile units
        SizeU                       _vpsize_tu{1,1};

        std::vector<tmx_tile>       _tiles;
        std::shared_ptr<Imagesheet> _imagesheet{nullptr};

    };

}


