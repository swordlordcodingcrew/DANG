// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "BTNode.h"
#include "NTreeState.h"

namespace dang
{
    class NTreeState;
    using spNTreeState = std::shared_ptr<NTreeState>;
    using spNTree = std::shared_ptr<NTree>;

    class NTree
    {
    public:
        static BTNode::Status process(const spSprite& spr, std::shared_ptr<NTreeState>& state);

//        static BTNode* addInverter(BTNode* node);
//        static BTNode* addForwarder(BTNode* node);

    public:
        NTree();
        ~NTree();

        const BTNode* getRoot() {return _root;}

    protected:
        friend class NTBuilder;

        BTNode*  _root{nullptr};

    };


}

