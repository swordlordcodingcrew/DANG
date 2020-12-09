// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "dang.hpp"
#include <unordered_map>
#include <forward_list>
#include "RectT.h"
#include <memory>

namespace dang
{
    struct tmx_level;
    struct tmx_tilelayer;
    struct tmx_objectlayer;
    struct tmx_layer;

    class Sprite;
    class Layer;
    class Imagesheet;

    using spImagesheet = std::shared_ptr<Imagesheet>;

    class Gear
    {
    public:
        Gear();
        virtual ~Gear();

        void    initLevel(const tmx_level& lvl, RectF& viewport);

        void    update(uint32_t dt);
        void    render(uint32_t time);

//        void                        addImagesheet(const std::string& key, std::shared_ptr<Imagesheet> is);
        void                        addImagesheet(std::shared_ptr<Imagesheet> is);
        std::shared_ptr<Imagesheet> getImagesheet(const std::string& name) const;
        void                        removeImagesheet(const std::string& name);
        void                        removeImagesheets();

        void                        addLayer(std::shared_ptr<Layer> layer);
        std::shared_ptr<Layer>      getLayerByName(const std::string& name);
        void                        removeLayer(std::shared_ptr<Layer> layer);
        void                        removeLayers();

        RectF       getActiveWorld() const;
        Vector2F    getActiveWorldSize() const { return _active_world_size; };
        void        setActiveWorldSize(Vector2F& aws) {_active_world_size = aws; };
        void        setActiveWorldSize(float w, float h) {_active_world_size.w = w; _active_world_size.h = h; };

        RectF  getViewport() const { return _viewport; }
        void   setViewport(const RectF& vp);
        void   setViewportPos(const Vector2F& pos);

        void   follow(const Vector2F& dest);

        RectF  getWorld() const { return _world;}
        void   setWorld(const RectF& world) {_world = world; };

        // function pointers
        static void empty_blit_sprite_cb(RectU sr, Vector2I p, uint8_t t) {};
        static void empty_set_spritesheet_cb(spImagesheet is) {};
        void (*blit_sprite_cb)(RectU, Vector2I, uint8_t) = empty_blit_sprite_cb;
        void (*set_spritesheet_cb)(spImagesheet is) = empty_set_spritesheet_cb;
#ifdef DANG_DEBUG_DRAW
        static void empty_blit_sprite_debug_draw_cb(RectF sr, Vector2F p, uint8_t t);
        void (*blit_sprite_debug_draw_cb)(RectF, Vector2F, uint8_t) = empty_blit_sprite_debug_draw_cb;
#endif
    protected:

        std::unordered_map<std::string, std::shared_ptr<Imagesheet>> _imagesheets;
        std::forward_list<std::shared_ptr<Layer>> _layers;

        // viewport handling
        RectF       _world{0,0,0,0};    // size of the world
        RectF       _viewport{0,0,0,0};     // part of world to be drawn
        Vector2F    _active_world_size{0,0}; // center equals to center of viewport. sprite within active world will be updated and - if in viewport - drawnd
    };
}

