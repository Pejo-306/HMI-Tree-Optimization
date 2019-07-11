#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_CACHE_ENTRY_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_CACHE_ENTRY_HH

#include "tree/cache_entry.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIWidgetCacheEntry final: public CacheEntry {
        public:
            explicit HMIWidgetCacheEntry();
            ~HMIWidgetCacheEntry() noexcept;
            std::string repr() const final;
        };  // class HMIWidgetCacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

