// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

// This file is based on beehive - https://github.com/crust/beehive

#include <iostream>
#include "bt.hpp"

namespace dang
{
    TreeState::TreeState(std::shared_ptr<BehaviourTree> tree) : _tree(move(tree))
    {

    }


    /*!
 \brief Composite that returns success if all children return success.
*/
    BTNodeStatus sequence(const std::shared_ptr<Sprite>& context, Generator const &next_child, std::shared_ptr<TreeState>& state)
    {
        while (auto const *child = next_child())
        {
            auto status = child->process(context, state);
            if (status != BTNodeStatus::SUCCESS)
            {
                if (status == BTNodeStatus::FAILURE)
                {
                    return status;
                }
                return status;
            }
        }
        return BTNodeStatus::SUCCESS;
    }

/*!
 \brief Composite that returns success on the first successful call.
*/
    BTNodeStatus selector(const std::shared_ptr<Sprite>& context, Generator const &next_child, std::shared_ptr<TreeState>& state)
    {
//        std::cout << "--- selector" << std::endl;
        while (auto const *child = next_child())
        {
            auto status = child->process(context, state);
            if (status != BTNodeStatus::FAILURE)
            {
                return status;
            }
        }
        return BTNodeStatus::FAILURE;
    }

    /*!
 \brief Composite that returns success on the first successful call.
*/
    BTNodeStatus randomiser(const std::shared_ptr<Sprite>& context, Generator const &next_child, std::shared_ptr<TreeState>& state)
    {
        while (auto const *child = next_child())
        {
            auto status = child->process(context, state);
            if (status != BTNodeStatus::FAILURE)
            {
                return status;
            }
        }
        return BTNodeStatus::FAILURE;
    }

/*!
 \brief Decorator that just returns the result of the child. Not very useful...
*/
    BTNodeStatus forwarder(const std::shared_ptr<Sprite>& context, Node const &child, std::shared_ptr<TreeState>& state)
    {
        return child.process(context, state);
    }

/*!
 \brief Decorator that inverts the result of its child node.
*/
    BTNodeStatus inverter(const std::shared_ptr<Sprite>& context, Node const &child, std::shared_ptr<TreeState>& state)
    {
        const auto status = child.process(context, state);
        // this is to allow for the process function to also return a bool which is then translated to Status
        if (status == BTNodeStatus::RUNNING)
        {
            return status;
        }
        return status == BTNodeStatus::FAILURE ? BTNodeStatus::SUCCESS : BTNodeStatus::FAILURE;
    }

/*!
 \brief Decorator that returns success regardless of the child result.
*/
    BTNodeStatus succeeder(const std::shared_ptr<Sprite>& context, Node const &child, std::shared_ptr<TreeState>& state)
    {
        child.process(context, state);
        return BTNodeStatus::SUCCESS;
    }

/*!
 \brief A leaf that always succeeds. Not very useful...
*/
    BTNodeStatus noop(const std::shared_ptr<Sprite>&)
    {
        return BTNodeStatus::SUCCESS;
    }

    BehaviourTree::BehaviourTree(std::vector<Node> nodes) : _nodes(move(nodes))
    {
        size_t i = 0;
        for (auto &node : _nodes) {
            node._index = i++;
        }
    }

    BTNodeStatus BehaviourTree::process(std::shared_ptr<TreeState>& state, const std::shared_ptr<Sprite>& context) const
    {
        // bug fix for correct state handling
        state->_set = false;
        // TODO redo assert with pointer?
        //assert(state->_tree_id == _id); // another tree's state used with this tree
//        return _nodes.at(state->resume_index()).process(context, state);
        return _nodes.at(0).process(context, state);
    }

/// @cond

    void Node::add_child()
    {
        ++_child_count;
    }

    Node::ProcessFunction BuilderBase::make_branch(Decorator f)
    {
        return [process = move(f)](const std::shared_ptr<Sprite>& context, Node const &self, std::shared_ptr<TreeState>& state)
        {
            assert(self.child_count() == 1); // invariant violation!
            auto &child = *(&self + 1);
            return process(context, child, state);
        };
    }

    Node::ProcessFunction BuilderBase::make_branch(Composite f)
    {
        return [process = move(f)](const std::shared_ptr<Sprite>& context, Node const &self, std::shared_ptr<TreeState>& state)
        {
            size_t i = 0; // C specs say: size_t ... is the unsigned integer type of the result of the sizeof operator
            auto *child = self.first_child();
            if (self.index() == state->resume_index()) {
                for (; i < state->offset(); ++i) {
                    child = child->next_sibling();
                }
            }
            auto generator = [&self, &i, &child]() -> Node const * {
                if (i++ == self.child_count()) {
                    return nullptr;
                }
                auto c = child;
                child = child->next_sibling();
                return c;
            };
            auto status = process(context, generator, state);
            if (status == BTNodeStatus::RUNNING) {
                if (!state->_set)
                {
                    self.save_state_at_child_index(state, i - 1);
                }
            } else {
                self.clear_state(state);
            }
            return status;
        };
    }

    Node::ProcessFunction BuilderBase::make_leaf(Leaf f)
    {
        return [process = move(f)](const std::shared_ptr<Sprite>& context, Node const &self, std::shared_ptr<TreeState>& state)
        {
            assert(self.child_count() == 0); // invariant violation!
            return process(context);
        };
    }

    Node::ProcessFunction BuilderBase::make_leaf(VoidLeaf f)
    {
        return make_leaf(Leaf{[void_process = move(f)](const std::shared_ptr<Sprite>& context)
                             {
                                 void_process(context);
                                 return BTNodeStatus::SUCCESS;
                             }});
    }

    Node::ProcessFunction BuilderBase::make_leaf(BoolLeaf f)
    {
        return make_leaf(Leaf{[bool_process = move(f)](const std::shared_ptr<Sprite>& context)
                             {
                                 const bool result = bool_process(context);
                                 return result ? BTNodeStatus::SUCCESS : BTNodeStatus::FAILURE;
                             }});
    }

    BuilderBase BuilderBase::composite(Composite composite)
    {
        return _branch(std::move(composite));
    }

    BuilderBase BuilderBase::decorator(Decorator decorator)
    {
        return _branch(std::move(decorator));
    }

    template<typename BranchType>
    auto BuilderBase::_branch(BranchType &&branch) -> BuilderBase
    {
        assert((_type != Type::DECORATOR) || node().child_count() == 0); // Decorators may only have one child!
        auto type = std::is_same<
                typename std::decay<BranchType>::type,
                Decorator
        >::value ? Type::DECORATOR : Type::COMPOSITE;
        auto child_offset = add_child(make_branch(move(branch)));
        return {*this, child_offset, type};
    }

    template<typename LeafType>
    auto BuilderBase::_leaf(LeafType &&leaf) -> BuilderBase &
    {
        assert((_type != Type::DECORATOR) || node().child_count() == 0); // Decorators may only have one child!
        add_child(make_leaf(move(leaf)));
        return *this;
    }

    auto BuilderBase::leaf(Leaf leaf) -> BuilderBase &
    {
        return _leaf(std::move(leaf));
    }

    auto BuilderBase::leaf(BoolLeaf leaf) -> BuilderBase &
    {
        return _leaf(std::move(leaf));
    }

    auto BuilderBase::void_leaf(VoidLeaf leaf) -> BuilderBase &
    {
        return _leaf(std::move(leaf));
    }

    auto BuilderBase::tree(BehaviourTree const &subtree) -> BuilderBase &
    {
        assert((_type != Type::DECORATOR) || node().child_count() == 0); // Decorators may only have one child!
        auto const &subtree_nodes = subtree.nodes();
        copy(subtree_nodes.begin(), subtree_nodes.end(), back_inserter(nodes()));
        node().add_child();
        return *this;
    }

    auto BuilderBase::end() -> BuilderBase &
    {
        assert(node().child_count() > 0); // can't have composite/decorator without children!
        return _parent;
    }

    auto BuilderBase::build() const & -> BehaviourTree
    {
        assert(false); // unterminated tree!
        return {{}};
    }

    auto BuilderBase::build() && -> BehaviourTree
    {
        assert(false); // unterminated tree!
        return {{}};
    }

    auto BuilderBase::sequence() -> BuilderBase
    {
        return _branch(Composite{&dang::sequence});
    }
    auto BuilderBase::selector() -> BuilderBase
    {
        return _branch(Composite{&dang::selector});
    }
    auto BuilderBase::inverter() -> BuilderBase
    {
        return _branch(Decorator{&dang::inverter});
    }
    auto BuilderBase::succeeder() -> BuilderBase
    {
        return _branch(Decorator{&dang::succeeder});
    }

/// @endcond

} // namespace dang