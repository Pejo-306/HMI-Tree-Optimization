#include "tree/node.hh"

#include <iostream>
#include <unordered_set>
#include <string>
#include <sstream>

#include "tree/nodes/parent_node.hh"

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

        Node& Node::add_parent(ParentNode *parent_node) {
            parents_.insert(parent_node);
            if (!parent_node->has_child(this))
                parent_node->add_child(this);
            return *this;
        }

        Node& Node::add_parent(ParentNode& parent_node) {
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

        bool Node::has_parent(const ParentNode *parent_node) const noexcept {
            return parents_.find(parent_node) != parents_.end();
        }

        bool Node::has_parent(const ParentNode& parent_node) const noexcept {
            return has_parent(&parent_node);
        }

        std::string Node::to_string() const noexcept {
            std::ostringstream result;
            
            result << get_typename() << "{" 
                << "id=" << id_
                << ", dc=" << dirty_counter_
                << "}";
            return result.str();
        }

        std::string Node::get_typename() const noexcept {
            return "Node";
        }

        std::ostream& operator<<(std::ostream& out, const Node& node) {
            out << node.to_string();
            return out;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

