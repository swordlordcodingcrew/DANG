// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "../DangFwdDecl.h"
#include "BMLNode.h"

namespace dang
{
    class BMLTree
    {
    public:
        static BMLNode::Status process(const spFullSpr& spr, spBMLState& state);

//        static BTNode* addInverter(BTNode* node);
//        static BTNode* addForwarder(BTNode* node);

    public:
        BMLTree();
        virtual ~BMLTree();

        uint8_t _nodeCount{0};

        const BMLNode* getRoot() {return _root;}

    protected:
        friend class BMLBuilder;

        BMLNode*  _root{nullptr};

    };


}

