// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <iterator>
#include <memory>

namespace dang
{
    class SpriteObject;
    using spSprObj = std::shared_ptr<SpriteObject>;

    class SprIterator : public std::iterator<std::forward_iterator_tag, spSprObj>
    {
    public:
        SprIterator();

        // comparison operators. just compare node pointers
        bool operator== (const SprIterator& rhs) const;

        bool operator!= (const SprIterator& rhs) const;

        // dereference operator. return a reference to
        // the value pointed to by nodePtr
        const spSprObj& operator* () const;

        // preincrement. move forward to next larger value
        SprIterator& operator++ ();

        // postincrement
        SprIterator operator++ (int);

        // predecrement. move backward
        SprIterator  operator-- ();

        // postdecrement
        SprIterator  operator-- (int);
    protected:
        friend class SprLayer;

        spSprObj _current;
        spSprObj _root;

        // used to construct an iterator return value from
        // a node pointer
        SprIterator (const spSprObj cur, const spSprObj root);
    };

}
