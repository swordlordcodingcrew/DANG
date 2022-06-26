// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "Gear.hpp"
#include "src/layer/Layer.hpp"
#include "Sprite.hpp"
#include "Imagesheet.hpp"
#include "TmxExtruder.hpp"
#include "SpriteLayer.hpp"
#include "src/layer/TileLayer.hpp"
#include "CollisionSpriteLayer.hpp"
#include "CollisionSprite.hpp"
#include "bt/NTree.h"

#include <cassert>
#include <algorithm>
#include <src/snd/SndGear.hpp>
//#include <iostream>

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
        for (spLayer l : _layers)
        {
            if (l->visible())
            {
                l->render(*this);
            }
        }

        if (_fade_overlay)
        {
            blit::screen.pen = _fade_colour;
            blit::screen.rectangle({0,0, blit::screen.bounds.w, blit::screen.bounds.h});
        }

    }

    void Gear::update(uint32_t dt)
    {
        // let it rumble
        SndGear::updateRumble();

        for (spLayer l : _layers)
        {
            if (l->active())
            {
                l->update(dt, *this);
            }
        }

        if (_fade_overlay)
        {
            fadeForward();
        }
    }

/*    BTNode::Status Gear::runNTree(const spCollisionSprite& s) const
    {
        assert(s != nullptr);

        spNTreeState ts = s->getNTreeState();
        assert(ts != nullptr);
        assert(ts->_tree != nullptr);

        return ts->_tree->process(s, ts);
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

    void Gear::addNTree(const std::string& name, spNTree bt)
    {
        assert(!name.empty());
        _nTree[name] = move(bt);
    }

    void Gear::removeNTree(const std::string& key)
    {
        _nTree.erase(key);
    }

    void Gear::removeNTrees()
    {
        _nTree.clear();
    }

    spNTree Gear::getNTree(const std::string &name) const
    {
        // there are no empty named behaviour trees
        if(name.length() == 0)
        {
            return nullptr;
        }

        std::unordered_map<std::string, spNTree>::const_iterator it = _nTree.find (name);

        if ( it == _nTree.end() )
        {
            return nullptr;
        }
        else
        {
            return it->second;
        }
    }

    void Gear::addLayer(spLayer layer)
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
            return first->zOrder() < second->zOrder();
        });

    }

    void Gear::removeLayer(const spLayer& layer)
    {
        _layers.remove(layer);
    }

    void Gear::removeLayers()
    {
        _layers.clear();
    }

    void Gear::setLayerVisibility(const std::string &name, bool visible)
    {
        auto layer_it = std::find_if(_layers.begin(), _layers.end(), [=](const std::shared_ptr<Layer>& val)
        {
            return (val->name() == name);
        });

        assert(layer_it != _layers.end());

        (*layer_it)->setVisible(visible);
    }

    void Gear::setLayersVisibility(bool visible)
    {
        for (auto& lit : _layers)
        {
            (*lit).setVisible(visible);
        }
    }

    void Gear::setLayerActive(const std::string &name, bool active)
    {
        auto layer_it = std::find_if(_layers.begin(), _layers.end(), [=](const std::shared_ptr<Layer>& val)
        {
            return (val->name() == name);
        });

        assert(layer_it != _layers.end());

        (*layer_it)->setActive(active);

    }

    void Gear::setLayersActive(bool active)
    {
        for (auto& lit : _layers)
        {
            (*lit).setActive(active);
        }

    }

    spLayer Gear::getLayerByName(const std::string &name)
    {
        auto layer_it = std::find_if(_layers.begin(), _layers.end(), [=](const std::shared_ptr<Layer>& val)
        {
            return (val->name() == name);
        });

/*        if (layer_it == _layers.end())
        {
            return nullptr;
        }
*/
        return (*layer_it);
    }

    spLayer Gear::getLayerByTypename(dang::Layer::E_TYPE type)
    {
        auto layer_it = std::find_if(_layers.begin(), _layers.end(), [=](const std::shared_ptr<Layer>& val)
        {
            return (val->getType() == type);
        });

        if (layer_it == _layers.end())
        {
            return nullptr;
        }

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

    void Gear::fade(const blit::Pen &col, uint8_t fade_step, const std::function<void (void)>& cb)
    {
        if (_fade_overlay)
        {
            // already fading
            return;
        }

        _fade_overlay = true;
        _fade_step = fade_step > 0 ? fade_step : 8;
        _fade_colour = col;
        _fade_colour.a = 0;
        _faded_cb = cb;
        _fade_state = NONE;

    }

    void Gear::fadeForward()
    {
        switch (_fade_state)
        {
            case NONE:
            {
                _fade_state = FADE_TO_COL;
                break;
            }
            case FADE_TO_COL:
            {
                if (_fade_colour.a < 255)
                {
                    _fade_colour.a = _fade_colour.a + _fade_step > 255 ? 255 : _fade_colour.a + _fade_step;
                }
                else
                {
                    if (_faded_cb)
                    {
                        _faded_cb();
                    }
                    _fade_state = FADE_TO_TRANS;
                }
                break;
            }
            case FADE_TO_TRANS:
            {
                if (_fade_colour.a > 0)
                {
                    _fade_colour.a = _fade_colour.a - _fade_step < 0 ? 0 : _fade_colour.a - _fade_step;
                }
                else
                {
                    _fade_state = END_OF_FADE;
                    _fade_overlay = false;
                    _fade_step = 8;
                    _fade_colour = {0,0,0, 0};
                    _faded_cb = nullptr;
                }
                break;
            }
        }
    }

}