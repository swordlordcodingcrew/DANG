// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <memory>

namespace dang
{
    /** bt classe */
    class BTNode;
    class NTBuilder;
    class NTree;
    using spNTree = std::shared_ptr<NTree>;
    class NTreeState;
    using spNTreeState = std::shared_ptr<NTreeState>;

    /** path classes */
    class SceneGraph;
    using spSceneGraph = std::shared_ptr<SceneGraph>;
    class Waypoint;
    using wpWaypoint = std::weak_ptr<Waypoint>;
    using spWaypoint = std::shared_ptr<Waypoint>;

    /** snd classes */
    // todo

    /** tween classes */
    class Tweenable;
    using spTweenable = std::shared_ptr<Tweenable>;
    class TwAnim;
    using spTwAnim = std::shared_ptr<TwAnim>;
    class TwNull;
    using spTwNull = std::shared_ptr<TwNull>;
    class TwSequence;
    using spTwSequence = std::shared_ptr<TwSequence>;
    class TwVel;
    using spTwVel = std::shared_ptr<TwVel>;
    class TwVelY;
    using spTwVelY = std::shared_ptr<TwVelY>;


    /** remaining classes */
    class Layer;
    using spLayer = std::shared_ptr<Layer>;
    class SpriteLayer;
    using spSpriteLayer = std::shared_ptr<SpriteLayer>;
    class TileLayer;
    using spTileLayer = std::shared_ptr<TileLayer>;
    class BaseHUDLayer;
    using spBaseHUDLayer = std::shared_ptr<BaseHUDLayer>;
    class CollisionSprite;
    using spCollisionSprite = std::shared_ptr<CollisionSprite>;
    class CollisionSpriteLayer;
    using spCollisionSpriteLayer = std::shared_ptr<CollisionSpriteLayer>;
    class Gear;
    class Imagesheet;
    using spImagesheet = std::shared_ptr<Imagesheet>;
    struct image_import;
    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;
    class Layer;
    using spLayer = std::shared_ptr<Layer>;
    class Event;
    class Dispatcher;

    /** tmx extruder classes */
    struct tmx_spriteobject;
    struct tmx_level;
    struct tmx_layer;




}