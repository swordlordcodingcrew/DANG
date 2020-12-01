// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework


#include "TmxExtruder.h"
#include "Imagesheet.h"
#include "CollisionSpriteLayer.h"
#include "SpriteLayer.h"
#include "TileLayer.h"
#include "Gear.h"

namespace dang
{



    TmxExtruder::TmxExtruder(tmx_level *lvl) : _level(lvl)
    {
    }

    /**
     * Extrude imagesheet from tmx_level with given name
     * @param name the name of the imagesheet (the corresponding tmx_tileset must have the same name)
     * @return image sheet as shared pointer
     */
    std::shared_ptr<Imagesheet> TmxExtruder::extrudeImagesheet(const std::string& name)
    {
        if (_level == nullptr)
        {
            return nullptr;
        }

        try
        {
            // find tileset by name. In the tileset the rows and cols are stored
            auto ts_it = std::find_if(_level->tilesets.begin(), _level->tilesets.end(), [=](const std::pair<uint8_t, tmx_tileset>& val)
            {
                return (val.second.name == name);
            });

            if (ts_it != _level->tilesets.end())
            {
                const uint8_t *image = _level->images.at(name);
                SizeU imgsh_size(ts_it->second.imageWidth, ts_it->second.imageHeight);
                std::shared_ptr<Imagesheet> is = std::make_shared<Imagesheet>(name, image, imgsh_size, ts_it->second.cols, ts_it->second.rows);
//                std::shared_ptr<Imagesheet> is = dang::Imagesheet::loadShared(image, nullptr, ts_it->second.cols, ts_it->second.rows);
                return is;
            }

            return nullptr;
        }
        catch (std::out_of_range &oor)
        {
            return nullptr;
        }

    }

    spSpriteLayer TmxExtruder::extrudeSpriteLayer(const std::string &name)
    {
        const tmx_objectlayer* tol = getTmxObjectLayer(name);

        if (tol == nullptr)
        {
            return nullptr;
        }

        spSpriteLayer sl = std::make_shared<SpriteLayer>();

        sl->_name = tol->name;
        // TODO js-exporter: implement zOrder
        sl->_z_order = 1;
        // TODO js-exporter: implement visible flag
        // sl->_visible = ola->visible;
        // TODO js-exporter: implement position
        // sl->_position = position

        return sl;

    }

    spCollisionSpriteLayer TmxExtruder::extrudeCollisionSpriteLayer(const std::string &name)
    {
        const tmx_objectlayer* tol = getTmxObjectLayer(name);

        if (tol == nullptr)
        {
            return nullptr;
        }

        spCollisionSpriteLayer sl = std::make_shared<CollisionSpriteLayer>();

        sl->_name = tol->name;
        // TODO js-exporter: implement zOrder
        sl->_z_order = 1;
        // TODO js-exporter: implement visible flag
        // csl->_visible = ola->visible;
        // TODO js-exporter: implement position
        // tl->_position = position

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
    spTileLayer TmxExtruder::extrudeTileLayer(const std::string &name, const Gear &gear)
    {
        const tmx_tilelayer* ttl = getTmxTileLayer(name);

        if (ttl == nullptr)
        {
            return nullptr;
        }

        // Fetch one tmx_tileset - this is not entirely correct since we assume that all tiles are in the same imagesheet.
        tmx_tileset& ts = _level->tilesets[ttl->tiles[0].tileset];

        spImagesheet is = gear.getImagesheet(ts.name);
        std::shared_ptr<TileLayer> tl = std::make_shared<TileLayer>(ts, *ttl, is, gear.getViewport());
        tl->_name = ttl->name;

        // TODO js-exporter: implement zOrder
        tl->_z_order = 0;
        // TODO js-exporter: implement visible flag
        // tl->_visible = tilel->visible;
        // TODO js-exporter: implement position
        // tl->_position = position

        return tl;

    }

    const tmx_objectlayer* TmxExtruder::getTmxObjectLayer(const std::string &name)
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
            // .. and is a tmx_objectlayer
            if (l_it->get()->type == ltObjects)
            {
                dang::tmx_objectlayer *ola = static_cast<tmx_objectlayer*>(l_it->get());
                return ola;
            }

        }

        return nullptr;
    }

    const tmx_tilelayer* TmxExtruder::getTmxTileLayer(const std::string &name)
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
            // .. and is a tmx_tilelayer
            if (l_it->get()->type == ltTile)
            {
                dang::tmx_tilelayer *tla = static_cast<tmx_tilelayer*>(l_it->get());
                return tla;
            }

        }

        return nullptr;
    }

}