// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <stack>

#include "../DangFwdDecl.h"

#include "BTNode.h"

namespace dang
{

    class NTBuilder
    {
    public:
        NTBuilder();
        virtual ~NTBuilder();

        NTBuilder& sequence();
        NTBuilder& selector();
        NTBuilder& leaf(LeafFunction func);
        NTBuilder& end();
        std::shared_ptr<NTree>  build();

    protected:
        std::shared_ptr<NTree> _tree{nullptr};

        BTNode* _builder_pos{nullptr};
        std::stack<BTNode*> _composite_stack;

        BTNode* inComposite();
        BTNode* getLastSibling(const BTNode* comp);
        void    attach(BTNode* node);
    };

}


