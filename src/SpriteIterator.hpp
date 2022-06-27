// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#ifdef DANG_SPR

#include <iterator>
#include <memory>

namespace dang
{
    class Sprite;
    using spSprite = std::shared_ptr<Sprite>;

    class SpriteIterator : public std::iterator<std::forward_iterator_tag, spSprite>
    {
    public:
        SpriteIterator();

        // comparison operators. just compare node pointers
        bool operator== (const SpriteIterator& rhs) const;

        bool operator!= (const SpriteIterator& rhs) const;

        // dereference operator. return a reference to
        // the value pointed to by nodePtr
        const spSprite& operator* () const;

        // preincrement. move forward to next larger value
        SpriteIterator& operator++ ();

        // postincrement
        SpriteIterator operator++ (int);

        // predecrement. move backward
        SpriteIterator  operator-- ();

        // postdecrement
        SpriteIterator  operator-- (int);
    protected:
        friend class SpriteLayer;

        spSprite _current;
        spSprite _root;

        // used to construct an iterator return value from
        // a node pointer
        SpriteIterator (const spSprite cur, const spSprite root);
    };

}

#endif