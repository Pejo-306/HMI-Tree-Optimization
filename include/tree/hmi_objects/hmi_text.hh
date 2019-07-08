#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH

#include <string>

#include "tree/nodes/idirty_node.hh"
#include "tree/nodes/ichild_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIText final: public IDirtyNode, public IChildNode {
        public:
            explicit HMIText(nid_t) noexcept;
            virtual std::string to_string() const noexcept final;
        };  // class HMIText
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

