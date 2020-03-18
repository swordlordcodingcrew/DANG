// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#pragma once

#include "Layer.h"

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
        TileLayer(const blit::Size& tilesize_px, const blit::Point& layerpos_px, const blit::Size& layersize_px, const std::vector<tmx_tile>& tiles, blit::Size& wordlsize_tu, std::shared_ptr<Imagesheet> is);
        TileLayer(const blit::Point& layerpos_px, const tmx_tileset& tileset, const tmx_tilelayer& tilelayer, std::shared_ptr<Imagesheet> is);
        virtual ~TileLayer();

        virtual void    update(uint32_t time, const Gear& gear) override;
        virtual void    render(const Gear& gear) override;

    protected:
        blit::Size                  _tilesize{1,1};
        blit::Size                  _worldsize_tu{1,1};
        std::vector<tmx_tile>           _tiles;
        std::shared_ptr<Imagesheet> _imagesheet{nullptr};

    };

}


