#ifndef HMI_TREE_OPTIMIZATION__SOLUTION__CACHING_HH
#define HMI_TREE_OPTIMIZATION__SOLUTION__CACHING_HH

#include <unordered_map>
#include <unordered_set>

#include "tree/hmi_tree.hh"
#include "tree/cache_entry.hh"

namespace hmi_tree_optimization {
    namespace solution {
        // map{node_id; cache entry}
        extern std::unordered_map<tree::nid_t, tree::CacheEntry *> g_cache_table;
        void evaluate_tree_dirtiness(tree::HMITree&, const std::unordered_set<tree::nid_t>&);
        void refresh_screen(tree::HMITree&);
        void clear_cache() noexcept;
    }  // namespace solution
}  // namespace hmi_tree_optimization

#endif

