// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "dang.hpp"
#include "DangFwdDecl.h"
#include "src/layer/Layer.hpp"
#include "bt/BTNode.h"
#include "RectT.hpp"

#include <32blit.hpp>

#include <unordered_map>
#include <forward_list>
#include <memory>
#include <functional>

namespace dang
{
    class Gear
    {
    public:
        Gear();
        virtual ~Gear();

        void                initLevel(const tmx_level* lvl, RectF& viewport);

        void                update(uint32_t dt);
        void                render(uint32_t time);
//        BTNode::Status      runNTree(const spCollisionSprite& s) const;

        void                addImagesheet(spImagesheet is);
        spImagesheet        getImagesheet(const std::string& name) const;
        void                removeImagesheet(const std::string& name);
        void                removeImagesheets();
        const std::unordered_map<std::string, spImagesheet>& getImagesheets() { return _imagesheets; }

        void                addNTree(const std::string& name, spNTree tree);
        spNTree             getNTree(const std::string& name) const;
        void                removeNTree(const std::string& name);
        void                removeNTrees();

        void                addLayer(spLayer layer);
        spLayer             getLayerByName(const std::string& name);
        spLayer             getLayerByTypename(Layer::E_TYPE type);
        void                removeLayer(const spLayer& layer);
        void                removeLayers();
        void                setLayerVisibility(const std::string& name, bool visible);
        void                setLayersVisibility(bool visible);
        void                setLayerActive(const std::string& name, bool active);
        void                setLayersActive(bool active);


        RectF               getActiveWorld() const;
        Vector2F            getActiveWorldSize() const { return _active_world_size; };
        void                setActiveWorldSize(Vector2F& aws) {_active_world_size = aws; };
        void                setActiveWorldSize(float w, float h) {_active_world_size.w = w; _active_world_size.h = h; };

        RectF               getViewport() const { return _viewport; }
        void                setViewport(const RectF& vp);
        void                setViewportPos(const Vector2F& pos);
        void                follow(const Vector2F& dest);

        RectF  getWorld() const { return _world;}
        void   setWorld(const RectF& world) {_world = world; };

        void fade(const blit::Pen &col, uint8_t fade_step, const std::function<void(void)>& cb);
        bool fading() const { return _fade_overlay; }

    protected:

        std::unordered_map<std::string, spImagesheet> _imagesheets;
        std::unordered_map<std::string, spNTree> _nTree;
        std::forward_list<spLayer> _layers;

        // viewport handling
        RectF       _world{0,0,0,0};    // size of the world
        RectF       _viewport{0,0,0,0};     // part of world to be drawn
        Vector2F    _active_world_size{0,0}; // center equals to center of viewport. sprite within active world will be updated and - if in viewport - drawnd

        // fading params & funcs
        bool        _fade_overlay{false};
        blit::Pen   _fade_colour{0,0, 0, 0};
        uint8_t     _fade_step{8};
        std::function<void (void)> _faded_cb{nullptr};
        void        fadeForward();
        enum FADE_STATE : uint8_t
        {
            NONE,
            FADE_TO_COL,
            FADE_TO_TRANS,
            END_OF_FADE
        };
        FADE_STATE _fade_state{NONE};

    };
}

