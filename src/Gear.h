// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#pragma once

#include "dang_globals.hpp"
#include <unordered_map>
#include <forward_list>
#include "RectT.h"

namespace dang
{
    struct tmx_level;
    struct tmx_tilelayer;
    struct tmx_objectlayer;
    struct tmx_layer;

    class Sprite;
    class Layer;
    class Imagesheet;

    class Gear
    {
    public:
        Gear();
        virtual ~Gear();

        void    initLevel(const tmx_level& lvl, RectF& viewport);

        void    update(uint32_t time);
        void    render(uint32_t time);
//        void    renderImage(const std::shared_ptr<Imagesheet> is, const uint16_t im_id, const blit::Point& dest, const uint8_t transform) const;

        std::shared_ptr<Imagesheet> getImagesheet(const std::string& name);
        std::shared_ptr<Imagesheet> addImagesheet(const tmx_level& lvl, const std::string& name);
        void                        removeImagesheet(const std::string& name);
//        void                        addImagesheet(const std::string& key, std::shared_ptr<Imagesheet> is);

        std::shared_ptr<Layer>      addTileLayer(tmx_level &lvl, const std::string &name);
        std::shared_ptr<Layer>      addSpriteLayer(tmx_level &lvl, const std::string &name, bool collision_enabled);
        void                        addLayer(std::shared_ptr<Layer> layer);
        void                        removeLayer(std::shared_ptr<Layer> layer);
        std::shared_ptr<Layer>      getLayerByName(const std::string& name);

        // this function should be in some sort of TmxExtractor-class
        const tmx_objectlayer&     getTmxObjectLayer(tmx_level &lvl, const std::string &name);

        RectF  getActiveWorld() const;
        RectF  getViewport() const { return _viewport; }
        void        setViewportPos(Vector2F& pos);
        RectF  getWorld() const { return _world;}

    protected:
//        dang::level _lvl;
        std::unordered_map<std::string, std::shared_ptr<Imagesheet>> _imagesheets;
        std::forward_list<std::shared_ptr<Layer>> _layers;

        // viewport handling
        RectF      _world{0,0,0,0};    // size of the world
        RectF      _viewport{0,0,0,0};     // part of world to be drawn
        Vector2F   _active_world_size{0,0}; // center equals to center of viewport. sprite within active world will be updated and - if in viewport - drawnd

    };
}


