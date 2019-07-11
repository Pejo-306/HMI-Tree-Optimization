#include "tree/cache_entries/hmi_widget_cache_entry.hh"

#include <string>
#include <sstream>

namespace hmi_tree_optimization {
    namespace tree {
        HMIWidgetCacheEntry::HMIWidgetCacheEntry() {
        }

        HMIWidgetCacheEntry::~HMIWidgetCacheEntry() noexcept {
        }

        std::string HMIWidgetCacheEntry::repr() const {
            std::ostringstream res;
            
            res << "HMIWidgetCacheEntry{"
                << "}";
            return res.str();
        }
    }  // namespace tree 
}  // namespace hmi_tree_optimization

