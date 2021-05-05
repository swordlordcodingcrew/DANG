// (c) 2019-20 by SwordLord - the coding crew
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

namespace dang
{

    TmxExtruder::TmxExtruder(const tmx_level* lvl) : _level(lvl)
    {
    }

    void TmxExtruder::getImagesheets(Gear &gear)
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
            gear.addImagesheet(is);
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

    spSpriteLayer TmxExtruder::getSpriteLayer(const std::string& name, Gear& gear, bool addSprites, bool addToGear)
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

                spImagesheet is = gear.getImagesheet(so->tileset);
                sl->addSprite(std::make_shared<Sprite>(so, is));
            }
        }

        if (addToGear)
        {
            gear.addLayer(sl);
        }

        return sl;


/*        const std::shared_ptr<tmx_objectlayer> tol = getTmxObjectLayer(name);

        if (tol == nullptr)
        {
            return nullptr;
        }

        spSpriteLayer sl = std::make_shared<SpriteLayer>();

        sl->_name = tol->name;
        sl->_z_order = tol->z_order;

        // TODO js-exporter: implement visible flag
        // sl->_visible = ola->visible;
        // TODO js-exporter: implement position
        // sl->_position = position

        if (addSprites)
        {
            for (const dang::tmx_spriteobject &so : getSOList(sl))
            {
                spImagesheet is = gear.getImagesheet(_level->tilesets[so.tileset].name);
                sl->addSprite(std::make_shared<Sprite>(so, is));
            }
        }

        if (addToGear)
        {
            gear.addLayer(sl);
        }

        return sl;
*/    }

    void TmxExtruder::fillHUDLayer(spHUDLayer layer, const std::string& name, Gear& gear, bool addSprites, bool addToGear)
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

/*        const std::shared_ptr<tmx_objectlayer> tol = getTmxObjectLayer(name);

        if (tol == nullptr)
        {
            return;
        }
*/
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

                spImagesheet is = gear.getImagesheet(so->tileset);
                layer->addSprite(std::make_shared<Sprite>(so, is));
            }

/*            for (const dang::tmx_spriteobject &so : getSOList(layer))
            {
                spImagesheet is = gear.getImagesheet(_level->tilesets[so.tileset].name);
                layer->addSprite(std::make_shared<Sprite>(so, is));
            }
*/        }

        if (addToGear)
        {
            gear.addLayer(layer);
        }
    }

    spCollisionSpriteLayer TmxExtruder::getCollisionSpriteLayer(const std::string &name, Gear& gear, bool addSprites, bool addToGear)
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

/*        const std::shared_ptr<tmx_objectlayer> tol = getTmxObjectLayer(name);

        if (tol == nullptr)
        {
            return nullptr;
        }
*/
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

                spImagesheet is = gear.getImagesheet(so->tileset);
                sl->addCollisionSprite(std::make_shared<CollisionSprite>(so, is));
            }

/*            for (const dang::tmx_spriteobject &so : getSOList(sl))
            {
                spImagesheet is = gear.getImagesheet(_level->tilesets[so.tileset].name);
                sl->addCollisionSprite(std::make_shared<CollisionSprite>(so, is));
            }
*/        }

        if (addToGear)
        {
            gear.addLayer(sl);
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
    spTileLayer TmxExtruder::getTileLayer(const std::string &name, Gear &gear, bool addToGear)
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

/*        const std::shared_ptr<tmx_tilelayer> ttl = getTmxTileLayer(name);

        if (ttl == nullptr)
        {
            return nullptr;
        }
*/
        const tmx_tileset* ts = getTileset(l->tl_tileset);

        if (ts == nullptr)
        {
            return nullptr;
        }

        // Fetch one tmx_tileset - assuming that all tiles are in the same imagesheet.
//        tmx_tileset& ts = _level->tilesets[ttl->tiles[0].tileset];

        spImagesheet is = gear.getImagesheet(ts->name);
        std::shared_ptr<TileLayer> tl = std::make_shared<TileLayer>(ts, l, is, gear.getViewport());
        tl->_name = l->name;
        tl->_z_order = l->z_order;
        tl->_visible = l->visible;
        tl->_tmx_layer = l;

        // TODO js-exporter: implement position
        // tl->_position = position

        if (addToGear)
        {
            gear.addLayer(tl);
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


/*    const std::shared_ptr<tmx_objectlayer> TmxExtruder::getTmxObjectLayer(const std::string &name)
    {
        if (_level == nullptr)
        {
            return nullptr;
        }

        auto l_it = std::find_if(_level->layers.begin(), _level->layers.end(), [=](const std::shared_ptr<dang::tmx_layer>& val)
        {
            return (val->name == name);
        });

        // tmx_layer found..
        if (l_it != _level->layers.end())
        {
            return (*l_it)->type == ltObjects ? std::static_pointer_cast<tmx_objectlayer>(*l_it) : nullptr;
        }

        return nullptr;
    }

    const std::shared_ptr<tmx_tilelayer> TmxExtruder::getTmxTileLayer(const std::string &name)
    {
        if (_level == nullptr)
        {
            return nullptr;
        }

        auto l_it = std::find_if(_level->layers.begin(), _level->layers.end(), [=](const std::shared_ptr<dang::tmx_layer>& val)
        {
            return (val->name == name);
        });

        // tmx_layer found..
        if (l_it != _level->layers.end())
        {
            return (*l_it)->type == ltTile ? std::static_pointer_cast<tmx_tilelayer>(*l_it) : nullptr;
        }

        return nullptr;
    }
*/

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


/*        try
        {
            const tmx_tileanimation& ta = _level->tileanimation.at(is_name + "_" + anim_name);
            std::vector<uint16_t> frame_list;
            uint32_t duration = 0;
            for (auto f : ta.frames)
            {
                frame_list.push_back(f.tileId);
                duration += f.duration;
            }

            TwAnim twa = TwAnim(frame_list, duration, ease_cb, loops, alternating, delay);
            spTwAnim ret = std::make_shared<TwAnim>(twa);
            return ret;
        }
        catch (std::out_of_range& oor)
        {
            return nullptr;
        }
*/
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


/*    const std::vector<tmx_spriteobject> &TmxExtruder::getSOList(const spSpriteLayer& sl)
    {
        const std::shared_ptr<tmx_objectlayer> ola = getTmxObjectLayer(sl->_name);
        return ola->so;
    }
*/
}