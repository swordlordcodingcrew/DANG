// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
//
// Created by LordFilu on 16.2.20.
//

#include "dang_globals.hpp"
#include "Gear.h"
#include "Layer.h"
#include "TileLayer.h"
#include "Sprite.h"
#include "Imagesheet.h"
#include "SpriteLayer.h"
//#include "dang_collision.hpp"

namespace dang
{

    Gear::Gear()
    {
    }

    Gear::~Gear()
    {
    }

    void Gear::initLevel(level &lvl, blit::Rect& viewport)
    {
//        _lvl = lvl;

        _viewport = viewport;

        _world.w = lvl.w.getWidthInPixel();
        _world.h = lvl.w.getHeightInPixel();

        // active world defaults to double width and height of world
        _active_world.w = 2 * _world.w;
        _active_world.h = 2 * _world.h;

        lvl.imagesheets["background"]->_name = "background";
        lvl.imagesheets["players"]->_name = "players";

        // first add the imagesheets
        std::shared_ptr<Imagesheet> bg_is = std::shared_ptr<Imagesheet>(lvl.imagesheets["background"]);
        addImagesheet("background", bg_is);
        std::shared_ptr<Imagesheet> pl_is = std::shared_ptr<Imagesheet>(lvl.imagesheets["players"]);
        addImagesheet("players", pl_is);


        // TODO: ugly hack, test and clean up
/*        for (std::shared_ptr<layer>& l : lvl.layers)
        {
            if (l->type == ltObjects)
            {
                std::shared_ptr<dang::objectlayer>& ol = std::static_pointer_cast<dang::objectlayer>(&l);
                for (spriteobject& so : ol->so)
                {
                    if (_viewport.contains(blit::Point(so.x, so.y)))
                    {
                        std::shared_ptr<Sprite> spr = std::make_shared(Sprite(so));
                        _sprites.push_front(spr);
                    }
                }
            }

        }
*/

    }

    void Gear::render(uint32_t time)
    {
        for (std::shared_ptr<Layer> l : _layers)
        {
            if (l->_visible)
            {
                l->render(*this);
            }
        }
    }

    void Gear::update(uint32_t time)
    {
        for (std::shared_ptr<Layer> l : _layers)
        {
            l->update(time, *this);
        }
    }

    void Gear::processCollisionDetection()
    {
        // collision tests
/*        for(auto i = _lvl.sprites.begin(); i != _lvl.sprites.end(); i++) {

            std::shared_ptr<sprite> s = *i;

            if(s->velX == 0 && s->velY == 0){
                continue;
            }

            point goal = {s->x + s->velX, s->y + s->velY};

            std::vector<dang_collision::collision> collisions;

            // void move(sprite* me, point* goal, point* moveto, std::vector<collision>* collisions)
            dang_collision::move(&_lvl, s, &goal, &collisions);

            //std::cout << "number of collisions: " << collisions.size() << std::endl;

            for(auto j = collisions.begin(); j != collisions.end(); j++)
            {
                dang_collision::collision clsn = *j;
            }
        }
*/
    }

    void Gear::addImagesheet(const std::string& key, std::shared_ptr<Imagesheet> is)
    {
        _imagesheets[key] = is;
    }

    void Gear::removeImagesheet(const std::string& key)
    {

        _imagesheets.erase(key);
    }

    void Gear::renderImage(const std::shared_ptr<Imagesheet> is, const uint16_t im_id, const blit::Point &dest, const uint8_t transform) const
    {
        if (is == nullptr) return;

        if (blit::screen.sprites != is.get())
        {
            blit::screen.sprites = is.get();
        }
//        - s->height
//        blit::screen.blit_sprite(is->getRect(im_id), blit::Point(dest.x - _viewport.x, dest.y - _viewport.y), transform);
        blit::screen.blit_sprite(is->getRect(im_id), dest, transform);
    }

    void Gear::addLayer(std::shared_ptr<Layer> layer)
    {
        _layers.push_front(layer);
        _layers.sort([] (const std::shared_ptr<Layer> &first, const std::shared_ptr<Layer> &second)
        {
            return first->_z_order < second->_z_order;
        });

    }

    void Gear::removeLayer(std::shared_ptr<Layer> layer)
    {
        _layers.remove(layer);
    }

    blit::Rect Gear::getActiveWorld() const
    {
        return blit::Rect(_viewport.x - (_active_world.w - _viewport.w)/2, _viewport.y + ((_active_world.h - _viewport.h)/2), _active_world.w, _active_world.h);
    }

    std::shared_ptr<Imagesheet> Gear::getImagesheetByName(std::string &name)
    {
        return _imagesheets[name];
    }

    void Gear::setViewportPos(blit::Point& pos)
    {
        _viewport.x = pos.x;
        _viewport.y = pos.y;
    }


}