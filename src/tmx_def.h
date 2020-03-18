// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <unordered_map>
#include <forward_list>
#include <vector>
#include <memory>
//#include <32blit.hpp>
//#include "Imagesheet.h"
//#include "tween/TwAnim.h"

namespace dang
{
    class Imagesheet;

    struct tmx_tileset
    {
        std::string name; // this name is the reference to the image
        uint16_t tileCount;
        uint16_t tileWidth;
        uint16_t tileHeight;

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
        uint16_t id;
        uint16_t tileset;

        // actually values from the cell...
        uint8_t isFlippedHorizontally : 1;
        uint8_t isFlippedVertically : 1;
        uint8_t isFlippedAntiDiagonally : 1;
    };

    struct tmx_spriteobject
    {
        uint16_t id; // global
        //std::string name; // not needed, the reference contains the name
        std::string type;
        //std::string shape;
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;
        bool visible;
        uint16_t tileset;
        uint16_t tile;

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
        std::string name;
        float opacity;
        bool visible;
        uint8_t type; // 0 = tmx_tile, 1 = objects
        // TODO: to be considered
        //offset : point 	Offset in pixels that is applied when this tmx_layer is rendered.
        //map : TileMap 	Map that this tmx_layer is part of (or null in case of a standalone tmx_layer).
    };


    struct tmx_tilelayer : tmx_layer
    {
        uint8_t width;
        uint8_t height;
        std::vector<tmx_tile> tiles;

        tmx_tilelayer(std::string n, uint32_t size, tmx_tile _tiles[], uint8_t w, uint8_t h) : tmx_layer()
        {
            name = n;
            width = w;
            height = h;
            type = 0;
            tiles.assign(_tiles, _tiles + size);
        };
    };

    struct tmx_objectlayer : tmx_layer
    {
        std::vector<tmx_spriteobject> so;

        tmx_objectlayer(std::string n, uint16_t size, tmx_spriteobject _so[]) : tmx_layer()
        {
            name = n;
            type = 1;
            so.assign(_so, _so + size);
        };

        tmx_objectlayer(tmx_spriteobject _so) : tmx_layer()
        {
            type = 1;
            so.push_back(_so);
        };
    };


    struct tmx_world
    {
        uint16_t width = 0; // width in tiles
        uint16_t height = 0; // height in tiles

        uint16_t tileWidth = 0; // width of tiles
        uint16_t tileHeight = 0; // height of tiles

        uint16_t getWidthInPixel()
        {
            return width * tileWidth;
        }

        uint16_t getHeightInPixel()
        {
            return height * tileHeight;
        }
    };

    struct tmx_level
    {
        std::vector<std::shared_ptr<tmx_layer>> layers;
        std::unordered_map<uint8_t, tmx_tileset> tilesets;
        std::unordered_map<std::string, Imagesheet*> imagesheets;
//        std::unordered_map<std::string, std::shared_ptr<Imagesheet>> imagesheets;

        // TODO: sprites should know on which tmx_layer they belong...
        std::forward_list<std::shared_ptr<tmx_spriteobject>> sprites;

//        std::shared_ptr<sprite> hero;

        u_int16_t camX = 0;
        u_int16_t camY = 0;

        bool debug = false;

//        viewport vp;
        tmx_world w;

        std::shared_ptr<tmx_spriteobject> addSprite(tmx_spriteobject* s)
        {
            std::shared_ptr<tmx_spriteobject> s_p = std::make_shared<tmx_spriteobject>(*s);
            sprites.push_front(s_p);

            return s_p;
        }
    };
}
