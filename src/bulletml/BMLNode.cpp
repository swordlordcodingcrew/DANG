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

    BMLNode::Status   BMLNode::forwarder(const spSprite& spr, const BMLNode* node, spBMLState& state)
    {
        assert(node->_child != nullptr);

        BMLNode::Status ret{Status::FAILURE};
        BMLNode* next_node = node->_child;

        ret = next_node->_process(spr, node->_child, state);

        return ret;
    }

    BMLNode::Status   BMLNode::inverter(const spSprite& spr, const BMLNode* node, spBMLState& state)
    {
        assert(node->_child != nullptr);

        BMLNode::Status ret{Status::FAILURE};
        BMLNode* next_node = node->_child;

        ret = next_node->_process(spr, node->_child, state);

        // apply inverter only when not seeking, not state set and not running
        if (state->_internal_state == BMLState::internal_state::FOUND && ret != Status::RUNNING)
        {
            ret = (ret == Status::FAILURE ? Status::SUCCESS : Status::FAILURE);
        }

        return ret;
    }

    BMLNode::Status BMLNode::selector(const spSprite &spr, const BMLNode* node, spBMLState& state)
    {
        assert(node->_child != nullptr);

        BMLNode::Status ret{Status::FAILURE};

        BMLNode* next_node = node->_child;
        while (next_node != nullptr)
        {
            ret = next_node->_process(spr, next_node, state);

            if (state->_internal_state == BMLState::internal_state::FOUND)
            {
                if (ret == Status::SUCCESS)
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

}
