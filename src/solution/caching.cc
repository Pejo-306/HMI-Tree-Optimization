#include "solution/caching.hh"

#include <stack>

#include "tree/hmi_tree.hh"
#include "tree/node.hh"
#include "tree/cache_entry.hh"

using namespace hmi_tree_optimization::tree;

namespace hmi_tree_optimization {
    namespace solution {
        std::unordered_map<tree::nid_t, CacheEntry *> g_cache_table;

        void evaluate_tree_dirtiness(tree::HMITree& tree) {
            Node *node;
            std::stack<Node *> nodes;

            // reverse the order of the tree:
            // popping from the stack will begin at the tree's leaves and
            // end at the tree's root
            for (HMITree::bfs_iterator it = tree.bfs_begin(); it != tree.bfs_end(); ++it)
                nodes.push(it.address());

            while (!nodes.empty()) {
                node = nodes.top();
                nodes.pop();
                // TODO evaluate the node here via its dirty counter
                if (rand() % 101 <= 30)  // FIXME
                    node->mark_as_very_dirty();
                else
                    node->mark_as_very_clean();
            }
        }

        void refresh_screen(tree::HMITree& tree) {

        }

        void clear_cache() noexcept {
            for (auto& entry : g_cache_table)
                delete entry.second;  // clear the cache
        }
    }  // namespace solution
}  // namespace hmi_tree_optimization

