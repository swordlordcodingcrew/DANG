// (c) 2019-22 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <cassert>
#include <iostream>

#include "BMLBuilder.h"
#include "BMLState.h"
#include "BMLTree.h"

#include "../dang.hpp"

#ifdef DANG_DEBUG_PRINT

#ifdef TARGET_32BLIT_HW
#include "32blit.hpp"
#endif

#include <malloc.h>
#endif

namespace dang
{

    BMLBuilder::BMLBuilder()
    {
        _tree = std::make_shared<BMLTree>();
    }

    BMLBuilder::~BMLBuilder()
    {
//        std::cout << "NTBuilder destructor" << std::endl;
    }


    BMLBuilder& BMLBuilder::sequence()
    {
        BMLNode* node = new BMLNode(BMLNode::Type::COMPOSITE);
        node->_process = BMLNode::sequence;

        _composite_stack.push(node);

        attach(node);

        return *this;
    }

    BMLBuilder &BMLBuilder::selector()
    {
        BMLNode* node = new BMLNode(BMLNode::Type::COMPOSITE);
        node->_process = BMLNode::selector;

        _composite_stack.push(node);

        attach(node);

        return *this;
    }

    BMLBuilder &BMLBuilder::end()
    {
        BMLNode* node = _composite_stack.top();
        _composite_stack.pop();
        _builder_pos = node->_parent;

        return *this;
    }

    BMLBuilder &BMLBuilder::inverter()
    {
        BMLNode* node = new BMLNode(BMLNode::Type::DECORATOR);
        node->_process = BMLNode::inverter;
        attach(node);

        return *this;
    }

    BMLBuilder& BMLBuilder::leaf(LeafFunction func)
    {
        BMLNode* node = new BMLNode(BMLNode::Type::LEAF);
        node->_process = [func](const spSprite& spr, const BMLNode* node, spBMLState& state)
        {
            BMLNode::Status ret{BMLNode::Status::FAILURE};

            // check if to continue here
            if (state->_internal_state == BMLState::internal_state::SEEKING)
            {
                if (state->_node != node)
                {
                    return ret; // not this node
                }
                else
                {
                    // correct node to continue
                    state->_internal_state = BMLState::internal_state::FOUND;
                    state->_node = nullptr;
//                    std::cout << "node found" << std::endl;

                }
            }

            ret = func(spr);

            // only leafs set the state
            if (ret == BMLNode::Status::RUNNING)
            {
                state->_node = node;
                state->_internal_state = BMLState::internal_state::SET;
//                std::cout << "state running" << std::endl;
            }

            return ret;
        };

        attach(node);

        return *this;
    }

/*    NTBuilder& NTBuilder::leaf(NodeFunction func)
    {
        BTNode* node = new BTNode(BTNode::Type::LEAF);
        node->_process = func;

        attach(node);

        return *this;
    }
*/

    BMLBuilder &BMLBuilder::tree(const spBMLTree& tree)
    {
//        BTNode* _pos = _builder_pos;
        BMLNode* clone = preOrderClone(tree->_root);
        attach(clone);
//        preOrderCopy(tree->_root);
//        _builder_pos = _pos;
        D_DEBUG_PRINT("NTBuilder, tree: copy done (%d)\r\n", mallinfo().uordblks);

        return *this;
    }


    BMLNode* BMLBuilder::inComposite()
    {
        if (_builder_pos->_parent == nullptr)
        {
            return nullptr;
        }

        BMLNode* node = _builder_pos->_parent;
        while (node != nullptr)
        {
            if (node->isComposite())
            {
                return node;
            }
            node = node->_parent;
        }
        return nullptr;
    }

    BMLNode* BMLBuilder::getLastSibling(const BMLNode* comp)
    {
        assert(comp != nullptr);
        assert(comp->isComposite());

        BMLNode* node = comp->_child;
        if (node == nullptr)
        {
            return nullptr;
        }

        while (node->_sibling != nullptr)
        {
            node = node->_sibling;
        }
        return node;
    }

    void BMLBuilder::attach(BMLNode* node)
    {
//        assert(_builder_pos != nullptr);
        assert(node != nullptr);

        if (_tree->_root == nullptr)   // first node to attach
        {
            _tree->_root = node;
            _builder_pos = _tree->_root;
        }
        else if (_builder_pos->isDecorator())   // the node is attached to a decorator
        {
            node->_parent = _builder_pos;
            _builder_pos->_child = node;
        }
        else if (_builder_pos->isComposite())   // the node is attached directly to a composite
        {
            node->_parent = _builder_pos;
            BMLNode* nd = getLastSibling(_builder_pos);
            if (nd != nullptr)
            {
                nd->_sibling = node;
            }
            else
            {
                _builder_pos->_child = node;
            }
        }
        else    // position shows to a leaf - check, if there is a composite. If not -> error
        {
            BMLNode* comp = inComposite();

            assert(comp != nullptr);    // attaching a node to a leaf ist not allowed

            node->_parent = comp;
            BMLNode* nd = getLastSibling(comp);
            if (nd != nullptr)
            {
                nd->_sibling = node;
            }
            else
            {
                comp->_child = node;
            }
        }
        _builder_pos = node;

    }

    std::shared_ptr<BMLTree> BMLBuilder::build()
    {
        assert(_composite_stack.empty());
        return std::move(_tree);
    }

    void BMLBuilder::preOrderCopy(const BMLNode *node)
    {

        if (node == nullptr)
        {
            D_DEBUG_PRINT("NTBuilder, preOrderCopy: empty node (%d)\r\n", mallinfo().uordblks);
            return;
        }
        D_DEBUG_PRINT("NTBuilder, preOrderCopy: copy node (%d)\r\n", mallinfo().uordblks);

        // copy the node and attach to the tree
        BMLNode* nn = new BMLNode(*node);
        nn->_parent = nullptr;
        nn->_sibling = nullptr;
        nn->_child = nullptr;
        attach(nn);

        preOrderCopy(node->_child);

        preOrderCopy(node->_sibling);
    }


    /**
     * make a copy of a tree
     * inspired: https://www.techcrashcourse.com/2016/06/c-program-to-create-duplicate-binary-tree.html
     * @param root tree to clone
     * @return cloned tree
     */

    BMLNode* BMLBuilder::preOrderClone(const BMLNode *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        // copy the node and attach to the tree
        BMLNode* nn = new BMLNode(*root);
        nn->_parent = nullptr;
        nn->_sibling = nullptr;
        nn->_child = nullptr;

        /* Recursively create clone of left and right sub tree */
        nn->_child = preOrderClone(root->_child);
        nn->_sibling = preOrderClone(root->_sibling);
        /* Return root of cloned tree */
        return nn;

    }

}
