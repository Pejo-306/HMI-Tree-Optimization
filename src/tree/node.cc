/*!
 * \file src/tree/node.cc
 * \brief Implementation of HMI tree node structure
 *
 * \see include/tree/node.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/node.hh"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <thread>
#include <chrono>

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        constexpr long Node::render_time;  /*!< Average (simulated) time for rendering. */

        /*!
         * \brief Construct HMI tree node
         */
        Node::Node(nid_t id) noexcept 
            : id_(id),
              dirty_(false),
              very_dirty_(false) {
        }

        /*!
         * \brief Destroy HMI tree node
         *
         * This destructor removes all children nodes and call their destructors.
         * Therefore, calling the destructor of a node recursively destroys
         * all of its direct and indirect children, i.e. it removes a whole
         * branch of the HMI tree.
         */
        Node::~Node() noexcept {
            const std::unordered_set<Node *> children_copy(children_);

            for (auto& ch : children_copy) {
                remove_child(ch);
                if (ch->get_parents().size() == 0)  // only destroy child if it has no other parents
                    delete ch;
            }
        }

        /*!
         * \brief Get the node's assigned id
         */
        nid_t Node::get_id() const noexcept {
            return id_;
        }

        /*!
         * \brief Access the node's associated child nodes
         */
        std::unordered_set<Node *>& Node::get_children() noexcept {
            return children_;
        }

        /*!
         * \brief Get the node's associated child nodes
         */
        const std::unordered_set<Node *>& Node::get_children() const noexcept {
            return children_;
        }

        /*!
         * \brief Associate another node as a child of this one
         *
         * The provided node is inserted into this node's children set. The
         * opposite relation (i.e. associating this node as the given's parent)
         * is automatically accomplished if it hasn't been done already.
         *
         * \see Node::add_parent
         * \see Node::has_parent
         */
        Node& Node::add_child(Node *child_node) {
            children_.insert(child_node);
            if (!child_node->has_parent(this))
                child_node->add_parent(this);
            return *this;
        }

        /*!
         * \brief Associate another node as a child of this one
         */
        Node& Node::add_child(Node& child_node) {
            return add_child(&child_node);
        }

        /*!
         * \brief Remove the parent-child association between this node and the given child
         *
         * The provided node is removed from this node's children set. The
         * opposite relation removal (i.e. disbanding this node from the given's 
         * parents set) is automatically accomplished if it hasn't been done already.
         *
         * \see Node::remove_parent
         * \see Node::has_parent
         */
        Node& Node::remove_child(Node *child_node) {
            children_.erase(child_node);
            if (child_node->has_parent(this))
                child_node->remove_parent(this);
            return *this;
        }

        /*!
         * \brief Remove the parent-child association between this node and the given child
         */
        Node& Node::remove_child(Node& child_node) {
            return remove_child(&child_node);
        }

        /*!
         * \brief Check if a specified node is this node's direct child
         *
         * Returns true only if the given node is in this node's children set.
         */
        bool Node::has_child(const Node *child_node) const noexcept {
            return children_.find(const_cast<Node *>(child_node)) != children_.end();
        }

        /*!
         * \brief Check if a specified node is this node's direct child
         */
        bool Node::has_child(const Node& child_node) const noexcept {
            return has_child(&child_node);
        }

        /*!
         * \brief Access the node's associated parent nodes
         */
        std::unordered_set<const Node *>& Node::get_parents() noexcept {
            return parents_;
        }

        /*!
         * \brief Get the node's associated parent nodes
         */
        const std::unordered_set<const Node *>& Node::get_parents() const noexcept {
            return parents_;
        }

        /*!
         * \brief Associate another node as a parent of this one
         * 
         * The provided node is inserted into this node's parents set. The
         * opposite relation (i.e. associating this node as the given's child)
         * is automatically accomplished if it hasn't been done already.
         *
         * \see Node::add_child
         * \see Node::has_child
         */
        Node& Node::add_parent(Node *parent_node) {
            parents_.insert(parent_node);
            if (!parent_node->has_child(this))
                parent_node->add_child(this);
            return *this;
        }

        /*!
         * \brief Associate another node as a parent of this one
         */
        Node& Node::add_parent(Node& parent_node) {
            return add_parent(&parent_node);
        }

        /*!
         * \brief Remove the child-parent association between this node and the given parent 
         *
         * The provided node is removed from this node's parents set. The
         * opposite relation removal (i.e. disbanding this node from the given's 
         * children set) is automatically accomplished if it hasn't been done already.
         *
         * \see Node::remove_child
         * \see Node::has_child
         */
        Node& Node::remove_parent(Node *parent_node) {
            parents_.erase(parent_node);
            if (parent_node->has_child(this))
                parent_node->remove_child(this);
            return *this;
        }

        /*!
         * \brief Remove the child-parent association between this node and the given parent 
         */
        Node& Node::remove_parent(Node& parent_node) {
            return remove_parent(&parent_node);   
        }

        /*!
         * \brief Check if a specified node is this node's direct parent
         *
         * Returns true only if the given node is in this one's parents set.
         */
        bool Node::has_parent(const Node *parent_node) const noexcept {
            return parents_.find(parent_node) != parents_.end();
        }

        /*!
         * \brief Check if a specified node is this node's direct parent
         */
        bool Node::has_parent(const Node& parent_node) const noexcept {
            return has_parent(&parent_node);
        }

        /*!
         * \brief Check if node is dirty
         *
         * BLACK MAGIC
         */
        bool Node::is_dirty() const noexcept {
            // ... BLACK MAGIC ...
            return dirty_;
        }

        /*!
         * \brief Check if node is very dirty
         *
         * A very dirty node must fulfill one of the two following conditions:
         * - be manually marked as very dirty from the outside;
         * - have a very dirty child (direct or indirect).
         */
        bool Node::is_very_dirty() const {
            if (std::any_of(children_.begin(), children_.end(), [](Node *child) {
                        return child->is_very_dirty();
                        })) {
                return true;
            }
            return very_dirty_;
        }

        /*!
         * \brief Mark a node as very dirty
         */
        Node& Node::mark_as_very_dirty() noexcept {
            very_dirty_ = true;
            return *this;
        }

        /*!
         * \brief Check if node is very clean
         */
        bool Node::is_very_clean() const {
            return !is_very_dirty();
        }

        /*!
         * \brief Mark a node as very clean
         */
        Node& Node::mark_as_very_clean() noexcept {
            very_dirty_ = false;
            return *this;
        }

        /*!
         * \brief Restore dirtiness state after a frame render
         */
        Node& Node::clean_up() noexcept {
            dirty_ = false;
            return *this;
        }

        /*!
         * \brief Update this node
         *
         * After applying the requested update, the node is marked as dirty.
         *
         * \see Node::apply_update
         */
        Node& Node::update(const std::vector<std::string>& parameters) {
            apply_update(parameters);
            dirty_ = true;
            return *this;
        }

        /*!
         * \brief Simulate time-consuming work when rendering a node
         *
         * The time consuming work is simulated by sleeping the current thread for
         * some arbitrary time.
         */
        Node& Node::render(bool debug, std::ostream& out) noexcept {
            if (debug)
                out << "Rendering node " << id_ << "..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(render_time));
            return *this;
        }

        /*!
         * \brief Cache this node and return an entry to be stored
         *
         * \details The CacheEntry instance is to be dynamically allocated. <!--
         * --> The caller is responsible for freeing the allocated memory.
         *
         * \see CacheEntry
         * \see Node::generate_cache_entry
         * \see Node::load_from_cache
         */
        CacheEntry *Node::cache(bool debug, std::ostream& out) {
            if (debug)
                out << "Caching node " << id_ << "..." << std::endl;
            return generate_cache_entry();
        }

        /*!
         * \brief Use a cache entry to restore a node's previous state
         *
         * \see CacheEntry
         * \see Node::use_cache_entry
         * \see Node::cache
         */
        Node& Node::load_from_cache(const CacheEntry *entry, bool debug, std::ostream& out) {
            if (debug)
                out << "Loading node " << id_ << " from cache..." << std::endl;
            use_cache_entry(entry);
            return *this;
        }

        /*!
         * \brief Count the number of all direct and indirect children
         *
         * Recursively adds the result of each child's analogous method.
         */
        size_t Node::nall_children() const noexcept {
            size_t count = children_.size();
            
            for (const auto& child : children_)
                count += child->nall_children();
            return count;
        }

        /*!
         * \brief Count the number of all direct and indirect parents
         *
         * Recursively adds the result of each child's analogous method.
         */
        size_t Node::nall_parents() const noexcept {
            size_t count = parents_.size();

            for (const auto& parent : parents_)
                count += parent->nall_parents();
            return count;
        }

        /*!
         * \brief Print a node to an output stream
         *
         * \see hmi_tree_optimization::tree::Node
         * \see hmi_tree_optimization::tree::Node::to_string
         */
        std::ostream& operator<<(std::ostream& out, const Node& node) {
            out << node.to_string();
            return out;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

