#include "tree/cache_entry.hh"

#include <unordered_map>
#include <string>

namespace hmi_tree_optimization {
    namespace tree {
        const std::unordered_map<std::string, void *>& 
                CacheEntry::get_attributes() const noexcept {
            return attributes_;
        }

        CacheEntry::~CacheEntry() noexcept {
        }

        std::unordered_map<std::string, void *>& CacheEntry::get_attributes() noexcept {
            return attributes_;
        }
    }  // namespace tree 
}  // namespace hmi_tree_optimization

