#ifndef HMI_TREE_OPTIMIZATION__TREE__IPARENT_NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__IPARENT_NODE_HH

#include <unordered_set>

#include "tree/nodes/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class IChildNode;

        class IParentNode: public virtual Node {
        public:
            explicit IParentNode(nid_t) noexcept;
            virtual ~IParentNode() noexcept;
            IParentNode& add_child(IChildNode *);
            IParentNode& add_child(IChildNode&);
            IParentNode& remove_child(IChildNode *);
            IParentNode& remove_child(IChildNode&);
            const std::unordered_set<IChildNode *>& get_children() const noexcept;
            bool has_child(const IChildNode *) const noexcept;
            bool has_child(const IChildNode&) const noexcept;
        private:
            std::unordered_set<IChildNode *> children_;
        };  // class IParentNode
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

