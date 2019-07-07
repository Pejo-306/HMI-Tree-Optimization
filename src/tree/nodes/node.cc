#include "tree/nodes/node.hh"

#include <iostream>

namespace hmi_tree_optimization {
    namespace tree {
        Node::Node(nid_t id) noexcept 
            : id_(id) {
        }

        Node::~Node() noexcept {
        }

        nid_t Node::get_id() const noexcept {
            return id_;
        }

        std::ostream& operator<<(std::ostream& out, const Node& node) {
            out << node.to_string();
            return out;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

