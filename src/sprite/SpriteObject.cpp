// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "SpriteObject.hpp"
#include "src/TmxExtruder.hpp"
#include "src/dang.hpp"

#include <cassert>

namespace dang
{
    SpriteObject::SpriteObject(const tmx_spriteobject *so)
    {
        _id = so->id;
        _pos.x = so->x;
        _pos.y = so->y;
        _z_order = so->z_order;
        _size.w = so->width;
        _size.h = so->height;
    }


    /** adds a sprite as child to this sprite
     *
     * @param s spriteobject to be added as child
     */
    void SpriteObject::addSpriteObject(spSpriteObject s)
    {
        s->_parent = this;

        if (_child == nullptr)  // first child
        {
            D_DEBUG_PRINT("add sprite with id=%u as first child\n", s->id());
            _child = s;
            _child->_next_sibling = nullptr;
            _child->_prev_sibling = nullptr;
        }
        else
        {
            // go through siblings until z_order is correct
            spSpriteObject sp = _child;
            if (sp->_z_order >= s->_z_order)
            {
                // first position
                D_DEBUG_PRINT("add sprite with id=%u at first position\n", s->id());
                _child = s;
                _child->_next_sibling = sp;
                _child->_prev_sibling = nullptr;

                sp->_prev_sibling = _child.get();
            }
            else
            {
                while (sp->_next_sibling != nullptr)
                {
                    sp = sp->_next_sibling;
                    if (sp->_z_order >= s->_z_order)
                    {
                        break;
                    }
                }

                if (sp->_next_sibling == nullptr && sp->_z_order < s->_z_order)
                {
                    // last sibling
                    D_DEBUG_PRINT("add sprite with id=%u at last position\n", s->id());
                    sp->_next_sibling = s;
                    s->_prev_sibling = sp.get();
                    s->_next_sibling = nullptr;
                }
                else
                {
                    // somewhere in between
                    D_DEBUG_PRINT("add sprite with id=%u in between\n", s->id());
                    assert(sp->_prev_sibling != nullptr);
                    sp->_prev_sibling->_next_sibling = s;

                    s->_next_sibling = sp;
                    s->_prev_sibling = sp->_prev_sibling;

                    sp->_prev_sibling = s.get();
                }
            }
        }
    }

    /**
     * removes this sprite from the tree (and its children)
     */
    void SpriteObject::removeMeFromTree()
    {
        D_DEBUG_PRINT("removeMeFromTree\n");
        if (_parent != nullptr)
        {
            D_DEBUG_PRINT("removeMeFromTree: spr locked\n");
            if (_prev_sibling == nullptr)
            {
                // first child
                if (_next_sibling == nullptr)
                {
                    // only child
                    _parent->_child = nullptr;
                    D_DEBUG_PRINT("removeMeFromTree: remove only child\n");
                }
                else
                {
                    _parent->_child = _next_sibling;
                    _parent->_child->_prev_sibling = nullptr;
                    D_DEBUG_PRINT("removeMeFromTree: remove first child\n");
                }
            }
            else
            {
                if (_next_sibling != nullptr)
                {
                    _prev_sibling->_next_sibling = _next_sibling;
                    _next_sibling->_prev_sibling = _prev_sibling;
                    D_DEBUG_PRINT("removeMeFromTree: remove a middle sibling\n");
                }
                else
                {
                    _prev_sibling->_next_sibling.reset();
                    D_DEBUG_PRINT("removeMeFromTree: remove last sibling\n");
                }
            }

            _parent = nullptr;
            _next_sibling.reset();
            _prev_sibling = nullptr;
        }
        else
        {
            // sprite is not in a tree;
            // (and the root sprite cannot be removed)
        }
    }

    Vector2F SpriteObject::local2Global(const Vector2F &in)
    {
        Vector2F ret = in;
        SpriteObject* par = _parent;
        while (par != nullptr)
        {
            ret += par->_pos;
            par = par->_parent;
        }
        return ret;
    }

    Vector2F SpriteObject::global2Local(const Vector2F &in)
    {
        Vector2F ret = in;
        SpriteObject* par = _parent;
        while (par != nullptr)
        {
            ret -= par->_pos;
            par = par->_parent;
        }
        return ret;
    }

    RectF SpriteObject::getSizeRect()
    {
        return RectF(getPos().x, getPos().y, getSize().x, getSize().y);
    }

    RectF SpriteObject::getSizeRectG()
    {
        Vector2F p = local2Global(getPos());
        return RectF(p.x, p.y, getSize().x, getSize().y);
    }

}

