// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the DANG game framework

// This file is based on beehive - https://github.com/crust/beehive
#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace dang
{
    class Sprite;

    /*
    class EnemiesAroundChecker
    {
    public:

        bool operator()(std::shared_ptr<Sprite> s) const {
            return _enemies_around;
        }
        bool _enemies_around{false};
    };
     */

    /*
           auto tr = dang::Builder{}
           .sequence()
           .leaf([](std::shared_ptr<dang::Sprite> s) -> dang::BTNodeStatus { // Passing a lambda
               return s->is_hungry ? dang::BTNodeStatus::SUCCESS : dang::BTNodeStatus::FAILURE;
           })
           .leaf(dang::Sprite::BTLoiter) // Passed a member function pointer
           .leaf([](std::shared_ptr<dang::Sprite> s) { return dang::BTNodeStatus::RUNNING; })
           .inverter()
           .leaf(dang::EnemiesAroundChecker{}) // Passing functor
           .end()
           .void_leaf(&dang::Sprite::eat_food) // Void member function
           .end()
           .build();

/*!
\brief The status returned by process functions.
*/
    enum class BTNodeStatus : uint8_t
    {
        FAILURE = 0, //!< Returned when the process function has failed.
        RUNNING, //!< Returned when the outcome of process has not been determined yet.
        SUCCESS //!< Returns when the process has succeeded.
    };

    /*!
 \brief Pass a TreeState instance to #beehive::Tree's process function in order to resume Running nodes. Instantiate with #beehive::Tree::make_state.
*/
    class BehaviourTree;

    class TreeState
    {

    public:
        TreeState(std::shared_ptr<BehaviourTree> tree);
        TreeState();
        //TreeState(std::shared_ptr<Tree> sharedPtr);

        // For internal use only.
        uint16_t resume_index() const {
            return _resume_index;
        }
        // For internal use only.
        uint16_t offset() const {
            return _offset;
        }

        // For internal use only. Moves the ownership
        void setTree(std::shared_ptr<BehaviourTree> tree) {
            _tree = std::move(tree);
        }

        const std::shared_ptr<BehaviourTree>& getTree()
        {
            return _tree;
        }

        //private:

        //size_t _tree_id;
        std::shared_ptr<BehaviourTree> _tree{nullptr};
        uint16_t _resume_index{};
        uint16_t _offset{};

        friend class BehaviourTree;
        friend struct Node;


    };

/*!
 \brief A handle on a process function. This should not be built directly, see #beehive::Builder.
*/
    class Node
    {
    public:

        using ProcessFunction = std::function<BTNodeStatus(const std::shared_ptr<Sprite>& context, Node const &self, std::shared_ptr<TreeState>& state)>;

        Node(ProcessFunction process): _process(move(process)) {}

        BTNodeStatus process(const std::shared_ptr<Sprite>& context, std::shared_ptr<TreeState> state) const
        {
            return _process(context, *this, state);
        }

        size_t child_count() const {
            return _child_count;
        }

        size_t descendant_count() const {
            // Only calculate on the first call
            if (_descendant_count == 0 && _child_count > 0) {
                _descendant_count = _child_count;
                auto *child = first_child();
                for (size_t i = 0; i < _child_count; ++i) {
                    _descendant_count += child->descendant_count();
                    child = child->next_sibling();
                }
            }
            return _descendant_count;
        }

        void add_child();

        Node const *first_child() const {
            if (_child_count == 0) {
                return nullptr;
            }
            // Tree nodes are stored contiguously in depth-first order.
            // Therefore, first child is always current pointer plus 1.
            return this + 1;
        }

        Node const *next_sibling() const {
            // Tree nodes are stored contiguously in depth-first order.
            return this + descendant_count() + 1;
        }

        /*!
            \brief Returns this node's index in its tree.
        */
        size_t index() const {
            return _index;
        }

        /*!
            \brief Updates the given tree state so that the tree can resume at this (composite) node with the child generator starting at the given child index.
        */
        void save_state_at_child_index(std::shared_ptr<TreeState>& state, size_t child_index) const {
            state->_resume_index = index();
            assert(child_index < child_count());
            state->_offset = child_index;
        }

        /*!
            \brief Clears the given tree state so that subsequent process() calls do not resume.
        */
        void clear_state(std::shared_ptr<TreeState>& state) const {
            state->_resume_index = 0;
            state->_offset = 0;
        }

    private:
        friend class BehaviourTree;

        size_t _index{};
        size_t _child_count{};
        mutable size_t _descendant_count{};
        ProcessFunction _process;
    };

    using Generator = std::function<Node const *()>;

/*!
 \brief Composites define how to run the process() function on the child range.

   The generator function returns the next child in the child array or nullptr after the
   end of the child array. If the previous call to process() returned RUNNING status,
   the first result of the generator will be the same child as was returned when the
   previous called returned the RUNNING status. This allows composites to resume
   where they left off.

   The child pointer returned is only valid within the scope of the composite function
   body.
*/
    using Composite = std::function<BTNodeStatus(const std::shared_ptr<Sprite>&, Generator const &, std::shared_ptr<TreeState>&)>;

/*!
 \brief Composite that returns success if all children return success.
*/
    BTNodeStatus sequence(const std::shared_ptr<Sprite>& context, Generator const &next_child, std::shared_ptr<TreeState>& state);

/*!
 \brief Composite that returns success on the first successful call.
*/
    BTNodeStatus selector(const std::shared_ptr<Sprite>& context, Generator const &next_child, std::shared_ptr<TreeState>& state);

/*!
 \brief A decorator is a composite that may only have a single child.
*/
    using Decorator = std::function<BTNodeStatus(const std::shared_ptr<Sprite>& context, Node const &child, std::shared_ptr<TreeState>& state)>;

/*!
 \brief Decorator that just returns the result of the child. Not very useful...
*/
    BTNodeStatus forwarder(const std::shared_ptr<Sprite>& context, Node const &child, std::shared_ptr<TreeState>& state);

/*!
 \brief Decorator that inverts the result of its child node.
*/
    BTNodeStatus inverter(const std::shared_ptr<Sprite>& context, Node const &child, std::shared_ptr<TreeState>& state);

/*!
 \brief Decorator that returns success regardless of the child result.
*/
    BTNodeStatus succeeder(const std::shared_ptr<Sprite>& context, Node const &child, std::shared_ptr<TreeState>& state);

    using Leaf = std::function<BTNodeStatus(const std::shared_ptr<Sprite>& context)>; //!< A Leaf function takes a Context & and returns a Status.

    using BoolLeaf = std::function<bool(const std::shared_ptr<Sprite>& context)>;//BasicLeaf<bool, std::shared_ptr<SpriteContext>>; //!< A Leaf function returning bool returns SUCCESS on true and FAILURE on false. It is not possible to return RUNNING from such a function.

    using VoidLeaf = std::function<void(const std::shared_ptr<Sprite>& context)>;//BasicLeaf<void, std::shared_ptr<SpriteContext>>; //!< A Leaf function returning anything other than bool or Status can be added using #beehive::BuilderBase::void_leaf. The return value is ignored and SUCCESS is returned.

/*!
 \brief A leaf that always succeeds. Not very useful...
*/
    BTNodeStatus noop(const std::shared_ptr<Sprite>&);

/*!
 \brief The behavior tree class which passes the ContextType around. See #beehive::Builder for making one.
*/
    class BehaviourTree
    {
    public:
        /*!
         \brief Process with the given context reference.
        */
        //Status process(std::shared_ptr<Sprite> context) const;

        /*!
         \brief Process with the given state and context reference.
        */
        BTNodeStatus process(std::shared_ptr<TreeState>& state, const std::shared_ptr<Sprite>& context) const;

        /*!
         \brief Retrieves the nodes, for debugging purposes.
        */
        std::vector<Node> const &nodes() const {
            return _nodes;
        }

        /*!
         \brief Creates a state object that can be passed to subsequent process() calls.
        */
        //TreeState make_state() const {
        //    return {shared_from_this()};
        //}

    private:
        static size_t id() {
            static size_t id{};
            return ++id;
        }

        friend class BuilderBase;
        friend class Builder;

        /*!
         \brief Constructs a tree with the given nodes.
            See #beehive::Builder.
        */
        BehaviourTree(std::vector<Node> nodes);

        std::vector<Node> _nodes;
        size_t _id{id()};

        // redeclared -> Status process(std::shared_ptr<SpriteContext> context) const;
    };

    /// @cond

/// @endcond

    class Builder;

/*!
 \brief A helper for building trees which can be instantiated as #beehive::Builder.
*/
    class BuilderBase
    {
    public:
        /// @cond
        enum class Type
        {
            COMPOSITE,
            DECORATOR,
        };
        /// @endcond

        /*!
         \brief Adds the given composite to the tree. Composites have one or more children.

         \note The composite builder must call end() to signify end of child list.
        */
        BuilderBase composite(Composite composite);

        /*!
         \brief Adds the given decorator to the tree. Decorators have exactly one child.

         \note The decorator builder must call end() to signify the end of the child list.
        */
        BuilderBase decorator(Decorator decorator);

        // Note: "no known conversion" warnings here could indicate that you forgot to return something from your lambda.
        /*!
         \brief Adds the given leaf to the tree. Leaves have no children.
        */
        BuilderBase &leaf(Leaf leaf);

        /*!
         \brief Convenience wrapper so that bool functions can be used. Translates true
            result to Status::SUCCESS, false to Status::FAILURE and never returns Status:RUNNING.
        */
        BuilderBase &leaf(BoolLeaf leaf);

        /*!
         \brief Convenience wrapper for a void function, or really a function returning any type other than bool or Status. This always returns Status::SUCCESS.
        */
        BuilderBase &void_leaf(VoidLeaf leaf);

        /*!
         \brief Copies another tree as a subtree at the current node.
        */
        BuilderBase &tree(BehaviourTree const &subtree);

        /*!
         \brief Closes the composite or decorator branch.

            Each call to composite() or decorator() must have a corresponding end().
        */
        BuilderBase &end();

        /*!
         \brief Finalizes the tree by returning a copy. This will assert if done while
            a decorator or composite branch is still 'open'.
        */
        virtual BehaviourTree build() const &;

        /*!
         \brief Finalizes the tree by returning a tree constructed with the builder's
            root node. The builder is then invalid.
        */
        virtual BehaviourTree build() &&;

        /*!
         \brief Shorthand for `composite(&sequence<C>)`.
        */
        BuilderBase sequence();

        /*!
         \brief Shorthand for `composite(&selector<C>)`.
        */
        BuilderBase selector();

        /*!
         \brief Shorthand for `decorator(&inverter<C>)`.
        */
        BuilderBase inverter();

        /*!
         \brief Shorthand for `decorator(&succeeder<C>)`.
        */
        BuilderBase succeeder();

        static Node::ProcessFunction make_branch(Decorator f);
        static Node::ProcessFunction make_branch(Composite f);

        static Node::ProcessFunction make_leaf(Leaf f);
        static Node::ProcessFunction make_leaf(VoidLeaf f);
        static Node::ProcessFunction make_leaf(BoolLeaf f);

    protected:
        /// @cond
        BuilderBase(BuilderBase &parent, size_t offset, Type type)
                : _parent(parent)
                , _offset(offset)
                , _type(type)
        {}

        Node &node()
        {
            return nodes()[_offset];
        }

        virtual std::vector<Node> &nodes()
        {
            return _parent.nodes();
        }

    private:
        size_t add_child(typename Node::ProcessFunction &&fn)
        {
            node().add_child();
            nodes().emplace_back(std::move(fn));
            return nodes().size() - 1;
        }

        template<typename LeafType>
        BuilderBase &_leaf(LeafType &&leaf);

        template<typename BranchType>
        BuilderBase _branch(BranchType &&branch);

        BuilderBase &_parent;
        size_t _offset{};
        Type _type{};

        /// @endcond
    };

/*!
 \brief Defines the tree structure and instantiates it.

    This Builder pattern is inspired by arvidsson's implementation, BrainTree.
 \sa #beehive::BuilderBase
*/
    class Builder : public BuilderBase
    {
    public:
        /*!
         \brief Begins construction of a tree.
        */
        Builder() : BuilderBase(*this, 0, BuilderBase::Type::DECORATOR)
        {
            auto root = make_branch(Decorator(&forwarder));
            _nodes.emplace_back(std::move(root));
        }

        Builder(Builder const &) = delete; //!< Deleted copy constructor.
        Builder(Builder &&) = default; //!< Move constructor.
        Builder &operator=(Builder const &) = delete; //!< Deleted copy assignment operator.
        Builder &operator=(Builder &&) = default; //!< Move assignment operator.

        virtual BehaviourTree build() const & override
        {
            assert(_nodes[0].child_count() > 0); // must have at least one leaf node added
            return {_nodes};
        }

        virtual BehaviourTree build() && override
        {
            assert(_nodes[0].child_count() > 0); // must have at least one leaf node added
            return {std::move(_nodes)};
        }

    private:
        virtual std::vector<Node> &nodes() override
        {
            return _nodes;
        }

        std::vector<Node> _nodes;
    };

    const TreeState* getTreeStateFromTree(std::shared_ptr<BehaviourTree> tree);

} // namespace dang