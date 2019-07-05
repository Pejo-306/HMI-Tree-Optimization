#include "tree/node.hh"

#include <iostream>
#include <unordered_set>

namespace hmi_tree_optimization {
    namespace tree {
        Node::Node(nid_t id) noexcept 
            : id_(id),
              dirty_counter_(0) {
        }

        Node::Node(nid_t id, const std::unordered_set<const ParentNode *>& parents) noexcept 
            : id_(id),
              parents_(parents),
              dirty_counter_(0) {
        }

        Node::~Node() noexcept {
        }

        Node& Node::add_parent(const ParentNode *parent_node) {
            parents_.insert(parent_node);
            return *this;
        }

        Node& Node::add_parent(const ParentNode& parent_node) {
            return add_parent(&parent_node);
        }

        nid_t Node::get_id() const noexcept {
            return id_;
        }

        const std::unordered_set<const ParentNode *>& Node::get_parents() const noexcept {
            return parents_;
        }

        int Node::get_dirty_counter() const noexcept {
            return dirty_counter_;
        }

        std::string Node::get_typename() const noexcept {
            return "Node";
        }

        std::ostream& operator<<(std::ostream& out, const Node& node) {
            out << node.get_typename() << "{" 
                << "id=" << node.get_id()
                << ", dc=" << node.get_dirty_counter()
                << "}";
            return out;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

