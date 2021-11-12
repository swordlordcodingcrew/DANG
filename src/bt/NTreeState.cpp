// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <iostream>
#include "NTreeState.h"

namespace dang
{
    NTreeState::NTreeState(std::shared_ptr<NTree> tree) : _tree(tree)
    {
    }

    NTreeState::~NTreeState()
    {
//        std::cout << "NTreeState destructor" << std::endl;
    }

    void NTreeState::clearState()
    {
        _node = nullptr;
        _internal_state = internal_state::FOUND;
    }

}
