// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework
// (c) 2019-20 by SwordLord - the coding crew

#include "dang_globals.hpp"
#include "Gear.h"
#include "Layer.h"
#include "Sprite.h"
#include "Imagesheet.h"
#include "tmx_def.h"
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

    void Gear::initLevel(tmx_level &lvl, RectF& viewport)
    {
        _viewport = viewport;

        _world.w = lvl.w.getWidthInPixel();
        _world.h = lvl.w.getHeightInPixel();

        // active world defaults to double width and height of world
        _active_world_size.x = 2 * _world.w;
        _active_world_size.y = 2 * _world.h;

        lvl.imagesheets["background"]->_name = "background";
        lvl.imagesheets["players"]->_name = "players";

        // first add the imagesheets
        std::shared_ptr<Imagesheet> bg_is = std::shared_ptr<Imagesheet>(lvl.imagesheets["background"]);
        addImagesheet("background", bg_is);
        std::shared_ptr<Imagesheet> pl_is = std::shared_ptr<Imagesheet>(lvl.imagesheets["players"]);
        addImagesheet("players", pl_is);

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

/*    void Gear::renderImage(const std::shared_ptr<Imagesheet> is, const uint16_t im_id, const blit::Point &dest, const uint8_t transform) const
    {
        if (is == nullptr) return;

        if (blit::screen.sprites != is.get())
        {
            blit::screen.sprites = is.get();
        }

        blit::screen.blit_sprite(is->getRect(im_id), dest, transform);
    }
*/
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

    std::shared_ptr<Layer> Gear::getLayerByName(std::string &name)
    {
        std::forward_list<std::shared_ptr<Layer>>::iterator layer_it = std::find_if(_layers.begin(), _layers.end(), [=](const std::shared_ptr<Layer>& val)
        {
            return (val->_name == name);
        });
        return (*layer_it);
    }


    RectF Gear::getActiveWorld() const
    {
        return RectF(_viewport.x - (_active_world_size.x - _viewport.w)/2, _viewport.y + ((_active_world_size.y - _viewport.h)/2), _active_world_size.x, _active_world_size.y);
    }

    std::shared_ptr<Imagesheet> Gear::getImagesheetByName(std::string &name)
    {
        return _imagesheets[name];
    }

    void Gear::setViewportPos(Vector2F& pos)
    {
        _viewport.x = pos.x;
        _viewport.y = pos.y;
    }



}