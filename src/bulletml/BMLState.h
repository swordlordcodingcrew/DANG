// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "../DangFwdDecl.h"

namespace dang
{
    class BMLState
    {
    public:
        BMLState() = delete;
        explicit BMLState(std::shared_ptr<BMLTree> tree);
        virtual ~BMLState();
        void    clearState();

        std::shared_ptr<BMLTree>                _tree{nullptr};
        const BMLNode*                          _node{nullptr};
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


