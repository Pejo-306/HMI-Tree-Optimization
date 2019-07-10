#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_CACHE_ENTRY_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_CACHE_ENTRY_HH

#include <string>

#include "tree/cache_entry.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMITextCacheEntry final: public CacheEntry {
        public:
            explicit HMITextCacheEntry(const std::string&);
            ~HMITextCacheEntry() noexcept;
        private:
            void must_implement() const noexcept final;
        };  // class HMITextCacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

