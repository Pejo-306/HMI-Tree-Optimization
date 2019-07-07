#include "tree/nodes/ichild_node.hh"

#include "tree/nodes/node.hh"
#include "tree/nodes/iparent_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        IChildNode::IChildNode(nid_t id) noexcept
            : Node(id) {
        }

        IChildNode::IChildNode(nid_t id, 
                const std::unordered_set<const IParentNode *>& parents) noexcept
            : Node(id),
              parents_(parents) {
        }

        IChildNode& IChildNode::add_parent(IParentNode *parent_node) {
            parents_.insert(parent_node);
            if (!parent_node->has_child(this))
                parent_node->add_child(this);
            return *this;
        }

        IChildNode& IChildNode::add_parent(IParentNode& parent_node) {
            return add_parent(&parent_node);
        }

        const std::unordered_set<const IParentNode *>& IChildNode::get_parents() const noexcept {
            return parents_;
        }

        bool IChildNode::has_parent(const IParentNode *parent_node) const noexcept {
            return parents_.find(parent_node) != parents_.end();
        }

        bool IChildNode::has_parent(const IParentNode& parent_node) const noexcept {
            return has_parent(&parent_node);
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

