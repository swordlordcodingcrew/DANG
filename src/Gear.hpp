// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "dang.hpp"
#include "bt/NTree.h"

#include <unordered_map>
#include <forward_list>
#include "RectT.hpp"
#include <memory>

namespace dang
{
    struct tmx_level;
    struct tmx_layer;

    class Sprite;
    class CollisionSprite;
    class Layer;
    class Imagesheet;

    using spImagesheet = std::shared_ptr<Imagesheet>;
    using spNTree = std::shared_ptr<NTree>;
    using spNTreeState = std::shared_ptr<NTreeState>;
    using spSprite = std::shared_ptr<Sprite>;
    using spCollisionSprite = std::shared_ptr<CollisionSprite>;
    using spLayer = std::shared_ptr<Layer>;

    class Gear
    {
    public:
        Gear();
        virtual ~Gear();

        void    initLevel(const tmx_level* lvl, RectF& viewport);

        void    update(uint32_t dt);
        void    render(uint32_t time);
        BTNode::Status        runNTree(const spCollisionSprite& s) const;

        void                        addImagesheet(spImagesheet is);
        spImagesheet                getImagesheet(const std::string& name) const;
        void                        addNTree(const std::string& name, spNTree tree);
        spNTree                     getNTree(const std::string& name) const;

        void                        removeImagesheet(const std::string& name);
        void                        removeImagesheets();

        void                        removeNTree(const std::string& name);
        void                        removeNTrees();

        void                        addLayer(spLayer layer);
        std::shared_ptr<Layer>      getLayerByName(const std::string& name);
        void                        removeLayer(const spLayer& layer);
        void                        removeLayers();

        RectF       getActiveWorld() const;
        Vector2F    getActiveWorldSize() const { return _active_world_size; };
        void        setActiveWorldSize(Vector2F& aws) {_active_world_size = aws; };
        void        setActiveWorldSize(float w, float h) {_active_world_size.w = w; _active_world_size.h = h; };

        RectF  getViewport() const { return _viewport; }
        void   setViewport(const RectF& vp);
        void   setViewportPos(const Vector2F& pos);

        void   follow(const Vector2F& dest);

        RectF  getWorld() const { return _world;}
        void   setWorld(const RectF& world) {_world = world; };

        const std::unordered_map<std::string, spImagesheet>& getImagesheets() { return _imagesheets; };

    protected:

        std::unordered_map<std::string, spImagesheet> _imagesheets;
        std::unordered_map<std::string, spNTree> _nTree;
        std::forward_list<spLayer> _layers;

        // viewport handling
        RectF       _world{0,0,0,0};    // size of the world
        RectF       _viewport{0,0,0,0};     // part of world to be drawn
        Vector2F    _active_world_size{0,0}; // center equals to center of viewport. sprite within active world will be updated and - if in viewport - drawnd

    };
}

