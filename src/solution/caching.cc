/*!
 * \file src/solution/caching.cc
 * \brief Implementation of tree dirtiness evaluation and caching routines.
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

            /*!
             * \brief Evaluate the dirtiness of a given tree node.
             *
             * \see evaluate_tree_dirtiness()
             */
            void _evaluate_node_dirtiness(Node& node,
                    const std::unordered_set<nid_t>& heavy_hitters) {
                for (auto child_node : node.get_children())
                    _evaluate_node_dirtiness(*child_node, heavy_hitters);

                if (heavy_hitters.find(node.get_id()) != heavy_hitters.end())
                    // node is a heavy hitter - mark as not fit for caching
                    node.mark_as_very_dirty();
                else
                    node.mark_as_very_clean();
                node.clean_up();
            }
        }  // anonymous namespace

        std::unordered_map<nid_t, CacheEntry *> g_cache_table;

        /*!
         * \brief Evaluate the dirtiness of each tree node.
         *
         * This procedure traverses through the entire tree by utilizing DFS.
         * A depth-first search is preferred here because it naturally reaches
         * a parent node's children nodes before operating on said parent node.
         * Therefore, each parent node will have its children evaluated and
         * marked as either very dirty or very clean before evaluating the
         * parent.
         *
         * Essentially a node is marked as very dirty only if it is considered
         * a heavy hitter (i.e. a frequently updated element). All other nodes
         * are marked as very clean.
         *
         * All nodes are cleaned up (have their dirty state reset) for the
         * duration of the next frame in this step.
         */
        void evaluate_tree_dirtiness(HMITree& tree, 
                const std::unordered_set<nid_t>& heavy_hitters) {
            _evaluate_node_dirtiness(tree.get_root(), heavy_hitters);
        }

        /*!
         * \brief Refresh the screen.
         *
         * The refreshing process traverses the tree using BFS which naturally
         * must go through a parent node before reaching any of that parent's
         * children nodes. This way the optimization algorithm will always
         * prefer the highest possible nodes in the tree's hierarchy to cache.
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
         *
         * After the traversal process is complete, this routine cleans the
         * cache table of unnecessary cache table entries (i.e. whose 
         * corresponding nodes have a higher parent cached).
         */
        void refresh_screen(HMITree& tree) {
            Node *node;
            std::queue<Node *> nodes;
            nid_t node_id;
            std::unordered_set<nid_t> cached_ids;
            std::unordered_set<nid_t> erased_entries;

            // start from the root node
            nodes.push(&tree.get_root());
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
                    cached_ids.insert(node_id);
                } else {  // if (node->is_very_dirty())
                    auto cache_entry_it = g_cache_table.find(node_id);

                    if (cache_entry_it != g_cache_table.end()) {
                        // delete unnecessary cache entry
                        delete g_cache_table.at(node_id);
                        g_cache_table.erase(cache_entry_it);
                    }
                    node->render(g_debug);

                    // add a very dirty node's children for processing
                    for (auto child_node : node->get_children())
                        nodes.push(child_node);
                }
            }

            // garbage collection:
            // erase unnecessary cache table entries
            for (auto& entry : g_cache_table) {
                auto cache_entry_it = cached_ids.find(entry.first);

                if (cache_entry_it == cached_ids.end()) {
                    delete entry.second;
                    erased_entries.insert(entry.first);
                }
            }
            for (auto erased_entry_id : erased_entries)
                g_cache_table.erase(erased_entry_id);
            // cached_ids.clear();
            // erased_entries.clear();
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

