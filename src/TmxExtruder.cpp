// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework


#include "tween/Ease.hpp"
#include "tween/TwAnim.hpp"
#include "TmxExtruder.hpp"
#include "ImageImport.h"
#include "Imagesheet.hpp"
#include "CollisionSpriteLayer.hpp"
#include "SpriteLayer.hpp"
#include "TileLayer.hpp"
#include "Gear.hpp"
#include "Sprite.hpp"
#include "CollisionSprite.hpp"
#include "BaseHUDLayer.hpp"
#include "path/SceneGraph.hpp"
#include "path/Wavepoint.hpp"

#include <iostream>

namespace dang
{

    TmxExtruder::TmxExtruder(const tmx_level* lvl, dang::Gear* gear) : _level(lvl), _gear(gear)
    {
    }

    void TmxExtruder::getImagesheets()
    {
        if (_level == nullptr)
        {
            return;
        }

        for (size_t i = 0; i < _level->tilesets_len; i++)
        {
            const tmx_tileset* ts = _level->tilesets + i;
            const image_import* ii = ts->image;
            std::shared_ptr<Imagesheet> is = std::make_shared<Imagesheet>(ts->name, ii, ts->cols, ts->rows);
            _gear->addImagesheet(is);
        }
    }

    /**
     * Extrude imagesheet from tmx_level with given name
     * @param name the name of the imagesheet (the corresponding tmx_tileset must have the same name)
     * @return image sheet as shared pointer
     */
    std::shared_ptr<Imagesheet> TmxExtruder::getImagesheet(const std::string& name)
    {
        if (_level == nullptr)
        {
            return nullptr;
        }

        const tmx_tileset* ts = getTileset(name);

        if (ts == nullptr)
        {
            return nullptr;
        }

        std::shared_ptr<Imagesheet> is = std::make_shared<Imagesheet>(ts->name, ts->image, ts->cols, ts->rows);

        return is;
    }

    spSpriteLayer TmxExtruder::getSpriteLayer(const std::string& name, bool addSprites, bool addToGear, bool autoFillAnimations)
    {
        const tmx_layer* l = getTmxLayer(name);

        if (l == nullptr)
        {
            return nullptr;
        }

        if (l->type != tmx_layerType::ltObjects)
        {
            return nullptr;
        }

        spSpriteLayer sl = std::make_shared<SpriteLayer>();

        sl->_name = l->name;
        sl->_z_order = l->z_order;
        sl->_visible = l->visible;
        sl->_tmx_layer = l;

        // to be considered whether useful
        // sl->spriteobjects = l->spriteobjects

        // TODO js-exporter: implement position
        // sl->_position = position

        if (addSprites)
        {
            for (size_t j = 0; j < l->spriteobejcts_len; j++)
            {
                const tmx_spriteobject* so = l->spriteobjects + j;

                spImagesheet is = _gear->getImagesheet(so->tileset);

                auto sprite = std::make_shared<Sprite>(so, is);

                if(autoFillAnimations && is != nullptr && !so->type.empty())
                {
                    auto animation = getAnimation(is->getName(), so->type);
                    if(animation != nullptr)
                    {
                        sprite->setAnimation(animation);
                    }
                }

                sl->addSprite(sprite);
            }
        }

        if (addToGear)
        {
            _gear->addLayer(sl);
        }

        return sl;

    }

    void TmxExtruder::fillHUDLayer(spBaseHUDLayer layer, const std::string& name, bool addSprites, bool addToGear)
    {
        const tmx_layer* l = getTmxLayer(name);

        if (l == nullptr)
        {
            return;
        }

        if (l->type != tmx_layerType::ltObjects)
        {
            return;
        }

        layer->_name = l->name;
        layer->_z_order = l->z_order;
        layer->_visible = l->visible;
        layer->_tmx_layer = l;

        // TODO js-exporter: implement position
        // sl->_position = position

        if (addSprites)
        {
            for (size_t j = 0; j < l->spriteobejcts_len; j++)
            {
                const tmx_spriteobject* so = l->spriteobjects + j;

                spImagesheet is = _gear->getImagesheet(so->tileset);
                auto spr = std::make_shared<Sprite>(so, is);
                if(spr->_type_name == "hud_boss" || spr->_type_name == "hud_boss_health")
                {
                    spr->_visible = false; // as long as no boss battle is running
                }
                layer->addSprite(spr);
            }

        }

        if (addToGear)
        {
            _gear->addLayer(layer);
        }
    }

    spCollisionSpriteLayer TmxExtruder::getCollisionSpriteLayer(const std::string &name, bool addSprites, bool addToGear)
    {
        const tmx_layer* l = getTmxLayer(name);

        if (l == nullptr)
        {
            return nullptr;
        }

        if (l->type != tmx_layerType::ltObjects)
        {
            return nullptr;
        }

        spCollisionSpriteLayer sl = std::make_shared<CollisionSpriteLayer>();

        sl->_name = l->name;
        sl->_z_order = l->z_order;
        sl->_visible = l->visible;
        sl->_tmx_layer = l;

        // TODO js-exporter: implement position
        // tl->_position = position

        if (addSprites)
        {
            for (size_t j = 0; j < l->spriteobejcts_len; j++)
            {
                const tmx_spriteobject* so = l->spriteobjects + j;

                spImagesheet is = _gear->getImagesheet(so->tileset);
                sl->addCollisionSprite(std::make_shared<CollisionSprite>(so, is));
            }

        }

        if (addToGear)
        {
            _gear->addLayer(sl);
        }

        return sl;

    }

    /**
     * Extrudes tilelayer with name @param name from the tmx-struct. @param gear is needed to fetch the Imagesheet.
     * If the imagsheet is not present in @param gear it will be extrudes also.
     *
     * @param name name of the tilelayer
     * @param gear the gear
     * @return shared ptr to the extruded tilelayer or null
     */
    spTileLayer TmxExtruder::getTileLayer(const std::string &name, bool addToGear)
    {
        const tmx_layer* l = getTmxLayer(name);

        if (l == nullptr)
        {
            return nullptr;
        }

        if (l->type != tmx_layerType::ltTile)
        {
            return nullptr;
        }

        const tmx_tileset* ts = getTileset(l->tl_tileset);

        if (ts == nullptr)
        {
            return nullptr;
        }

        spImagesheet is = _gear->getImagesheet(ts->name);
        std::shared_ptr<TileLayer> tl = std::make_shared<TileLayer>(ts, l, is, _gear->getViewport());
        tl->_name = l->name;
        tl->_z_order = l->z_order;
        tl->_visible = l->visible;
        tl->_tmx_layer = l;

        // TODO js-exporter: implement position
        // tl->_position = position

        if (addToGear)
        {
            _gear->addLayer(tl);
        }

        return tl;

    }

    const tmx_layer* TmxExtruder::getTmxLayer(const std::string &name)
    {
        if (_level == nullptr)
        {
            return nullptr;
        }

        for (size_t i = 0; i < _level->layers_len; i++)
        {
            const tmx_layer* l = _level->layers + i;
            if (name == l->name)
            {
                return l;
            }
        }

        return nullptr;
    }

    [[depracated("use getAnimation with param 'const spImagesheet &is' instead")]]
    spTwAnim TmxExtruder::getAnimation(const std::string &is_name, const std::string &anim_name, EaseFn ease_cb,
                                       int32_t loops, bool alternating, uint32_t delay)
    {
        if (_level == nullptr)
        {
            return nullptr;
        }

        const tmx_tileanimation* ta = getTileAnimation(is_name, anim_name);

        if (ta == nullptr)
        {
            return nullptr;
        }

        std::vector<uint16_t> frame_list;
        uint32_t duration = 0;
        for (auto f : ta->frames)
        {
            frame_list.push_back(f.tileId);
            duration += f.duration;
        }

        spTwAnim twa = std::make_shared<TwAnim>(frame_list, duration, ease_cb, loops, alternating, delay);
        return twa;
    }

    spTwAnim TmxExtruder::getAnimation(const spImagesheet &is, const std::string &anim_name, EaseFn ease_cb,
                                       int32_t loops, bool alternating, uint32_t delay)
    {
        if (_level == nullptr)
        {
            return nullptr;
        }

        const tmx_tileanimation* ta = getTileAnimation(is->getName(), anim_name);

        if (ta == nullptr)
        {
            return nullptr;
        }

        std::vector<uint16_t> frame_list;
        uint32_t duration = 0;
        for (auto f : ta->frames)
        {
            frame_list.push_back(f.tileId);
            duration += f.duration;
        }

        spTwAnim twa = std::make_shared<TwAnim>(is, frame_list, duration, ease_cb, loops, alternating, delay);
        return twa;
    }

    const tmx_tileset* TmxExtruder::getTileset(const std::string &name)
    {
        for (size_t i = 0; i < _level->tilesets_len; i++)
        {
            const tmx_tileset* ts = _level->tilesets + i;
            if (name == ts->name)
            {
                return ts;
            }
        }

        return nullptr;
    }

    const tmx_tileanimation *TmxExtruder::getTileAnimation(const std::string &tileset, const std::string &name)
    {
        for (size_t i = 0; i < _level->tileanimations_len; i++)
        {
            const tmx_tileanimation* ta = _level->tileanimations + i;
            if (tileset == ta->tileset && name == ta->name)
            {
                return ta;
            }
        }

        return nullptr;

    }

    void TmxExtruder::createSceneGraphs(RectF &room_extent, std::vector<dang::spSceneGraph>& scene_graphs)
    {
        spSceneGraph sg = std::make_shared<SceneGraph>();

        //!< first add all the waypoints
        for (size_t i = 0; i < _level->waypoints_len; ++i)
        {
            const tmx_waypoint* twap = _level->waypoints + i;
            if (room_extent.contains({twap->x, twap->y}))
            {
                sg->addWaypoint(twap->id, twap->x, twap->y, twap->type);
            }
        }

        //!< if there are no waypoints, leave
        if (sg->getWaypoints().empty())
        {
            return;
        }

        //!< second add the connections to the waypoints
        for (size_t j = 0; j < _level->waypoint_connections_len; ++j)
        {
            const tmx_waypoint_connection* twc = _level->waypoint_connections + j;
            sg->addNeighbour(twc->waypoint_start_id, twc->waypoint_goal_id, twc->connection_type);
        }

        //!< check if there are disconnected graphs, and if so, split them into separate scenegraphs (nasty work)
        const std::unordered_map<uint32_t, Waypoint>& wps = sg->getWaypoints();
        std::unordered_map<uint32_t, bool> visited;
        for (const auto& wp : wps)
        {
            visited[wp.first] = false;
        }

        sg->dfsRecursion(&wps.begin()->second, visited);
        size_t nr = std::count_if(visited.begin(), visited.end(), [](auto a)
        {
            return a.second;
        });

        if (nr == wps.size())
        {
            scene_graphs.push_back(sg);
            return;
        }

//        std::cout << "disconnected graph" << std::endl;

        scene_graphs.push_back(sg->split(visited));

        bool wps_empty = wps.empty();
        while (!wps_empty)
        {
            const std::unordered_map<uint32_t, Waypoint>& wpsx = sg->getWaypoints();

            visited.clear();
            for (const auto& wp : wpsx)
            {
                visited[wp.first] = false;
            }

            sg->dfsRecursion(&wpsx.begin()->second, visited);

            scene_graphs.push_back(sg->split(visited));

            wps_empty = wpsx.empty();
        }


        //!< the waypoints in the split graphs have to be connected again
        for (size_t j = 0; j < _level->waypoint_connections_len; ++j)
        {
            const tmx_waypoint_connection* twc = _level->waypoint_connections + j;
            for (auto sgit : scene_graphs)
            {
                sgit->addNeighbour(twc->waypoint_start_id, twc->waypoint_goal_id, twc->connection_type);
            }
        }

    }

    void TmxExtruder::createWaves(RectF &room_extent, std::unordered_map<uint32_t, dang::Wavepoint> &waves)
    {
        // first add all the wavepoints
        for (size_t i = 0; i < _level->wavepoints_len; ++i)
        {
            const tmx_wavepoint* w = _level->wavepoints + i;
            if (room_extent.contains({w->x, w->y}))
            {
                Wavepoint wp(w->id, Vector2F(w->x, w->y), w->next_wavepoint_id, uint32_t(w->duration*1000), w->orientation, uint32_t(w->delay*1000));
                waves[w->id] = wp;
            }
        }

        // resolve id to pointer
        for (auto& wit : waves)
        {
            uint32_t id = wit.second._next_id;
            if (waves.count(id) > 0)
            {
                wit.second._next = &waves[id];
            }
        }
    }


}