#include "solution/caching.hh"

#include <algorithm>
#include <stack>
#include <queue>

#include "solution/config.hh"
#include "tree/hmi_tree.hh"
#include "tree/node.hh"
#include "tree/cache_entry.hh"

using namespace hmi_tree_optimization::tree;

namespace hmi_tree_optimization {
    namespace solution {
        namespace {
            auto cmp = [](Node *child) {
                return child->is_dirty();
            };
        }  // anonymous namespace

        std::unordered_map<tree::nid_t, CacheEntry *> g_cache_table;

        void evaluate_tree_dirtiness(tree::HMITree& tree) {
            Node *node;
            std::stack<Node *> nodes;

            // Reverse the order of the tree:
            // popping from the stack will begin at the tree's leaves and
            // end at the tree's root.
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
                node->clean_up();
            }
        }

        void refresh_screen(tree::HMITree& tree) {
            Node *node;
            std::queue<Node *> nodes;
            nid_t node_id;

            // By utilizing DFS a parent node's children will immediately be
            // positioned after their shared parent node. This way, the 
            // appropriate amount of children nodes may be ignored and popped
            // from the queue when deciding which parent node to cache.
            for (HMITree::dfs_iterator it = tree.dfs_begin(); it != tree.dfs_end(); ++it)
                nodes.push(it.address());

            while (!nodes.empty()) {
                node = nodes.front();
                nodes.pop();
                node_id = node->get_id();
                if (node->is_very_clean()) {
                    const std::unordered_set<Node *>& node_children = node->get_children();

                    if (g_cache_table.find(node_id) == g_cache_table.end()) {
                        node->render(g_debug);
                        g_cache_table.insert(std::make_pair(node_id, node->cache(g_debug)));
                    } else if (std::any_of(node_children.begin(), node_children.end(), cmp)) {
                        node->render(g_debug);
                        delete g_cache_table.at(node_id);
                        g_cache_table.insert(std::make_pair(node_id, node->cache(g_debug)));
                    } else {
                        node->load_from_cache(g_cache_table.at(node_id), g_debug);
                    }

                    // ignore any children of a cached parent node
                    for (size_t i = 0; i < node->nall_children(); ++i)
                        nodes.pop();
                } else {  // if (node->is_very_dirty())
                    auto cache_entry_it = g_cache_table.find(node_id);

                    if (cache_entry_it != g_cache_table.end()) {
                        delete g_cache_table.at(node_id);
                        g_cache_table.erase(cache_entry_it);
                    }
                    node->render(g_debug);
                }
            }
        }

        void clear_cache() noexcept {
            for (auto& entry : g_cache_table)
                delete entry.second;  // clear the cache
        }
    }  // namespace solution
}  // namespace hmi_tree_optimization

