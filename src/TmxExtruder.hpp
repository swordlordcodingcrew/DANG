// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "tween/Ease.hpp"
#include "BaseHUDLayer.hpp"
#include "ImageImport.h"

#include <unordered_map>
#include <forward_list>
#include <vector>
#include <memory>

namespace dang
{
    class Imagesheet;

    struct tmx_tileset
    {
        const std::string name; // this name is the reference to the image
        const image_import* image;  // ref to image
        const uint16_t tileCount;
        const uint16_t tileWidth;
        const uint16_t tileHeight;
        const uint16_t imageWidth;
        const uint16_t imageHeight;
        const uint16_t cols;
        const uint16_t rows;

        /*
        TODO: to be considered
        backgroundColor : color 	Background color for this tmx_tileset in the Tilesets view.
        tileSpacing : int [read‑only] 	Spacing between tiles in this tmx_tileset in pixels.
        margin : int [read‑only] 	Margin around the tmx_tileset in pixels (only used at the top and left sides of the tmx_tileset image).
        tileOffset : point 	Offset in pixels that is applied when tiles from this tmx_tileset are rendered.
        */
    };

    struct tmx_tile
    {
        const uint16_t id;
//        const uint16_t tileset;
        // 32blit transform code
        const uint8_t transform;
    };

    struct tmx_frame
    {
        const uint16_t tileId; // relative to the tileset
        const uint16_t duration;
    };

    struct tmx_tileanimation
    {
        const std::string tileset; // reference to the tileset id
        const uint16_t tile; // reference to the tile
        const std::string name; // name of the animation to be referenced in the source
        const std::vector<tmx_frame> frames;
    };

    struct tmx_spriteobject
    {
        const uint16_t id; // global
        const std::string name;
        const std::string type;
        const int32_t x;
        const int32_t y;
        const int32_t width;
        const int32_t height;
        const bool visible;
        const std::string tileset;
        const uint16_t tile;

        /*
        // TODO: to be considered
         rotation : number 	Rotation of the object in degrees clockwise.
        polygon : Polygon 	Polygon of the object.
        text : string 	The text of a text object.
        font : Font 	The font of a text object.
        textAlignment : Alignment 	The alignment of a text object.
        wordWrap : bool 	Whether the text of a text object wraps based on the width of the object.
        textColor : color 	Color of a text object.
        tileFlippedHorizontally : bool 	Whether the tmx_tile is flipped horizontally.
        tileFlippedVertically : bool 	Whether the tmx_tile is flipped vertically.
        */
    };


    enum tmx_layerType{ltTile, ltObjects};

    struct tmx_layer
    {
        const std::string name;
        const uint8_t type; // 0 = tmx_tile, 1 = objects
        const float opacity;
        const bool visible;
        const uint32_t z_order;   // the z-order of the layers

        // tile layer vars
        const uint8_t tl_width;
        const uint8_t tl_height;
        const std::string tl_tileset;
        const tmx_tile* tl_tiles;
        const size_t tl_tiles_len;

        // object layer vars
        const tmx_spriteobject* spriteobjects;
        const size_t spriteobejcts_len;

        // TODO: to be considered
        //offset : point 	Offset in pixels that is applied when this tmx_layer is rendered.
        //map : TileMap 	Map that this tmx_layer is part of (or null in case of a standalone tmx_layer).
    };


    struct tmx_world
    {
        const uint16_t width = 0; // width in tiles
        const uint16_t height = 0; // height in tiles

        const uint16_t tileWidth = 0; // width of tiles
        const uint16_t tileHeight = 0; // height of tiles
    };

    struct tmx_level
    {
        const tmx_world* w;
        const tmx_tileset* tilesets;
        const size_t tilesets_len;

        const tmx_tileanimation* tileanimations;
        const size_t tileanimations_len;

        const tmx_layer* layers;
        const size_t layers_len;

/*        std::vector<std::shared_ptr<tmx_layer>> layers;

        // TODO: sprites should know on which tmx_layer they belong...
        std::forward_list<std::shared_ptr<tmx_spriteobject>> sprites;

        std::shared_ptr<tmx_spriteobject> addSprite(tmx_spriteobject* s)
        {
            std::shared_ptr<tmx_spriteobject> s_p = std::make_shared<tmx_spriteobject>(*s);
            sprites.push_front(s_p);

            return s_p;
        }
*/    };

    // forward declarations
    class Layer;
    class SpriteLayer;
    class CollisionSpriteLayer;
    class TileLayer;
    class Gear;
    class TwAnim;

    // using aliases
    using spLayer = std::shared_ptr<Layer>;
    using spSpriteLayer = std::shared_ptr<SpriteLayer>;
    using spHUDLayer = std::shared_ptr<BaseHUDLayer>;
    using spCollisionSpriteLayer = std::shared_ptr<CollisionSpriteLayer>;
    using spTileLayer = std::shared_ptr<TileLayer>;
    using spImagesheet = std::shared_ptr<Imagesheet>;
    using spTwAnim = std::shared_ptr<TwAnim>;


    /**
     * tmx extruder class gets or extrudes objects from the tmx-derived include-file with the DANG Extruder pattern
     */
    class TmxExtruder
    {

    public:
        TmxExtruder() = default;
        explicit TmxExtruder(const tmx_level* lvl);
        ~TmxExtruder() = default;

        spImagesheet            getImagesheet(const std::string& name);
        void                    getImagesheets(Gear& gear);

        spSpriteLayer           getSpriteLayer(const std::string& name, Gear& gear, bool addSprites, bool addToGear);
        spCollisionSpriteLayer  getCollisionSpriteLayer(const std::string& name, Gear& gear, bool addSprites, bool addToGear);
        void                    fillHUDLayer(spHUDLayer layer, const std::string& name, Gear& gear, bool addSprites, bool addToGear);
        spTileLayer             getTileLayer(const std::string& name, Gear& gear, bool addToGear);
        spTwAnim                getAnimation(const std::string& is_name, const std::string& anim_name, EaseFn ease_cb = Ease::Linear, int32_t loops = -1, bool alternating = false, uint32_t delay = 0);

//        const std::vector<tmx_spriteobject>& getSOList(const spSpriteLayer& sl);

        const tmx_layer* getTmxLayer(const std::string &name);
        const tmx_tileset*  getTileset(const std::string &name);
        const tmx_tileanimation* getTileAnimation(const std::string &tileset, const std::string& name);

//        const std::shared_ptr<tmx_objectlayer>     getTmxObjectLayer(const std::string &name);
//        const std::shared_ptr<tmx_tilelayer>     getTmxTileLayer(const std::string &name);


    protected:
        const tmx_level*  _level{nullptr};
    };

}



