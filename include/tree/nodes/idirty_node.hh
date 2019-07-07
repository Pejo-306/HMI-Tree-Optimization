#ifndef HMI_TREE_OPTIMIZATION__TREE__IDIRTY_NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__IDIRTY_NODE_HH

#include "tree/nodes/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class IDirtyNode: public virtual Node {
        public:
            explicit IDirtyNode(nid_t) noexcept;
            int get_dirty_counter() const noexcept;
        private:
            int dirty_counter_;
        };  // class IDirtyNode
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

