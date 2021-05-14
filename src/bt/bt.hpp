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
#include <vector>

namespace dang
{
    class SpriteContext
    {
    public:

        bool is_hungry{true};
        void eat_food() {};

        bool has_food() const
        {
            return _has_food;
        }

        bool _has_food{true};
    };

    class EnemiesAroundChecker
    {
    public:

        bool operator()(std::shared_ptr<SpriteContext>) const {
            return _enemies_around;
        }
        bool _enemies_around{false};
    };

/*!
 \brief The status returned by process functions.
*/
    enum class Status
    {
        FAILURE = 0, //!< Returned when the process function has failed.
        RUNNING, //!< Returned when the outcome of process has not been determined yet.
        SUCCESS //!< Returns when the process has succeeded.
    };

/*!
 \brief Pass a TreeState instance to #beehive::Tree's process function in order to resume Running nodes. Instantiate with #beehive::Tree::make_state.
*/
    struct TreeState {
        // For internal use only.
        size_t resume_index() const {
            return _resume_index;
        }
        // For internal use only.
        size_t offset() const {
            return _offset;
        }
    private:
        TreeState(size_t tree_id): _tree_id(tree_id) {}

        size_t _tree_id;
        size_t _resume_index{};
        size_t _offset{};

        friend class Tree;
        friend struct Node;
    };

/*!
 \brief A handle on a process function. This should not be built directly, see #beehive::Builder.
*/
    class Node
    {
    public:

        using ProcessFunction = std::function<Status(std::shared_ptr<SpriteContext> context, Node const &self, std::shared_ptr<TreeState> state)>;

        Node(ProcessFunction process): _process(move(process)) {}

        Status process(std::shared_ptr<SpriteContext> context, std::shared_ptr<TreeState> state) const
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
        void save_state_at_child_index(std::shared_ptr<TreeState> state, size_t child_index) const {
            state->_resume_index = index();
            assert(child_index < child_count());
            state->_offset = child_index;
        }

        /*!
            \brief Clears the given tree state so that subsequent process() calls do not resume.
        */
        void clear_state(std::shared_ptr<TreeState> state) const {
            state->_resume_index = 0;
            state->_offset = 0;
        }

    private:
        friend class Tree;

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
    using Composite = std::function<Status(std::shared_ptr<SpriteContext>, Generator const &, std::shared_ptr<TreeState>)>;

/*!
 \brief Composite that returns success if all children return success.
*/
    Status sequence(std::shared_ptr<SpriteContext> context, Generator const &next_child, std::shared_ptr<TreeState> state);

/*!
 \brief Composite that returns success on the first successful call.
*/
    Status selector(std::shared_ptr<SpriteContext> context, Generator const &next_child, std::shared_ptr<TreeState> state);

/*!
 \brief A decorator is a composite that may only have a single child.
*/
    using Decorator = std::function<Status(std::shared_ptr<SpriteContext> context, Node const &child, std::shared_ptr<TreeState> state)>;

/*!
 \brief Decorator that just returns the result of the child. Not very useful...
*/
    Status forwarder(std::shared_ptr<SpriteContext> context, Node const &child, std::shared_ptr<TreeState> state);

/*!
 \brief Decorator that inverts the result of its child node.
*/
    Status inverter(std::shared_ptr<SpriteContext> context, Node const &child, std::shared_ptr<TreeState> state);

/*!
 \brief Decorator that returns success regardless of the child result.
*/
    Status succeeder(std::shared_ptr<SpriteContext> context, Node const &child, std::shared_ptr<TreeState> state);

    using Leaf = std::function<Status(std::shared_ptr<SpriteContext> context)>; //!< A Leaf function takes a Context & and returns a Status.

    using BoolLeaf = std::function<bool(std::shared_ptr<SpriteContext> context)>;//BasicLeaf<bool, std::shared_ptr<SpriteContext>>; //!< A Leaf function returning bool returns SUCCESS on true and FAILURE on false. It is not possible to return RUNNING from such a function.

    using VoidLeaf = std::function<void(std::shared_ptr<SpriteContext> context)>;//BasicLeaf<void, std::shared_ptr<SpriteContext>>; //!< A Leaf function returning anything other than bool or Status can be added using #beehive::BuilderBase::void_leaf. The return value is ignored and SUCCESS is returned.

/*!
 \brief A leaf that always succeeds. Not very useful...
*/
    Status noop(std::shared_ptr<SpriteContext>);

/*!
 \brief The behavior tree class which passes the ContextType around. See #beehive::Builder for making one.
*/
    class Tree
    {
    public:
        /*!
         \brief Process with the given context reference.
        */
        Status process(std::shared_ptr<SpriteContext> context) const;

        /*!
         \brief Process with the given state and context reference.
        */
        Status process(std::shared_ptr<TreeState> state, std::shared_ptr<SpriteContext> context) const;

        /*!
         \brief Retrieves the nodes, for debugging purposes.
        */
        std::vector<Node> const &nodes() const {
            return _nodes;
        }

        /*!
         \brief Creates a state object that can be passed to subsequent process() calls.
        */
        TreeState make_state() const {
            return {_id};
        }

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
        Tree(std::vector<Node> nodes);

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
        BuilderBase &tree(Tree const &subtree);

        /*!
         \brief Closes the composite or decorator branch.

            Each call to composite() or decorator() must have a corresponding end().
        */
        BuilderBase &end();

        /*!
         \brief Finalizes the tree by returning a copy. This will assert if done while
            a decorator or composite branch is still 'open'.
        */
        virtual Tree build() const &;

        /*!
         \brief Finalizes the tree by returning a tree constructed with the builder's
            root node. The builder is then invalid.
        */
        virtual Tree build() &&;

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

        virtual Tree build() const & override
        {
            assert(_nodes[0].child_count() > 0); // must have at least one leaf node added
            return {_nodes};
        }

        virtual Tree build() && override
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

} // namespace dang