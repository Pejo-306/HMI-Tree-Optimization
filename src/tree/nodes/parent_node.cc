#include "tree/nodes/parent_node.hh"

#include <unordered_set>

namespace hmi_tree_optimization {
    namespace tree {
        ParentNode::ParentNode(nid_t id) noexcept
            : Node(id) {
        }

        ParentNode::ParentNode(nid_t id, 
                const std::unordered_set<const ParentNode *>& parents) noexcept
            : Node(id, parents) {
        }

        ParentNode& ParentNode::add_child(Node *child_node) {
            children_.insert(child_node);
            if (!child_node->has_parent(this))
                child_node->add_parent(this);
            return *this;
        }

        ParentNode& ParentNode::add_child(Node& child_node) {
            return add_child(&child_node);
        }

        const std::unordered_set<Node *>& ParentNode::get_children() const noexcept {
            return children_;
        }

        bool ParentNode::has_child(const Node *child_node) const noexcept {
            return children_.find(const_cast<Node *>(child_node)) != children_.end();
        }

        bool ParentNode::has_child(const Node& child_node) const noexcept {
            return has_child(&child_node);
        }

        std::string ParentNode::get_typename() const noexcept {
            return "ParentNode";
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

