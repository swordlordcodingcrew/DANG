// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "SprIterator.hpp"
#include "sprite/SpriteObject.hpp"

namespace dang
{
    SprIterator::SprIterator() : _current(nullptr), _root(nullptr)
    {
    }

    SprIterator::SprIterator(const spSprObj cur, const spSprObj root) : _current(cur), _root(root)
    {
    }

    // comparison operators. just compare node pointers
    bool SprIterator::operator== (const SprIterator& rhs) const
    {
        return _root == rhs._root && _current == rhs._current;
    }

    bool SprIterator::operator!= (const SprIterator& rhs) const
    {
        return _root != rhs._root || _current != rhs._current;
    }

    // dereference operator. return a reference to
    // the value pointed to by nodePtr
    const spSprObj& SprIterator::operator* () const
    {
        return _current;
    }

    /**
     * preincrement. move forward to next node
     * start at top and go down left (Pre Order)
     */
    SprIterator&    SprIterator::operator++ ()
    {
        if (_current == nullptr)
        {
            // ++ from end(). get the root of the tree
            _current = _root;
        }
        else if (_current->_child != nullptr)
        {
            _current = _current->_child;
        }
        else if (_current->_next_sibling != nullptr)
        {
            _current = _current->_next_sibling;
        }
        else
        {
            SpriteObject* par = _current->_parent;
            while (par != nullptr)
            {
                if (par->_next_sibling == nullptr)
                {
                    par = par->_parent;
                }
                else
                {
                    _current = par->_next_sibling;
                    break;
                }
            }

            if (par == nullptr)
            {
                _current = nullptr;
            }
        }

        return *this;
    }

    // postincrement
    SprIterator  SprIterator::operator++ (int)
    {
        auto saved = *this;
        operator++();
        return saved;
    }

}