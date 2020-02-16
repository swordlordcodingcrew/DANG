// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

/*
 * Install Packages:
- sudo apt install ffmpeg
- sudo apt install libavutil-dev
- sudo apt install libavformat-dev
- sudo apt install libswscale-dev
 */

#pragma once

#include <unordered_map>
#include <forward_list>
#include <vector>
#include <memory>
//#include <32blit.hpp>
//#include "Imagesheet.h"
#include "tween/TwAnim.h"

namespace dang {

    class Imagesheet;

    typedef blit::Point point;
    typedef blit::Rect  rect;
    typedef blit::Size  Size;

    enum collisionResponse{none, touch_me, cross_me, slide_over_me, bounce_off_me};

    // #include <globals>.h
    // TODO: move to globals
    struct tileset
    {
        std::string name; // this name is the reference to the image
        uint16_t tileCount;
        uint16_t tileWidth;
        uint16_t tileHeight;

        /*
        TODO: to be considered
        backgroundColor : color 	Background color for this tileset in the Tilesets view.
        tileSpacing : int [read‑only] 	Spacing between tiles in this tileset in pixels.
        margin : int [read‑only] 	Margin around the tileset in pixels (only used at the top and left sides of the tileset image).
        tileOffset : point 	Offset in pixels that is applied when tiles from this tileset are rendered.
        */
    };

    // TODO: move to globals
    struct tile
    {
        uint16_t id;
        uint16_t tileset;

        // actually values from the cell...
        uint8_t isFlippedHorizontally : 1;
        uint8_t isFlippedVertically : 1;
        uint8_t isFlippedAntiDiagonally : 1;
    };

    // TODO: add properties
    // Object.properties() : object
    // TODO: move to globals
    struct spriteobject
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

        uint16_t    _img_index{0};

        /*
        // TODO: to be considered
         rotation : number 	Rotation of the object in degrees clockwise.
        polygon : Polygon 	Polygon of the object.
        text : string 	The text of a text object.
        font : Font 	The font of a text object.
        textAlignment : Alignment 	The alignment of a text object.
        wordWrap : bool 	Whether the text of a text object wraps based on the width of the object.
        textColor : color 	Color of a text object.
        tileFlippedHorizontally : bool 	Whether the tile is flipped horizontally.
        tileFlippedVertically : bool 	Whether the tile is flipped vertically.
        */
    };

    struct sprite : spriteobject
    {
        int8_t   velX = 0;
        int8_t   velY = 0;
        bool    isHit = false;

    protected:
        std::forward_list<std::shared_ptr<Tweenable>> _tweens;

    public:
        sprite(uint16_t id, std::string type, uint16_t x, uint16_t y, uint16_t width, uint16_t height, bool visible, uint16_t tileset, uint16_t tile)
        {
            this->id = id;
            this->type = type;
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->visible = visible;
            this->tileset = tileset;
            this->tile = tile;
        }

        uint16_t wantToCollideWith(std::shared_ptr<sprite> other)
        {
            if(this->type == "coin")
            {
                return cross_me;
            }
            /*else if(this->type == "hotrect")
            {
                return slide_over_me;
            }*/
            else if(other->type == "hero")
            {
                return slide_over_me;
            }
            else if(this->type == "hero")
            {
                return touch_me;
            }

            return 0;
        }

        void addTween(std::shared_ptr<Tweenable> tw)
        {
            _tweens.push_front(tw);
        }

        void updateTweens(uint32_t time)
        {
            for (std::shared_ptr<Tweenable> const& tw : _tweens)
            {
                tw->update(time);

                // TODO: does this work?
/*                if (tw->is_tw_finished())
                {
                    _tweens.remove(tw);
                }
*/            }
        }
        /*
        bool hasIntersectionWith(sprite* other)
        {
            return dang_collision::hasIntersection(this->getHotrect(), other->getHotrect());
        }
        */

        rect* getHotrect()
        {
            rect r = {this->x, this->y, this->width, this->height};

            return &r;
        }
    };

    enum layerType{ltTile, ltObjects};

    struct layer
    {
        std::string name;
        float opacity;
        bool visible;
        uint8_t type; // 0 = tile, 1 = objects
        // TODO: to be considered
        //offset : point 	Offset in pixels that is applied when this layer is rendered.
        //map : TileMap 	Map that this layer is part of (or null in case of a standalone layer).
    };

    struct tilelayer : layer {
        uint8_t width;
        uint8_t height;
        std::vector<tile> tiles;

        tilelayer(std::string n, uint32_t size, tile _tiles[], uint8_t w, uint8_t h) : layer() {
            name = n;
            width = w;
            height = h;
            type = 0;
            tiles.assign(_tiles, _tiles + size);
        };
    };

    struct objectlayer : layer {
        std::vector<spriteobject> so;

        objectlayer(std::string n, uint16_t size, spriteobject _so[]) : layer() {
            name = n;
            type = 1;
            so.assign(_so, _so + size);
        };

        objectlayer(spriteobject _so) : layer() {
            type = 1;
            so.push_back(_so);
        };
    };

    struct viewport {

        dang::point pos;
        dang::point screen;
        dang::point world;

        void updatePosition(int32_t x, int32_t y)
        {
            int32_t deltaY = y - pos.y;
            int32_t deltaX = x - pos.x;

            if(pos.y > 0 && deltaY <= 50)
            {
                // todo: speed should be velocity of anchored object or we
                //  scroll on as long as the anchored object is still in
                //  scroll region and is forced to stay within viewport...
                pos.y -= 2;

            } else if(pos.y < world.y - screen.y && deltaY >= 130)
            {
                pos.y += 2;
            }

            if(pos.x > 0 && deltaX <= 50)
            {
                pos.x -= 2;

            } else if(pos.x < world.x - screen.x && deltaX >= 200)
            {
                pos.x += 2;
            }
        }

        void updatePosition(point* anchor){
            updatePosition(anchor->x, anchor->y);
        }

        void move(int32_t diffX, int32_t diffY){
            pos.x += diffX;
            pos.y += diffY;
        }

        void setPosition(int32_t x, int32_t y){
            pos.x = x;
            pos.y = y;
        }

        void setPosition(point* p){
            pos.x = p->x;
            pos.y = p->y;
        }

        void init(int32_t screenWidth, int32_t screenHeight, int32_t worldWidth, int32_t worldHeight)
        {
            world.x = worldWidth;
            world.y = worldHeight;

            screen.x = screenWidth;
            screen.y = screenHeight;
        }
    };

    struct world {
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

    struct level
    {
        // array of layers with all layers in them so that we can iterate over them
        //  to have them drawn to the screen.
        std::vector<std::shared_ptr<layer>> layers;
        std::unordered_map<uint8_t, tileset> tilesets;
//        std::unordered_map<std::string, blit::spritesheet*> spritesheets;
        std::unordered_map<std::string, Imagesheet*> imagesheets;
//        std::unordered_map<std::string, std::shared_ptr<Imagesheet>> imagesheets;

        // TODO: sprites should know on which layer they belong...
        std::forward_list<std::shared_ptr<sprite>> sprites;

        std::shared_ptr<sprite> hero;

        u_int16_t camX = 0;
        u_int16_t camY = 0;

        bool debug = false;

        viewport vp;
        world w;

        std::shared_ptr<sprite> addSprite(sprite* s)
        {
            std::shared_ptr<sprite> s_p = std::make_shared<sprite>(*s);
            sprites.push_front(s_p);

            return s_p;
        }
    };
} // end of namespace DANG