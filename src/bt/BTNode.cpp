// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <cassert>
#include "BTNode.h"
#include "NTreeState.h"

namespace dang
{


    BTNode::BTNode(Type type) : _type(type)
    {
    }


    BTNode::~BTNode()
    {
        delete _child;
        delete _sibling;
    }

    bool BTNode::isLeaf() const
    {
        return (Type::LEAF == _type);
    }

    bool BTNode::isComposite() const
    {
        return (Type::COMPOSITE == _type);
    }

    bool BTNode::isDecorator() const
    {
        return (Type::DECORATOR == _type);
    }

    BTNode::Status   BTNode::forwarder(const spSprite& spr, const BTNode* node, spNTreeState& state)
    {
        assert(node->_child != nullptr);

        BTNode::Status ret{Status::FAILURE};
        BTNode* next_node = node->_child;

        ret = next_node->_process(spr, node->_child, state);

        return ret;
    }

    BTNode::Status   BTNode::inverter(const spSprite& spr, const BTNode* node, spNTreeState& state)
    {
        assert(node->_child != nullptr);

        BTNode::Status ret{Status::FAILURE};
        BTNode* next_node = node->_child;

        ret = next_node->_process(spr, node->_child, state);

        // apply inverter only when not seeking, not state set and not running
        if (state->_internal_state == NTreeState::internal_state::FOUND && ret != Status::RUNNING)
        {
            ret = (ret == Status::FAILURE ? Status::SUCCESS : Status::FAILURE);
        }

        return ret;
    }

    BTNode::Status BTNode::selector(const spSprite &spr, const BTNode* node, spNTreeState& state)
    {
        assert(node->_child != nullptr);

        BTNode::Status ret{Status::FAILURE};

        BTNode* next_node = node->_child;
        while (next_node != nullptr)
        {
            ret = next_node->_process(spr, next_node, state);

            if (state->_internal_state == NTreeState::internal_state::FOUND)
            {
                if (ret == Status::SUCCESS)
                {
                    break;
                }
            }
            else if (state->_internal_state == NTreeState::internal_state::SET)
            {
                break;
            }

            next_node = next_node->_sibling;
        }

        return ret;
    }

    BTNode::Status BTNode::sequence(const spSprite &spr, const BTNode* node, spNTreeState &state)
    {
        assert(node->_child != nullptr);

        Status ret{Status::FAILURE};

        BTNode* next_node = node->_child;
        while (next_node != nullptr)
        {
            ret = next_node->_process(spr, next_node, state);

            if (state->_internal_state == NTreeState::internal_state::FOUND)
            {
                if (ret == Status::FAILURE)
                {
                    break;
                }
            }
            else if (state->_internal_state == NTreeState::internal_state::SET)
            {
                break;
            }

            next_node = next_node->_sibling;

        }

        return ret;
    }

}
