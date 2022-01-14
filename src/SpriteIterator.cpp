// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include "SpriteIterator.hpp"
#include "Sprite.hpp"

namespace dang
{
    SpriteIterator::SpriteIterator()
            : _current(nullptr), _root(nullptr)
    {
    }

    SpriteIterator::SpriteIterator(const spSprite cur, const spSprite root)
            : _current(cur), _root(root)
    {
    }

    // comparison operators. just compare node pointers
    bool SpriteIterator::operator== (const SpriteIterator& rhs) const
    {
        return _root == rhs._root && _current == rhs._current;
    }

    bool SpriteIterator::operator!= (const SpriteIterator& rhs) const
    {
        return _root != rhs._root || _current != rhs._current;
    }

    // dereference operator. return a reference to
    // the value pointed to by nodePtr
    const spSprite& SpriteIterator::operator* () const
    {
/*        if (_current == nullptr)
        {
            throw UnderflowException{};
        }
*/        return _current;
    }

    // preincrement. move forward to next node
    SpriteIterator&    SpriteIterator::operator++ ()
    {
        if (_current == nullptr)
        {
            // ++ from end(). get the root of the tree
            _current = _root;

            // error! ++ requested for an empty tree
/*            if (_current == nullptr)
            {
                throw UnderflowException { };
            }
*/
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
            spSprite s = _current->_parent.lock();
            while (s != nullptr)
            {
                if (s->_next_sibling == nullptr)
                {
                    s = s->_parent.lock();
                }
                else
                {
                    _current = s->_next_sibling;
                    break;
                }
            }

            if (s == nullptr)
            {
                _current = nullptr;
            }
        }

        return *this;
    }

    // postincrement

    SpriteIterator  SpriteIterator::operator++ (int)
    {
        auto saved = *this;
        operator++();
        return saved;
    }

}