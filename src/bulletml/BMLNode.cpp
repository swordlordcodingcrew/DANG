// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <cassert>
#include <iostream>

#include "BMLNode.h"
#include "BMLState.h"

namespace dang
{

    BMLNode::BMLNode(Type type) : _type(type)
    {
//        std::cout << "BTNode constructor" << std::endl;
    }


    BMLNode::~BMLNode()
    {
//        std::cout << "BTNode destructor" << std::endl;
        delete _child;
        delete _sibling;
    }

    bool BMLNode::isLeaf() const
    {
        return (Type::LEAF == _type);
    }

    bool BMLNode::isComposite() const
    {
        return (Type::COMPOSITE == _type);
    }

    bool BMLNode::isDecorator() const
    {
        return (Type::DECORATOR == _type);
    }

    BMLNode::Status BMLNode::sequence(const spSprite &spr, const BMLNode* node, spBMLState &state)
    {
        assert(node->_child != nullptr);

        Status ret{Status::FAILURE};

        BMLNode* next_node = node->_child;
        while (next_node != nullptr)
        {
            ret = next_node->_process(spr, next_node, state);

            if (state->_internal_state == BMLState::internal_state::FOUND)
            {
                if (ret == Status::FAILURE)
                {
                    break;
                }
            }
            else if (state->_internal_state == BMLState::internal_state::SET)
            {
                break;
            }

            next_node = next_node->_sibling;

        }

        return ret;
    }

    BMLNode::Status BMLNode::repeat(const spSprite &spr, const BMLNode* node, spBMLState &state)
    {
        assert(node->_child != nullptr);

        Status ret{Status::FAILURE};

        /*
         * TODO implement iterator, checking current round from state and max rounds from _value
        //auto addr = static_cast<const void*>(node);
        auto it = state->_payload.find("test");
        if(it != state->_payload.end())
        {

        }
        it->second
        */

        BMLNode* next_node = node->_child;
        while (next_node != nullptr)
        {
            ret = next_node->_process(spr, next_node, state);

            if (state->_internal_state == BMLState::internal_state::FOUND)
            {
                if (ret == Status::FAILURE)
                {
                    break;
                }
            }
            else if (state->_internal_state == BMLState::internal_state::SET)
            {
                break;
            }

            next_node = next_node->_sibling;

        }

        return ret;
    }

}
