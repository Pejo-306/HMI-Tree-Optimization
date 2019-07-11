#include "tree/cache_entries/hmi_view_cache_entry.hh"

#include <string>
#include <sstream>

namespace hmi_tree_optimization {
    namespace tree {
        HMIViewCacheEntry::HMIViewCacheEntry() {
        }

        HMIViewCacheEntry::~HMIViewCacheEntry() noexcept {
        }

        std::string HMIViewCacheEntry::repr() const {
            std::ostringstream res;
            
            res << "HMIViewCacheEntry{"
                << "}";
            return res.str();
        }
    }  // namespace tree 
}  // namespace hmi_tree_optimization

