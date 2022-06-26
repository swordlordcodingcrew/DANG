// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include "DangFwdDecl.h"

#include <functional>
//#include <cstdint>
//#include <memory>

namespace dang
{
    class BTNode
    {

    /** The status returned by process functions. */
    public:
        enum class Status : uint8_t
        {
            FAILURE = 0, //!< Returned when the process function has failed.
            RUNNING, //!< Returned when the outcome of process has not been determined yet.
            SUCCESS //!< Returns when the process has succeeded.
        };

        virtual ~BTNode();

    protected:
        friend class NTree;
        friend class NTBuilder;
        using NodeFunction = std::function<BTNode::Status (FullSpr&, const BTNode*, spNTreeState&, uint32_t dt)>;


        /** https://www.geeksforgeeks.org/left-child-right-sibling-representation-tree/ */
        NodeFunction    _process{nullptr};
        BTNode*         _parent{nullptr};
        BTNode*         _child{nullptr};    // left
        BTNode*         _sibling{nullptr};  // right

        enum class Type : uint8_t
        {
            LEAF = 0,
            COMPOSITE,
            DECORATOR
        };
        Type           _type{Type::LEAF};

        explicit BTNode(Type type);

        bool            isLeaf() const;
        bool            isComposite() const;
        bool            isDecorator() const;

        /** static behoviour tree elements */
        static BTNode::Status   forwarder(FullSpr& spr, const BTNode* node, spNTreeState& state, uint32_t dt);
        static BTNode::Status   inverter(FullSpr& spr, const BTNode* node, spNTreeState& state, uint32_t dt);
        static BTNode::Status   selector(FullSpr& spr, const BTNode* node, spNTreeState& state, uint32_t dt);
        static BTNode::Status   sequence(FullSpr& spr, const BTNode* node, spNTreeState& state, uint32_t dt);

    };

    using NodeFunction = std::function<BTNode::Status (FullSpr&, const BTNode*, spNTreeState&, uint32_t dt)>;
    using LeafFunction = std::function<BTNode::Status (FullSpr&, uint32_t dt)>;

}
