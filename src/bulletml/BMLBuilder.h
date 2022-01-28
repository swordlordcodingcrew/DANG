// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <stack>

#include "../DangFwdDecl.h"

#include "BMLNode.h"

namespace dang
{

    class BMLBuilder
    {
    public:
        BMLBuilder();
        virtual ~BMLBuilder();

        BMLBuilder& sequence();
        BMLBuilder& selector();
        BMLBuilder& inverter();
        BMLBuilder& leaf(LeafFunction func);
        BMLBuilder& tree(const spBMLTree& tree);
        BMLBuilder& end();
        spBMLTree  build();

    protected:
        std::shared_ptr<BMLTree> _tree{nullptr};

        BMLNode* _builder_pos{nullptr};
        std::stack<BMLNode*> _composite_stack;

        BMLNode*    inComposite();
        BMLNode*    getLastSibling(const BMLNode* comp);
        void        attach(BMLNode* node);

        // helper function for copying a tree
        void        preOrderCopy(const BMLNode* node);
        BMLNode*    preOrderClone(const BMLNode* root);
    };

}


