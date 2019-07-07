#include "tree/nodes/idirty_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        IDirtyNode::IDirtyNode(nid_t id) noexcept
            : Node(id),
              dirty_counter_(0) {
        }

        int IDirtyNode::get_dirty_counter() const noexcept {
            return dirty_counter_;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

