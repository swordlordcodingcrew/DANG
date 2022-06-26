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

        uint8_t _id{0};

        virtual ~BMLNode();

    protected:
        friend class BMLTree;
        friend class BMLBuilder;
        using SequenceFunction = std::function<BMLNode::Status (const spFullSpr&, const BMLNode*, spBMLState&)>;


        /** https://www.geeksforgeeks.org/left-child-right-sibling-representation-tree/ */
        SequenceFunction    _process{nullptr};
        BMLNode*            _parent{nullptr};
        BMLNode*            _child{nullptr};    // left
        BMLNode*            _sibling{nullptr};  // right

        uint16_t            _value{0}; // generic value to store things like number of repeats

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
        static BMLNode::Status   sequence(const spFullSpr& spr, const BMLNode* node, spBMLState& state);
        static BMLNode::Status   repeat(const spFullSpr& spr, const BMLNode* node, spBMLState& state);
        static BMLNode::Status   wait(const spFullSpr& spr, const BMLNode* node, spBMLState& state);

        // sequence -> action
        // Contents - (repeat | fire | fireRef | changeSpeed | changeDirection | accel | wait | vanish | action | actionRef)*

        //sequence -> repeat
        // Contents - times, (action | actionRef)

        // leaf -> wait
    };

    using SequenceFunction = std::function<BMLNode::Status (const spFullSpr&, const BMLNode*, spBMLState&)>;
    using ActionFunction = std::function<BMLNode::Status (const spFullSpr&)>;

}
