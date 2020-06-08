// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include <cassert>
//#include <memory>
#include "dang_globals.hpp"
#include "Gear.h"
#include "Layer.h"
#include "Sprite.h"
#include "Imagesheet.h"
#include "tmx_def.h"
#include "SpriteLayer.h"
#include "TileLayer.h"
#include "CollisionSpriteLayer.h"
//#include "dang_collision.hpp"

namespace dang
{

    Gear::Gear()
    {
    }

    Gear::~Gear()
    {
    }

    void Gear::initLevel(const tmx_level &lvl, RectF& viewport)
    {
        _viewport = viewport;

        _world.w = lvl.w.getWidthInPixel();
        _world.h = lvl.w.getHeightInPixel();

        // active world defaults to double width and height of world
        _active_world_size.x = 2 * _world.w;
        _active_world_size.y = 2 * _world.h;

        // add the imagesheets
        // actually the copy should not be necessary
        // TODO: js-exporter with std::shared_ptr
        for (const auto& map : lvl.imagesheets)
        {
            std::shared_ptr<Imagesheet> is = std::shared_ptr<Imagesheet>(map.second);
            is->_name = map.first;
            addImagesheet(map.first, is);

        }
    }

    void Gear::render(uint32_t time)
    {
        for (std::shared_ptr<Layer> l : _layers)
        {
            if (l->_visible)
            {
                l->render(*this);
            }
        }
    }

    void Gear::update(uint32_t time)
    {
        for (std::shared_ptr<Layer> l : _layers)
        {
            l->update(time, *this);
        }
    }

    void Gear::addImagesheet(const std::string& key, std::shared_ptr<Imagesheet> is)
    {
        _imagesheets[key] = is;
    }

    void Gear::removeImagesheet(const std::string& key)
    {
        _imagesheets.erase(key);
    }

    void Gear::addLayer(std::shared_ptr<Layer> layer)
    {
        _layers.push_front(layer);
        _layers.sort([] (const std::shared_ptr<Layer> &first, const std::shared_ptr<Layer> &second)
        {
            return first->_z_order < second->_z_order;
        });

    }

    std::shared_ptr<Layer> Gear::addTileLayer(tmx_level &lvl, const std::string &name)
    {
        auto layer_it = std::find_if(lvl.layers.begin(), lvl.layers.end(), [=](const std::shared_ptr<tmx_layer>& val)
        {
            return (val->name == name);
        });

        assert(layer_it != lvl.layers.end());
        assert(layer_it->get()->type == ltTile);

        tmx_tilelayer* ttl = static_cast<tmx_tilelayer*>(layer_it->get());

        // Fetch tmx_tileset - this is not entirely correct since we assume that all tiles are in the same imagesheet.
        tmx_tileset& ts = lvl.tilesets[ttl->tiles[0].tileset];

        std::shared_ptr<Imagesheet> is = getImagesheetByName(ttl->name);
        std::shared_ptr<TileLayer> tl = std::make_shared<dang::TileLayer>(dang::TileLayer(dang::PointF(0,0), ts, *ttl, is));
        tl->_name = ttl->name;
        // TODO js-exporter: implement zOrder
        tl->_z_order = 0;
        // TODO js-exporter: implement visible flag
        // tl->_visible = tilel->visible;
        addLayer(tl);

        return tl;
    }

    std::shared_ptr<Layer> Gear::addSpriteLayer(tmx_level &lvl, const std::string &name, bool collision_enabled)
    {
        auto layer_it = std::find_if(lvl.layers.begin(), lvl.layers.end(), [=](const std::shared_ptr<dang::tmx_layer>& val)
        {
            return (val->name == name);
        });

        assert(layer_it != lvl.layers.end());
        assert(layer_it->get()->type == dang::ltObjects);

        dang::tmx_objectlayer* ola = static_cast<dang::tmx_objectlayer*>(layer_it->get());

        std::shared_ptr<dang::SpriteLayer> sl;
        if (collision_enabled)
        {
            sl = std::make_shared<dang::CollisionSpriteLayer>(CollisionSpriteLayer(getWorld()));
        }
        else
        {
            sl = std::make_shared<dang::SpriteLayer>(SpriteLayer(getWorld()));
        }
        sl->_name = ola->name;
        // TODO js-exporter: implement zOrder
        sl->_z_order = 1;
        // TODO js-exporter: implement visible flag
        // csl->_visible = ola->visible;

        addLayer(sl);

        return sl;
    }

    void Gear::removeLayer(std::shared_ptr<Layer> layer)
    {
        _layers.remove(layer);
    }

    std::shared_ptr<Layer> Gear::getLayerByName(const std::string &name)
    {
        std::forward_list<std::shared_ptr<Layer>>::iterator layer_it = std::find_if(_layers.begin(), _layers.end(), [=](const std::shared_ptr<Layer>& val)
        {
            return (val->_name == name);
        });
        return (*layer_it);
    }

    const tmx_objectlayer &Gear::getTmxObjectLayer(tmx_level &lvl, const std::string &name)
    {
        auto layer_it = std::find_if(lvl.layers.begin(), lvl.layers.end(), [=](const std::shared_ptr<dang::tmx_layer>& val)
        {
            return (val->name == name);
        });

        assert(layer_it != lvl.layers.end());
        assert(layer_it->get()->type == dang::ltObjects);

        dang::tmx_objectlayer* ola = static_cast<dang::tmx_objectlayer*>(layer_it->get());

        return *ola;
    }

    RectF Gear::getActiveWorld() const
    {
        return RectF(_viewport.x - (_active_world_size.x - _viewport.w)/2, _viewport.y - ((_active_world_size.y - _viewport.h)/2), _active_world_size.x, _active_world_size.y);
    }

    std::shared_ptr<Imagesheet> Gear::getImagesheetByName(std::string &name)
    {
        return _imagesheets[name];
    }

    void Gear::setViewportPos(Vector2F& pos)
    {
        _viewport.x = pos.x;
        _viewport.y = pos.y;
    }


}