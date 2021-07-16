// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <stack>
#include "BTNode.h"
#include "NTree.h"

namespace dang
{

    class NTBuilder
    {
    public:
        NTBuilder();

        NTBuilder& sequence();
        NTBuilder& selector();
//        NTBuilder& leaf(NodeFunction func);
        NTBuilder& leaf(LeafFunction func);
        NTBuilder& end();
        std::shared_ptr<NTree>  build();

    protected:
        std::shared_ptr<NTree> _tree{nullptr};
//        BTNode* _root{nullptr};

        BTNode* _builder_pos{nullptr};
        std::stack<BTNode*> _composite_stack;

        BTNode* inComposite();
        BTNode* getLastSibling(const BTNode* comp);
        void    attach(BTNode* node);
    };

}


