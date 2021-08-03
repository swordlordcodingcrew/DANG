// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <memory>
#include <unordered_map>

#include "../DangFwdDecl.h"

namespace dang
{
    class NTreeState
    {
    public:
        NTreeState() = delete;
        explicit NTreeState(std::shared_ptr<NTree> tree);
        virtual ~NTreeState();

        std::shared_ptr<NTree>  _tree{nullptr};
        const BTNode*           _node{nullptr};
        std::unordered_map<std::string, float>  _payload;

        enum class internal_state : uint8_t
        {
            FOUND = 0,
            SEEKING,
            SET
        };
        internal_state _internal_state{internal_state::FOUND};
    };

}


