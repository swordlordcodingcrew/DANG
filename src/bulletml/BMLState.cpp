// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "BMLState.h"

namespace dang
{
    BMLState::BMLState(std::shared_ptr<BMLTree> tree) : _tree(tree)
    {
    }

    BMLState::~BMLState()
    {
//        std::cout << "NTreeState destructor" << std::endl;
    }

    void BMLState::clearState()
    {
        _node = nullptr;
        _internal_state = internal_state::FOUND;
    }
}
