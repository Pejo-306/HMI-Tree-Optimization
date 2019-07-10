#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH

#include <string>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIView final: public Node {
        public:
            explicit HMIView(nid_t) noexcept;
            bool is_very_dirty() const final;
            std::string to_string() const final;
            std::string repr() const final;
        private:
            void apply_update(const std::vector<std::string>&) final;
        };  // class HMIView
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

