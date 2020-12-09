// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <cassert>
#include <algorithm>
#include "Gear.h"
#include "Layer.h"
#include "Sprite.h"
#include "Imagesheet.h"
#include "TmxExtruder.h"
#include "SpriteLayer.h"
#include "TileLayer.h"
#include "CollisionSpriteLayer.h"

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
        _world.w = lvl.w.width * lvl.w.tileWidth;
        _world.h = lvl.w.height * lvl.w.tileHeight;

        // active world defaults to double width and height of world
        _active_world_size.x = 2 * _world.w;
        _active_world_size.y = 2 * _world.h;

        setViewport(viewport);
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

    void Gear::update(uint32_t dt)
    {
        for (std::shared_ptr<Layer> l : _layers)
        {
            l->update(dt, *this);
        }
    }

/*    void Gear::addImagesheet(const std::string& key, std::shared_ptr<Imagesheet> is)
    {
        assert(!is->getName().empty());
        _imagesheets[key] = is;
    }
*/
    void Gear::addImagesheet(std::shared_ptr<Imagesheet> is)
    {
        assert(!is->getName().empty());
        _imagesheets[is->getName()] = is;
    }


    void Gear::removeImagesheet(const std::string& key)
    {
        _imagesheets.erase(key);
    }

    void Gear::removeImagesheets()
    {
        _imagesheets.clear();
    }

    std::shared_ptr<Imagesheet> Gear::getImagesheet(const std::string &name) const
    {
        try
        {
            return _imagesheets.at(name);
        }
        catch(std::out_of_range& oor)
        {
            return nullptr;
        }
    }

    void Gear::addLayer(std::shared_ptr<Layer> layer)
    {
        // if the layer is already added, do nothing
        auto layer_it = std::find(_layers.begin(), _layers.end(), layer);
        if (layer_it != _layers.end())
        {
            return;
        }

        _layers.push_front(layer);
        _layers.sort([] (const std::shared_ptr<Layer> &first, const std::shared_ptr<Layer> &second)
        {
            return first->_z_order < second->_z_order;
        });

    }

    void Gear::removeLayer(std::shared_ptr<Layer> layer)
    {
        _layers.remove(layer);
    }

    void Gear::removeLayers()
    {
        _layers.clear();
    }

    std::shared_ptr<Layer> Gear::getLayerByName(const std::string &name)
    {
        std::forward_list<std::shared_ptr<Layer>>::iterator layer_it = std::find_if(_layers.begin(), _layers.end(), [=](const std::shared_ptr<Layer>& val)
        {
            return (val->_name == name);
        });
        return (*layer_it);
    }

    RectF Gear::getActiveWorld() const
    {
        return RectF(_viewport.x - (_active_world_size.w - _viewport.w)/2, _viewport.y - ((_active_world_size.h - _viewport.h)/2), _active_world_size.w, _active_world_size.h);
    }

    void Gear::setViewport(const RectF &vp)
    {
        _viewport.w = std::min(_world.w, vp.w);
        _viewport.h = std::min(_world.h, vp.h);
        setViewportPos(vp.tl());
    }

    void Gear::setViewportPos(const Vector2F& pos)
    {
        if (pos.x < 0)
        {
            _viewport.x = 0;
        }
        else if (pos.x + _viewport.w > _world.w)
        {
            _viewport.x = _world.w - _viewport.w;
        }
        else
        {
            _viewport.x = pos.x;
        }

        if (pos.y < 0)
        {
            _viewport.y = 0;
        }
        else if (pos.y + _viewport.h > _world.h)
        {
            _viewport.y = _world.h - _viewport.h;
        }
        else
        {
            _viewport.y = pos.y;
        }

    }

    void Gear::follow(const Vector2F& dest)
    {
        Vector2F vp_pos = _viewport.tl();
        vp_pos.x += _viewport.w / 2;
        vp_pos.y += _viewport.h /2;
        Vector2F pos = vp_pos + (dest - vp_pos) / 20.0f;
        pos.x = pos.x - _viewport.w / 2;
        pos.y = pos.y - _viewport.h / 2;

        setViewportPos(pos);

    }



}