#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_HH

#include <unordered_set>
#include <string>

#include "tree/node.hh"
#include "tree/cache_entry.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIWidget final: public Node {
        public:
            explicit HMIWidget(nid_t) noexcept;
            CacheEntry *cache() final;
            std::string to_string() const final;
            std::string repr() const final;
        private:
            void apply_update(const std::vector<std::string>&) final;
        };  // class HMIWidget
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

