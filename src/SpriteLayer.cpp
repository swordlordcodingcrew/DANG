// (c) 2019-20 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include <32blit.hpp>
#include <cassert>
#include "Gear.hpp"
#include "SpriteLayer.hpp"
#include "Sprite.hpp"
#include "Layer.hpp"
#include "Imagesheet.hpp"
#include "SpriteIterator.hpp"

namespace dang
{
    dang::SpriteLayer::SpriteLayer() : Layer(Layer::LT_SPRITELAYER)
    {
        _root = std::make_shared<Sprite>();
        _root->_visible = false;
        _root->setPos({0,0});
    }

    void SpriteLayer::update(uint32_t dt, const Gear &gear)
    {
        // core update and update active sprites
        coreUpdate(dt, gear);

        // then call update
        for (spSprite& spr : _active_sprites)
        {
            if (gear.getActiveWorld().intersects(spr->getSizeRect()))
            {
                spr->update(dt);
            }
        }
    }

    void SpriteLayer::render(const Gear &gear)
    {
        RectF vp = gear.getViewport();
        int32_t vpx = std::floor(vp.tl().x);
        int32_t vpy = std::floor(vp.tl().y);

        for (std::shared_ptr<Sprite>& spr : _active_sprites)
        {
            if (spr->_visible && spr->_imagesheet != nullptr && vp.intersects(spr->getSizeRect()))
            {
                spr->render(vpx, vpy);
            }
        }
    }


    void SpriteLayer::addSprite(std::shared_ptr<Sprite> spr)
    {
        if (spr != nullptr)
        {
            _inactive_sprites.push_front(spr);
        }
    }

    void SpriteLayer::removeSprite(std::shared_ptr<Sprite> spr)
    {
        if (spr != nullptr)
        {
            _active_sprites.remove(spr);
            _inactive_sprites.remove(spr);
        }
    }

    void SpriteLayer::coreUpdate(uint32_t dt, const Gear &gear)
    {
        std::list<spSprite> splice_list;

        // find inactive sprites becoming active, call coreUpdate and put them into the splice_list
        auto spr = _inactive_sprites.begin();
        while (spr != _inactive_sprites.end())
        {
            if (gear.getActiveWorld().intersects((*spr)->getSizeRect()))
            {
                splice_list.push_front(*spr);
                (*spr)->coreUpdate(dt);
                spr = _inactive_sprites.erase(spr);
            }
            else
            {
                spr++;
            }
        }

        // update active sprite or put them into the inactive sprite list
        spr = _active_sprites.begin();
        while (spr != _active_sprites.end())
        {
            if (gear.getActiveWorld().intersects((*spr)->getSizeRect()))
            {
                (*spr)->coreUpdate(dt);
                spr++;
            }
            else
            {
                _inactive_sprites.push_front(*spr);
                spr = _active_sprites.erase(spr);
            }
        }

        if (!splice_list.empty())
        {
            D_DEBUG_PRINT("merge active=%u", splice_list.size());

            _active_sprites.merge(splice_list);

            // TODO: might be optimized: use of a manual bubble sort with a swap each cycle
            _active_sprites.sort([] (const std::shared_ptr<Sprite> &first, const std::shared_ptr<Sprite> &second)
                 {
                     return first->_z_order < second->_z_order;
                 });

/*            _active_sprites.merge(splice_list, [] (const std::shared_ptr<Sprite> &first, const std::shared_ptr<Sprite> &second)
            {
                return first->_z_order < second->_z_order;
            });
*/        }

    }

    spSprite SpriteLayer::getSpriteByType(const std::string& name)
    {
        auto ret = std::find_if(_active_sprites.begin(), _active_sprites.end(), [&] (const std::shared_ptr<Sprite> &first)
        {
            return first->_type_name == name;
        });

        if (ret != _active_sprites.end())
        {
            return (*ret);
        }

        ret = std::find_if(_inactive_sprites.begin(), _inactive_sprites.end(), [&] (const std::shared_ptr<Sprite> &first)
        {
            return first->_type_name == name;
        });

        if (ret != _inactive_sprites.end())
        {
            return (*ret);
        }

        return nullptr;
    }


    spSprite SpriteLayer::getSpriteById(uint16_t id)
    {

        auto ret = std::find_if(_active_sprites.begin(), _active_sprites.end(), [&] (const std::shared_ptr<Sprite> &first)
            {
                return first->_id == id;
            });

        if (ret != _active_sprites.end())
        {
            return (*ret);
        }

        ret = std::find_if(_inactive_sprites.begin(), _inactive_sprites.end(), [&] (const std::shared_ptr<Sprite> &first)
            {
                return first->_id == id;
            });

        if (ret != _inactive_sprites.end())
        {
            return (*ret);
        }

        return nullptr;
    }

    void SpriteLayer::removeSpriteById(uint16_t id)
    {
        _active_sprites.remove_if([&] (const std::shared_ptr<Sprite> &spr)
            {
              return spr->_id == id;
            });

        _inactive_sprites.remove_if([&] (const std::shared_ptr<Sprite> &spr)
            {
                return spr->_id == id;
            });
    }

    void SpriteLayer::removeSpritesByTypeNum(uint8_t type_num)
    {
        for (auto it = _active_sprites.begin(); it != _active_sprites.end(); )
        {
            if ((*it)->_type_num == type_num)
            {
                it = _active_sprites.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = _inactive_sprites.begin(); it != _inactive_sprites.end(); )
        {
            if ((*it)->_type_num == type_num)
            {
                it = _inactive_sprites.erase(it);
            }
            else
            {
                ++it;
            }
        }

    }

    void SpriteLayer::sortSprites()
    {
/*        _inactive_sprites.sort([](const spSprite & first, const spSprite & second)
            {
                return first->_z_order < second->_z_order;
            });
*/
        _active_sprites.sort([](const spSprite & first, const spSprite & second)
            {
                return first->_z_order < second->_z_order;
            });

    }

    void SpriteLayer::addSpriteT(spSprite s)
    {
        assert(s != nullptr);
        _inactive_sprites.push_front(s);
    }

    void SpriteLayer::removeSpriteT(spSprite s)
    {
        assert(s != nullptr);
        s->removeMeFromTree();
        _inactive_sprites.remove(s);
    }

    void SpriteLayer::updateT(uint32_t dt, const Gear &gear)
    {
        // core update and update active sprites
        coreUpdateT(dt, gear);

        // then call update
        for (SpriteIterator it = begin(); it != end(); it++)
        {
            if (gear.getActiveWorld().intersects((*it)->getSizeRect()))
            {
                (*it)->update(dt);
            }
        }
    }

    void SpriteLayer::coreUpdateT(uint32_t dt, const Gear &gear)
    {
        // add inactive sprites becoming active
        auto sli = _inactive_sprites.begin();
        while (sli != _inactive_sprites.end())
        {
            if ((*sli)->_remove_me)
            {
                sli = _inactive_sprites.erase(sli);
            }
            else
            {
                if (gear.getActiveWorld().intersects((*sli)->getSizeRect()))
                {
                    _root->addSprite(*sli);
                    sli = _inactive_sprites.erase(sli);
                }
                else
                {
                    sli++;
                }
            }
        }

        // update active sprites (tree) or put them into the inactive sprite list
        auto sti = begin();
        while (sti != end())
        {
            if ((*sti)->_remove_me)
            {
                sti = erase(sti);
            }
            else
            {
                if (gear.getActiveWorld().intersects((*sti)->getSizeRect()))
                {
                    (*sti)->coreUpdate(dt);
                    sti++;
                }
                else
                {
                    _inactive_sprites.push_front(*sti);
                    sti = erase(sti);
                }
            }
        }
    }

    void SpriteLayer::renderT(const Gear &gear)
    {
        RectF vp = gear.getViewport();
        int32_t vpx = std::floor(vp.tl().x);
        int32_t vpy = std::floor(vp.tl().y);

        for (std::shared_ptr<Sprite>& spr : _active_sprites)
        {
            if (spr->_visible && spr->_imagesheet != nullptr && vp.intersects(spr->getSizeRect()))
            {
                spr->render(vpx, vpy);
            }
        }

    }

    SpriteIterator SpriteLayer::begin()
    {
        return SpriteIterator(_root, _root);
    }

    SpriteIterator SpriteLayer::end()
    {
        return SpriteIterator(nullptr, _root);
    }

    SpriteIterator SpriteLayer::erase(SpriteIterator pos)
    {
        spSprite s = (*pos);
        pos++;

        s->removeMeFromTree();

        return pos;
    }
}
