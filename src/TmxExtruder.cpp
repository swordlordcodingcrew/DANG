// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework


#include "TmxExtruder.h"
#include "Imagesheet.h"
#include "CollisionSpriteLayer.h"
#include "SpriteLayer.h"


namespace dang
{

    using spLayer = std::shared_ptr<Layer>;
    using spSpriteLayer = std::shared_ptr<SpriteLayer>;
    using spImagesheet = std::shared_ptr<Imagesheet>;


    TmxExtruder::TmxExtruder(tmx_level *lvl) : _level(lvl)
    {
    }

    /**
     * Extrude imagesheet from tmx_level with name name
     * @param name the name of the imagesheet (the corresponding tmx_tileset must have the same name
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
            // find tileset by name
            auto ts_it = std::find_if(_level->tilesets.begin(), _level->tilesets.end(), [=](const std::pair<uint8_t, tmx_tileset>& val)
            {
                return (val.second.name == name);
            });

            if (ts_it != _level->tilesets.end())
            {
                const uint8_t *image = _level->images.at(name);
                std::shared_ptr<Imagesheet> is = dang::Imagesheet::loadShared(image, nullptr, ts_it->second.cols,
                                                                              ts_it->second.rows);
                return is;
            }

            return nullptr;
        }
        catch (std::out_of_range &oor)
        {
            return nullptr;
        }

    }



}