// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

#include <cassert>
#include <iostream>

#include "NTBuilder.h"
#include "NTreeState.h"
#include "NTree.h"

#include "../dang.hpp"

#ifdef DANG_DEBUG_PRINT

#ifdef TARGET_32BLIT_HW
#include "32blit.hpp"
#endif

#include <malloc.h>
#endif

namespace dang
{

    NTBuilder::NTBuilder()
    {
        _tree = std::make_shared<NTree>();
    }

    NTBuilder::~NTBuilder()
    {
//        std::cout << "NTBuilder destructor" << std::endl;
    }


    NTBuilder& NTBuilder::sequence()
    {
        BTNode* node = new BTNode(BTNode::Type::COMPOSITE);
        node->_process = BTNode::sequence;

        _composite_stack.push(node);

        attach(node);

        return *this;
    }

    NTBuilder &NTBuilder::selector()
    {
        BTNode* node = new BTNode(BTNode::Type::COMPOSITE);
        node->_process = BTNode::selector;

        _composite_stack.push(node);

        attach(node);

        return *this;
    }

    NTBuilder &NTBuilder::end()
    {
        BTNode* node = _composite_stack.top();
        _composite_stack.pop();
        _builder_pos = node->_parent;

        return *this;
    }

    NTBuilder &NTBuilder::inverter()
    {
        BTNode* node = new BTNode(BTNode::Type::DECORATOR);
        node->_process = BTNode::inverter;
        attach(node);

        return *this;
    }

    NTBuilder& NTBuilder::leaf(LeafFunction func)
    {
        BTNode* node = new BTNode(BTNode::Type::LEAF);
        node->_process = [func](FullSpr& spr, const BTNode* node, spNTreeState& state, uint32_t dt)
        {
            BTNode::Status ret{BTNode::Status::FAILURE};

            // check if to continue here
            if (state->_internal_state == NTreeState::internal_state::SEEKING)
            {
                if (state->_node != node)
                {
                    return ret; // not this node
                }
                else
                {
                    // correct node to continue
                    state->_internal_state = NTreeState::internal_state::FOUND;
                    state->_node = nullptr;
//                    std::cout << "node found" << std::endl;

                }
            }

            ret = func(spr, dt);

            // only leafs set the state
            if (ret == BTNode::Status::RUNNING)
            {
                state->_node = node;
                state->_internal_state = NTreeState::internal_state::SET;
//                std::cout << "state running" << std::endl;
            }

            return ret;
        };

        attach(node);

        return *this;
    }


    NTBuilder &NTBuilder::tree(const spNTree& tree)
    {
        BTNode* clone = preOrderClone(tree->_root);
        attach(clone);
        D_DEBUG_PRINT("NTBuilder, tree: copy done (%d)\r\n", mallinfo().uordblks);

        return *this;
    }


    BTNode* NTBuilder::inComposite()
    {
        if (_builder_pos->_parent == nullptr)
        {
            return nullptr;
        }

        BTNode* node = _builder_pos->_parent;
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

    BTNode* NTBuilder::getLastSibling(const BTNode* comp)
    {
        assert(comp != nullptr);
        assert(comp->isComposite());

        BTNode* node = comp->_child;
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

    void NTBuilder::attach(BTNode* node)
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
            BTNode* nd = getLastSibling(_builder_pos);
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
            BTNode* comp = inComposite();

            assert(comp != nullptr);    // attaching a node to a leaf ist not allowed

            node->_parent = comp;
            BTNode* nd = getLastSibling(comp);
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

    std::shared_ptr<NTree> NTBuilder::build()
    {
        assert(_composite_stack.empty());
        return std::move(_tree);
    }

    void NTBuilder::preOrderCopy(const BTNode *node)
    {

        if (node == nullptr)
        {
            D_DEBUG_PRINT("NTBuilder, preOrderCopy: empty node (%d)\r\n", mallinfo().uordblks);
            return;
        }
        D_DEBUG_PRINT("NTBuilder, preOrderCopy: copy node (%d)\r\n", mallinfo().uordblks);

        // copy the node and attach to the tree
        BTNode* nn = new BTNode(*node);
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

    BTNode* NTBuilder::preOrderClone(const BTNode *root)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        // copy the node and attach to the tree
        BTNode* nn = new BTNode(*root);
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
