// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "../DangFwdDecl.h"
#include "BTNode.h"

namespace dang
{
    class NTree
    {
    public:
        static BTNode::Status process(const spSprite& spr, spNTreeState& state);

//        static BTNode* addInverter(BTNode* node);
//        static BTNode* addForwarder(BTNode* node);

    public:
        NTree();
        virtual ~NTree();

        const BTNode* getRoot() {return _root;}

    protected:
        friend class NTBuilder;

        BTNode*  _root{nullptr};

    };


}
