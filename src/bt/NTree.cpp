// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "NTree.h"

namespace dang
{

    BTNode::Status NTree::process(const spSprite& spr, std::shared_ptr<NTreeState>& state)
    {
        BTNode::Status ret{BTNode::Status::FAILURE};

        if (state->_node == nullptr)
        {
            // start from the beginning
            state->_internal_state = NTreeState::internal_state::FOUND;
//            std::cout << "state not set" << std::endl;
        }
        else
        {
            // seek the node
            state->_internal_state = NTreeState::internal_state::SEEKING;
//            std::cout << "state set" << std::endl;
        }

        ret = state->_tree->_root->_process(spr, state->_tree->_root, state);

//        assert(state->_internal_state != NTreeState::internal_state::SEEKING)

        return ret;
    }

    NTree::NTree()
    {
    }


    NTree::~NTree()
    {
        delete _root;
    }

}
