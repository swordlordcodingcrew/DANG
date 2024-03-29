// (c) 2019-2021 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "DangFwdDecl.h"
#include "RectT.hpp"
#include "tween/Ease.hpp"

#include <unordered_map>
#include <forward_list>
#include <vector>
#include <memory>
#include <bitset>

namespace dang
{

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
        const int16_t width;
        const int16_t height;
        const bool visible;
        const std::string tileset;
        const uint16_t tile;
        const std::string bt; // behaviour tree name
        const uint8_t z_order;

        // transformation flags
        // reserved         x000
        // antidiag         0x00 (probably not working)
        // vertically       00x0
        // horizontally     000x
        const std::bitset<4> transform; // flip vertical, horizontal

        /*
        // TODO: to be considered
         rotation : number 	Rotation of the object in degrees clockwise.
        polygon : Polygon 	Polygon of the object.
        text : string 	The text of a text object.
        font : Font 	The font of a text object.
        textAlignment : Alignment 	The alignment of a text object.
        wordWrap : bool 	Whether the text of a text object wraps based on the width of the object.
        textColor : color 	Color of a text object.
        */
    };


    enum tmx_layerType{ltTile, ltObjects};

    struct tmx_layer
    {
        const std::string name;
        const uint8_t type; // 0 = tmx_tile, 1 = objects
        const float opacity;
        const bool visible;
        const uint8_t z_order;   // the z-order of the layers
        const Vector2F position;

        // tile layer vars
        const uint16_t tl_width;
        const uint16_t tl_height;
        const std::string tl_tileset;
        const tmx_tile* tl_tiles;
        const size_t tl_tiles_len;

        // object layer vars
        const tmx_spriteobject* spriteobjects;
        const size_t spriteobejcts_len;

        // TODO: to be considered
        //map : TileMap 	Map that this tmx_layer is part of (or null in case of a standalone tmx_layer).
    };


    struct tmx_world
    {
        const uint16_t width = 0; // width in tiles
        const uint16_t height = 0; // height in tiles

        const uint16_t tileWidth = 0; // width of tiles
        const uint16_t tileHeight = 0; // height of tiles
    };

    enum e_tmx_waypoint_connection
    {
        wpc_invalid = 0x00,
        wpc_walk = 0x01,
        wpc_jump = 0x02,
        wpc_warp = 0x04,
        wpc_block = 0x08
    };

    struct tmx_waypoint
    {
        const uint32_t id;
        const float x;
        const float y;
        const uint8_t type;
    };

    struct tmx_waypoint_connection
    {
        const uint32_t waypoint_start_id;
        const uint32_t waypoint_goal_id;
        const uint8_t connection_type;     // see enum tmx_waypoint_connection
    };

    struct tmx_wavepoint
    {
        const uint32_t id;
        const std::string name;
        const std::string type;
        const float x;
        const float y;
        const float duration;
        const uint8_t orientation;
        const float delay;
        const uint32_t next_wavepoint_id;
    };

    struct tmx_zone
    {
        const uint16_t zone_nr;
        // global coords in pixel, aligend to tile-width/-height
        const float x;
        const float y;
        const float w;
        const float h;
    };

    struct tmx_zone_passage
    {
        const uint16_t zone_nr;
        const int16_t from_zone;
        // global coordd in pixel, aligend to tile-width/-height
        const float x;
        const float y;
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

        const tmx_waypoint* waypoints;
        const size_t waypoints_len;

        const tmx_waypoint_connection* waypoint_connections;
        const size_t waypoint_connections_len;

        const tmx_wavepoint* wavepoints;
        const size_t wavepoints_len;

        const tmx_zone* zones;
        const size_t zones_len;

        const tmx_zone_passage* zone_passages;
        const size_t zone_passages_len;

    };


    /**
     * tmx extruder class gets or extrudes objects from the tmx-derived include-file with the DANG Extruder pattern
     */
    class TmxExtruder
    {

    public:
        TmxExtruder() = default;
        explicit TmxExtruder(const tmx_level* lvl, Gear* gear);
        ~TmxExtruder() = default;

        spImagesheet            getImagesheet(const std::string& name);
        void                    getImagesheets();

        spImgSprLayer           getImgSprLayer(const std::string& name, bool addSprites, bool addToGear, bool autoFillAnimations);
        spColSprLayer           getColSprLayer(const std::string& name, bool addToGear);
        void                    fillHUDLayer(spImgSprLayer  layer, const std::string& name, bool addSprites, bool addToGear);
        spTileLayer             getTileLayer(const std::string& name, bool addToGear);
//        spTwAnim                getAnimation(const std::string& is_name, const std::string& anim_name, EaseFn ease_cb = Ease::Linear, int32_t loops = -1, bool alternating = false, uint32_t delay = 0);
        spTwAnim                getAnimation(const spImagesheet &is, const std::string& anim_name, EaseFn ease_cb = Ease::Linear, int32_t loops = -1, bool alternating = false, uint32_t delay = 0);
        void                    createSceneGraphs(RectF& zone, std::vector<dang::spSceneGraph>& scene_graphs);
        void                    createWaves(RectF& zone, std::unordered_map<uint32_t, dang::Wavepoint>& waves);
        RectF                   getZone(uint16_t zone_nr);
        uint16_t                getZoneNr(const Vector2F& pos);
        Vector2F                getPassage(uint16_t zone_nr, int16_t from_zone);

        const tmx_layer* getTmxLayer(const std::string &name);
        const tmx_tileset*  getTileset(const std::string &name);
        const tmx_tileanimation* getTileAnimation(const std::string &tileset, const std::string& name);

        dang::Gear*  _gear{nullptr};

    protected:
        const tmx_level*  _level{nullptr};
    };

}



