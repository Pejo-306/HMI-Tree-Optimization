/*!
 * \file src/solution/caching.cc
 * \brief Implementation of tree dirtiness evaluation and caching routines.
 *
 * FIXME: these routines are implemented in a very stupid way, using tree
 * iterators. Fix it pls.
 *
 * \see include/solution/caching.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "solution/caching.hh"

#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_set>

#include "solution/config.hh"
#include "tree/hmi_tree.hh"
#include "tree/node.hh"
#include "tree/cache_entry.hh"

using namespace hmi_tree_optimization::tree;

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace solution 
    //! \brief Solution to the presented optimization problem
    namespace solution {
        namespace {
            auto cmp = [](Node *child) {
                return child->is_dirty();
            };
        }  // anonymous namespace

        std::unordered_map<tree::nid_t, CacheEntry *> g_cache_table;

        /*!
         * \brief Evaluate the dirtiness of each tree node.
         *
         * Essentially a node is marked as very dirty only if it is considered
         * a heavy hitter (i.e. a frequently updated element). All other nodes
         * are marked as very clean.
         */
        void evaluate_tree_dirtiness(tree::HMITree& tree, 
                const std::unordered_set<nid_t>& heavy_hitters) {
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
                if (heavy_hitters.find(node->get_id()) != heavy_hitters.end())
                    // node is a heavy hitter - mark as not fit for caching
                    node->mark_as_very_dirty();
                else
                    node->mark_as_very_clean();
                node->clean_up();
            }
        }

        /*!
         * \brief Refresh the screen.
         *
         * The refreshing process traverses the tree using DFS. By utilizing 
         * this traversal algorithm a parent node's children will immediately be 
         * positioned after their shared parent node within a queue data 
         * structure. This way, the  appropriate amount of children nodes may 
         * be ignored and popped from the queue when deciding which parent node
         * to cache.
         *
         * If a node is marked as very clean, one of two scenarios occur:
         * - should the node be updated, it is firstly rendered again, cached
         *   into an appropriate cache entry and the cache table is updated 
         *   accordingly;
         * - if the node is not dirty, it is directly loaded from the cache
         *   table.
         * In both cases all of the node's children (direct and indirect) are
         * popped from the stack data structure as they do not need to be
         * examined further.
         * Otherwise, if a node is marked as very dirty, its cache entry is
         * deleted (if there is one) and it is rendered.
         */
        void refresh_screen(tree::HMITree& tree) {
            Node *node;
            std::queue<Node *> nodes;
            nid_t node_id;

            for (HMITree::dfs_iterator it = tree.dfs_begin(); it != tree.dfs_end(); ++it)
                nodes.push(it.address());

            while (!nodes.empty()) {
                node = nodes.front();
                nodes.pop();
                node_id = node->get_id();
                if (node->is_very_clean()) {
                    const std::unordered_set<Node *>& node_children = node->get_children();

                    if (g_cache_table.find(node_id) == g_cache_table.end()) {
                        // node has not been cached so far
                        node->render(g_debug);
                        g_cache_table.insert(std::make_pair(node_id, node->cache(g_debug)));
                    } else if (std::any_of(node_children.begin(), node_children.end(), cmp)) {
                        // node has been cached, but needs to be updated
                        node->render(g_debug);
                        delete g_cache_table.at(node_id);
                        g_cache_table.insert(std::make_pair(node_id, node->cache(g_debug)));
                    } else {
                        // an appropriate cache entry is loaded from the cache
                        // table, and the node's state is restored
                        node->load_from_cache(g_cache_table.at(node_id), g_debug);
                    }

                    // ignore any children of a cached parent node
                    for (size_t i = 0; i < node->nall_children(); ++i) {
                        auto cache_entry_it = g_cache_table.find(nodes.front()->get_id());

                        if (cache_entry_it != g_cache_table.end()) {
                            // delete unnecessary cache entry
                            delete g_cache_table.at(nodes.front()->get_id());
                            g_cache_table.erase(cache_entry_it);
                        }
                        nodes.pop();
                    }
                } else {  // if (node->is_very_dirty())
                    auto cache_entry_it = g_cache_table.find(node_id);

                    if (cache_entry_it != g_cache_table.end()) {
                        // delete unnecessary cache entry
                        delete g_cache_table.at(node_id);
                        g_cache_table.erase(cache_entry_it);
                    }
                    node->render(g_debug);
                }
            }
        }

        /*!
         * \brief Clear the cache table.
         *
         * Frees the dynamically allocated memory for each cache entry.
         */
        void clear_cache() noexcept {
            for (auto& entry : g_cache_table)
                delete entry.second;  // clear the cache
        }
    }  // namespace solution
}  // namespace hmi_tree_optimization

