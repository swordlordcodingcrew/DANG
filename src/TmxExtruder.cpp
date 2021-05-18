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

    spSpriteLayer TmxExtruder::getSpriteLayer(const std::string& name, bool addSprites, bool addToGear)
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
                sl->addSprite(std::make_shared<Sprite>(so, is));
            }
        }

        if (addToGear)
        {
            _gear->addLayer(sl);
        }

        return sl;

    }

    void TmxExtruder::fillHUDLayer(spHUDLayer layer, const std::string& name, bool addSprites, bool addToGear)
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
                layer->addSprite(std::make_shared<Sprite>(so, is));
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

        TwAnim twa = TwAnim(frame_list, duration, ease_cb, loops, alternating, delay);
        spTwAnim ret = std::make_shared<TwAnim>(twa);
        return ret;

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

    spSceneGraph TmxExtruder::createPaths(RectF &room_extent)
    {
        spSceneGraph ret = std::make_shared<SceneGraph>();

        // first add all the waypoints
        for (size_t i = 0; i < _level->waypoints_len; ++i)
        {
            const tmx_waypoint* twap = _level->waypoints + i;
            if (room_extent.contains({twap->x, twap->y}))
            {
                spWaypoint spwap = std::make_shared<Waypoint>(twap->id, twap->x, twap->y, twap->type);
                ret->addWaypoint(spwap->_id, spwap);
            }
        }

        std::map<uint32_t, spWaypoint>& wapts = ret->getWaypoints();

        // second add the connections to the waypoints
        for (size_t j = 0; j < _level->waypoint_connections_len; ++j)
        {
            const tmx_waypoint_connection* twc = _level->waypoint_connections + j;
            if (wapts.count(twc->waypoint_start_id) > 0 && wapts.count(twc->waypoint_goal_id) > 0)
            {
                spWaypoint start = wapts.at(twc->waypoint_start_id);
                spWaypoint goal = wapts.at(twc->waypoint_goal_id);
                float dist = start->distanceTo(goal);
                start->addNeighbour(goal, dist, twc->connection_type);
            }
        }

        return ret;
    }

}