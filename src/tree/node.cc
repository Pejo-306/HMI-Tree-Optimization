#include "tree/node.hh"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <thread>
#include <chrono>

namespace hmi_tree_optimization {
    namespace tree {
        constexpr long Node::render_time;

        Node::Node(nid_t id) noexcept 
            : id_(id),
              dirty_counter_(0),
              dirty_(false),
              very_dirty_(false) {
        }

        Node::~Node() noexcept {
            const std::unordered_set<Node *> children_copy(children_);

            for (auto& ch : children_copy) {
                remove_child(ch);
                if (ch->get_parents().size() == 0)
                    delete ch;
            }
        }

        nid_t Node::get_id() const noexcept {
            return id_;
        }

        std::unordered_set<Node *>& Node::get_children() noexcept {
            return children_;
        }

        const std::unordered_set<Node *>& Node::get_children() const noexcept {
            return children_;
        }

        Node& Node::add_child(Node *child_node) {
            children_.insert(child_node);
            if (!child_node->has_parent(this))
                child_node->add_parent(this);
            return *this;
        }

        Node& Node::add_child(Node& child_node) {
            return add_child(&child_node);
        }

        Node& Node::remove_child(Node *child_node) {
            children_.erase(child_node);
            if (child_node->has_parent(this))
                child_node->remove_parent(this);
            return *this;
        }

        Node& Node::remove_child(Node& child_node) {
            return remove_child(&child_node);
        }

        bool Node::has_child(const Node *child_node) const noexcept {
            return children_.find(const_cast<Node *>(child_node)) != children_.end();
        }

        bool Node::has_child(const Node& child_node) const noexcept {
            return has_child(&child_node);
        }

        std::unordered_set<const Node *>& Node::get_parents() noexcept {
            return parents_;
        }

        const std::unordered_set<const Node *>& Node::get_parents() const noexcept {
            return parents_;
        }

        Node& Node::add_parent(Node *parent_node) {
            parents_.insert(parent_node);
            if (!parent_node->has_child(this))
                parent_node->add_child(this);
            return *this;
        }

        Node& Node::add_parent(Node& parent_node) {
            return add_parent(&parent_node);
        }

        Node& Node::remove_parent(Node *parent_node) {
            parents_.erase(parent_node);
            if (parent_node->has_child(this))
                parent_node->remove_child(this);
            return *this;
        }

        Node& Node::remove_parent(Node& parent_node) {
            return remove_parent(&parent_node);   
        }

        bool Node::has_parent(const Node *parent_node) const noexcept {
            return parents_.find(parent_node) != parents_.end();
        }

        bool Node::has_parent(const Node& parent_node) const noexcept {
            return has_parent(&parent_node);
        }

        int Node::get_dirty_counter() const noexcept {
            return dirty_counter_;
        }

        bool Node::is_dirty() const noexcept {
            // ... BLACK MAGIC ...
            return dirty_;
        }

        bool Node::is_very_dirty() const {
            if (std::any_of(children_.begin(), children_.end(), [](Node *child) {
                        return child->is_very_dirty();
                        })) {
                return true;
            }
            return very_dirty_;
        }

        Node& Node::mark_as_very_dirty() noexcept {
            very_dirty_ = true;
            return *this;
        }

        bool Node::is_very_clean() const {
            return !is_very_dirty();
        }

        Node& Node::mark_as_very_clean() noexcept {
            very_dirty_ = false;
            return *this;
        }

        Node& Node::clean_up() noexcept {
            dirty_ = false;
            return *this;
        }

        Node& Node::update(const std::vector<std::string>& parameters) {
            apply_update(parameters);
            if (!dirty_) {
                ++dirty_counter_;
                dirty_ = true;
            }
            return *this;
        }

        Node& Node::render(bool debug, std::ostream& out) noexcept {
            // Simulate some rendering work
            if (debug)
                out << "Rendering node " << id_ << "..." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(render_time));
            return *this;
        }

        CacheEntry *Node::cache(bool debug, std::ostream& out) {
            if (debug)
                out << "Caching node " << id_ << "..." << std::endl;
            return generate_cache_entry();
        }

        Node& Node::load_from_cache(const CacheEntry *entry, bool debug, std::ostream& out) {
            if (debug)
                out << "Loading node " << id_ << " from cache..." << std::endl;
            use_cache_entry(entry);
            return *this;
        }

        size_t Node::nall_children() const noexcept {
            size_t count = children_.size();
            
            for (const auto& child : children_)
                count += child->nall_children();
            return count;
        }

        size_t Node::nall_parents() const noexcept {
            size_t count = parents_.size();

            for (const auto& parent : parents_)
                count += parent->nall_parents();
            return count;
        }

        std::ostream& operator<<(std::ostream& out, const Node& node) {
            out << node.to_string();
            return out;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

