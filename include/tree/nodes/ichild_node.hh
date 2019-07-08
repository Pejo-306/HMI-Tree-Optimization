#ifndef HMI_TREE_OPTIMIZATION__TREE__ICHILD_NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__ICHILD_NODE_HH

#include <unordered_set>

#include "tree/nodes/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class IParentNode;

        class IChildNode: public virtual Node {
        public:
            explicit IChildNode(nid_t) noexcept;
            IChildNode& add_parent(IParentNode *);
            IChildNode& add_parent(IParentNode&);
            IChildNode& remove_parent(IParentNode *);
            IChildNode& remove_parent(IParentNode&);
            const std::unordered_set<const IParentNode *>& get_parents() const noexcept;
            bool has_parent(const IParentNode *) const noexcept;
            bool has_parent(const IParentNode&) const noexcept;
        private:
            std::unordered_set<const IParentNode *> parents_;
        };  // class IChildNode
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

