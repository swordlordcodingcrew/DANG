// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <cassert>
#include <algorithm>
#include "Gear.hpp"
#include "Layer.hpp"
#include "Sprite.hpp"
#include "Imagesheet.hpp"
#include "TmxExtruder.hpp"
#include "SpriteLayer.hpp"
#include "TileLayer.hpp"
#include "CollisionSpriteLayer.hpp"

namespace dang
{

    Gear::Gear()
    {
    }

    Gear::~Gear()
    {
    }

    void Gear::initLevel(const tmx_level* lvl, RectF& viewport)
    {
        assert(lvl != nullptr);

        _world.w = lvl->w->width * lvl->w->tileWidth;
        _world.h = lvl->w->height * lvl->w->tileHeight;

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

    dang::BTNodeStatus Gear::runBehaviourTree(std::shared_ptr<TreeState> ts, std::shared_ptr<Sprite> s) const
    {
        // TODO refactor
        try
        {
            return _behaviourTree.at("insanity")->process(ts, s);
        }
        catch(std::out_of_range& oor)
        {
            return BTNodeStatus::FAILURE;
        }
    }

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

    std::shared_ptr<Imagesheet> Gear::getImagesheet(const std::string& name) const
    {
        // there are no empty named imagesheets, must be a hotrecht or something
        if(name.length() == 0)
        {
            return nullptr;
        }

        std::unordered_map<std::string, spImagesheet>::const_iterator it = _imagesheets.find (name);

        if ( it == _imagesheets.end() )
        {
            return nullptr;
        }
        else
        {
            return it->second;
        }
    }

    void Gear::addBehaviourTree(const std::string& name, std::shared_ptr<BehaviourTree> bt)
    {
        assert(!name.empty());
        _behaviourTree[name] = move(bt);
    }


    void Gear::removeBehaviourTree(const std::string& key)
    {
        _behaviourTree.erase(key);
    }

    void Gear::removeBehaviourTrees()
    {
        _behaviourTree.clear();
    }

    std::shared_ptr<BehaviourTree> Gear::getBehaviourTree(const std::string &name) const
    {
        // there are no empty named behaviour trees
        if(name.length() == 0)
        {
            return nullptr;
        }

        std::unordered_map<std::string, spBehaviourTree>::const_iterator it = _behaviourTree.find (name);

        if ( it == _behaviourTree.end() )
        {
            return nullptr;
        }
        else
        {
            return it->second;
        }
    }

    void Gear::addLayer(std::shared_ptr<Layer> layer)
    {
        assert(layer != nullptr);

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
        auto layer_it = std::find_if(_layers.begin(), _layers.end(), [=](const std::shared_ptr<Layer>& val)
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