#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_HH

#include <unordered_set>
#include <string>

#include "tree/nodes/idirty_node.hh"
#include "tree/nodes/iparent_node.hh"
#include "tree/nodes/ichild_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIWidget final: public IDirtyNode, public IParentNode, public IChildNode {
        public:
            explicit HMIWidget(nid_t) noexcept;
            virtual std::string to_string() const noexcept final;
        };  // class HMIWidget
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

