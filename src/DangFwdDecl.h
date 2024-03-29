// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <memory>

namespace dang
{
    /** bt classe - Behaviour Tree */
    class BTNode;
    class NTBuilder;
    class NTree;
    using spNTree = std::shared_ptr<NTree>;
    class NTreeState;
    using spNTreeState = std::shared_ptr<NTreeState>;

    /** bml classe - Bullet ML */
    class BMLNode;
    class BMLBuilder;
    class BMLTree;
    using spBMLTree = std::shared_ptr<BMLTree>;
    class BMLState;
    using spBMLState = std::shared_ptr<BMLState>;

    /** path classes */
    class SceneGraph;
    using spSceneGraph = std::shared_ptr<SceneGraph>;
    class Waypoint;
    using wpWaypoint = std::weak_ptr<Waypoint>;
    using spWaypoint = std::shared_ptr<Waypoint>;
    class Wavepoint;
    using spWavepoint = std::shared_ptr<Wavepoint>;


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
    class TwPosSpline;
    using spTwPosSpline = std::shared_ptr<TwPosSpline>;
    class TwPos;
    using spTwPos = std::shared_ptr<TwPos>;
    class TwPosX;
    using spTwPosX = std::shared_ptr<TwPosX>;
    class TwPosY;
    using spTwPosY = std::shared_ptr<TwPosY>;

    /** collision classes */
    class CollisionObject;
    using spCollisionObject = std::shared_ptr<CollisionObject>;

    /** sprite classes */
    class SpriteObject;
    using spSpriteObject = std::shared_ptr<SpriteObject>;
    class MotionObject;
    using spMotionObject = std::shared_ptr<MotionObject>;
    class ImageObject;
    using spImageObject = std::shared_ptr<ImageObject>;
    class ImgSpr;
    using spImgSpr = std::shared_ptr<ImgSpr>;
    class FullImgSpr;
    using spFullImgSpr = std::shared_ptr<FullImgSpr>;
    class ColSpr;
    using spColSpr = std::shared_ptr<ColSpr>;
    class FullColSpr;
    using spFullSpr = std::shared_ptr<FullColSpr>;


    /** remaining classes */
    class Layer;
    using spLayer = std::shared_ptr<Layer>;
    class ImgSprLayer;
    using spImgSprLayer = std::shared_ptr<ImgSprLayer>;
    class ColSprLayer;
    using spColSprLayer = std::shared_ptr<ColSprLayer>;
//    class SpriteLayer;
//    using spSpriteLayer = std::shared_ptr<SpriteLayer>;
    class TileLayer;
    using spTileLayer = std::shared_ptr<TileLayer>;
    class BaseHUDLayer;
    using spBaseHUDLayer = std::shared_ptr<BaseHUDLayer>;
    class MessageLayer;
    using spMessageLayer = std::shared_ptr<MessageLayer>;

    //    class CollisionSprite;
//    using spCollisionSprite = std::shared_ptr<CollisionSprite>;
//    class CollisionSpriteLayer;
//    using spCollisionSpriteLayer = std::shared_ptr<CollisionSpriteLayer>;
    class Gear;
    class Imagesheet;
    using spImagesheet = std::shared_ptr<Imagesheet>;
    struct image_import;
//    class Sprite;
//    using spSprite = std::shared_ptr<Sprite>;
    class Event;
    class Dispatcher;

    /** tmx extruder classes */
    struct tmx_spriteobject;
    struct tmx_level;
    struct tmx_layer;




}