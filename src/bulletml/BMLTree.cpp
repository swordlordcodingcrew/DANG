// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

//#include <iostream>

#include "BMLTree.h"
#include "BMLState.h"

namespace dang
{
    BMLNode::Status BMLTree::process(const spSprite& spr, std::shared_ptr<BMLState>& state)
    {
        BMLNode::Status ret{BMLNode::Status::FAILURE};

        if (state->_node == nullptr)
        {
            // start from the beginning
            state->_internal_state = BMLState::internal_state::FOUND;
//            std::cout << "state not set" << std::endl;
        }
        else
        {
            // seek the node
            state->_internal_state = BMLState::internal_state::SEEKING;
//            std::cout << "state set" << std::endl;
        }

        ret = state->_tree->_root->_process(spr, state->_tree->_root, state);

//        assert(state->_internal_state != NTreeState::internal_state::SEEKING)

        return ret;
    }

    BMLTree::BMLTree()
    {
    }


    BMLTree::~BMLTree()
    {
//        std::cout << "NTree destructor" << std::endl;
        delete _root;
    }

}
