#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH

#include <string>

#include "tree/nodes/iparent_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIView final: public IParentNode {
        public:
            explicit HMIView(nid_t) noexcept;
            std::string to_string() const final;
            std::string repr() const final;
        };  // class HMIView
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

