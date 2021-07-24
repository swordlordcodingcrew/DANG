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

    /** remaining classes */
    class CollisionSprite;
    using spCollisionSprite = std::shared_ptr<CollisionSprite>;
    class Imagesheet;
    using spImagesheet = std::shared_ptr<Imagesheet>;
    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;

    /** tmx extruder classes */
    struct tmx_spriteobject;




}