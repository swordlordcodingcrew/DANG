// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#pragma once

#include <functional>
#include <cstdint>
#include <memory>

#include "../DangFwdDecl.h"


namespace dang
{
    class BMLNode
    {

    /** The status returned by process functions. */
    public:
        enum class Status : uint8_t
        {
            FAILURE = 0, //!< Returned when the process function has failed (should not happen, TODO)
            RUNNING, //!< Returned when the tasks is still running (repeater, as example).
            SUCCESS //!< Returns when the process has succeeded.
        };

        virtual ~BMLNode();

    protected:
        friend class BMLTree;
        friend class BMLBuilder;
        using NodeFunction = std::function<BMLNode::Status (const spSprite&, const BMLNode*, spBMLState&)>;


        /** https://www.geeksforgeeks.org/left-child-right-sibling-representation-tree/ */
        NodeFunction    _process{nullptr};
        BMLNode*         _parent{nullptr};
        BMLNode*         _child{nullptr};    // left
        BMLNode*         _sibling{nullptr};  // right

        enum class Type : uint8_t
        {
            LEAF = 0,
            COMPOSITE,
            DECORATOR
        };
        Type           _type{Type::LEAF};

        explicit BMLNode(Type type);

        bool            isLeaf() const;
        bool            isComposite() const;
        bool            isDecorator() const;

        /** static behoviour tree elements */
        static BMLNode::Status   forwarder(const spSprite& spr, const BMLNode* node, spBMLState& state);
        static BMLNode::Status   inverter(const spSprite& spr, const BMLNode* node, spBMLState& state);
        static BMLNode::Status   selector(const spSprite& spr, const BMLNode* node, spBMLState& state);
        static BMLNode::Status   sequence(const spSprite& spr, const BMLNode* node, spBMLState& state);

    };

    using NodeFunction = std::function<BMLNode::Status (const spSprite&, const BMLNode*, spBMLState&)>;
    using LeafFunction = std::function<BMLNode::Status (const spSprite&)>;

}
