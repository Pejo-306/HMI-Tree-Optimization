#include "tree/node.hh"

#include <iostream>
#include <unordered_set>

namespace hmi_tree_optimization {
    namespace tree {
        Node::Node(nid_t id) noexcept 
            : id_(id),
              dirty_counter_(0) {
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

        std::ostream& operator<<(std::ostream& out, const Node& node) {
            out << node.to_string();
            return out;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

