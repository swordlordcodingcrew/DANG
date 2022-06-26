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

    BMLNode::Status BMLNode::sequence(const spFullSpr &spr, const BMLNode* node, spBMLState &state)
    {
        std::cout << "BMLNode sequence " << std::to_string(node->_id) << std::endl;

        assert(node->_child != nullptr);
        Status ret{Status::FAILURE};

        // TODO check if there is some need to check SEEKING

        // at every consequent run, jump here
        auto it = state->_payload.find( std::to_string(node->_id) + ".runcount");
        if(it != state->_payload.end())
        {
            // run once, lets quit otherwise
            auto val = it->second;
            if(val > 0)
            {
                return Status::SUCCESS;
            }
        }
        else
        {
            state->_payload[std::to_string(node->_id) + ".runcount"] = 0;
        }

        // execute every child node/leaf
        // TODO should only run first child on first run, second on the next run
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

        // ran once, lets not come back
        state->_payload[std::to_string(node->_id) + ".runcount"] = 1;
        return Status::SUCCESS;

        // TODO validate if always returning success of status of nodes?
        // return ret;
    }

    BMLNode::Status BMLNode::wait(const spFullSpr& spr, const BMLNode* node, spBMLState& state)
    {
        std::cout << "BMLNode wait " << std::to_string(node->_id) << std::endl;

        Status ret{Status::FAILURE};

        uint16_t runCount = 1;

        auto it = state->_payload.find( std::to_string(node->_id) + ".runcount");
        if(it != state->_payload.end())
        {
            auto val = it->second + 1;
            runCount = val;

            // TODO not sure this is correct here, probably check with SEEKING
            if(runCount >= node->_value)
            {
                std::cout << "BMLNode wait ended " << node->_value << std::endl;
                return Status::SUCCESS;
            }
        }

        state->_payload[std::to_string(node->_id) + ".runcount"] = runCount;

        std::cout << "BMLNode wait run count " << std::to_string(runCount) << " of " << node->_value << std::endl;

        // repeat n times in the next n rounds, quit otherwise
        if(runCount >= node->_value)
        {
            std::cout << "BMLNode wait ended " << node->_value << std::endl;
            return Status::SUCCESS;
        }
        else
        {
            return Status::RUNNING;
        }
    }


    BMLNode::Status BMLNode::repeat(const spFullSpr &spr, const BMLNode* node, spBMLState &state)
    {
        std::cout << "BMLNode repeat " << std::to_string(node->_id) << std::endl;

        assert(node->_child != nullptr);
        Status ret{Status::FAILURE};

        uint16_t runCount = 1;

        auto it = state->_payload.find( std::to_string(node->_id) + ".runcount");
        if(it != state->_payload.end())
        {
            auto val = it->second + 1;
            runCount = val;

            // TODO not sure this is correct here, probably check with SEEKING
            if(runCount >= node->_value)
            {
                std::cout << "BMLNode repeat ended " << node->_value << std::endl;
                return Status::SUCCESS;
            }
        }

        state->_payload[std::to_string(node->_id) + ".runcount"] = runCount;

        std::cout << "BMLNode repeat run count " << std::to_string(runCount) << " of " << node->_value << std::endl;

        // execute every child node/leaf
        // TODO should only run first child on first run, second on the next run
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

        // repeat n times in the next n rounds, quit otherwise
        if(runCount >= node->_value)
        {
            std::cout << "BMLNode repeat ended " << node->_value << std::endl;
            return Status::SUCCESS;
        }
        else
        {
            return Status::RUNNING;
        }
    }

}
