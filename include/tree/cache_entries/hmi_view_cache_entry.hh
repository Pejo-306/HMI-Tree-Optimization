#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_CACHE_ENTRY_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_CACHE_ENTRY_HH

#include "tree/cache_entry.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIViewCacheEntry final: public CacheEntry {
        public:
            explicit HMIViewCacheEntry();
            ~HMIViewCacheEntry() noexcept;
        private:
            void must_implement() const noexcept final;
        };  // class HMIViewCacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

