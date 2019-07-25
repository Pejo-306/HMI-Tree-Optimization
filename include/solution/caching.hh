/*!
 * \file include/solution/caching.hh
 * \brief Defines the algorithm's routines to evaluate the dirtiness of a <!--
 * --> tree and cache the most fitting nodes.
 *
 * \see src/solution/caching.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__SOLUTION__CACHING_HH
#define HMI_TREE_OPTIMIZATION__SOLUTION__CACHING_HH

#include <unordered_map>
#include <unordered_set>

#include "tree/hmi_tree.hh"
#include "tree/cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace solution 
    //! \brief Solution to the presented optimization problem
    namespace solution {
        extern std::unordered_map<tree::nid_t, tree::CacheEntry *> g_cache_table;  /*!< Maps node ids to cache entries. */

        /*!
         * \brief Evaluate the dirtiness of each tree node.
         *
         * Iterates over the whole HMI tree and decides how to mark each node.
         * They are either marked as very dirty (not fit for caching) or as
         * very clean (fit for caching). The decision is based on the frequency
         * of occurrence of each node.
         *
         * \note Whether marked as very dirty or very clean, each node has its <!--
         * --> dirtiness state reset (i.e. it is no longer considered dirty).
         *
         * After the evaluation process has been complete, the program must
         * decide which nodes to actually cache.
         *
         * \see tree::HMITree
         * \see refresh_screen
         *
         * \param tree HMI tree to evaluate.
         * \param heavy_hitters Set of frequently updated nodes.
         */
        void evaluate_tree_dirtiness(tree::HMITree& tree, 
                const std::unordered_set<tree::nid_t>& heavy_hitters);

        /*!
         * \brief Refresh the screen.
         *
         * Fully refreshing the screen would require the rendering of the
         * entire tree data structure which is a very slow operation. This is
         * why the cache table is firstly searched for any cached branches and
         * directly restores their state. The cache table is updated accordingly
         * if any of their cached nodes become dirty. All other nodes are
         * rendered as usual.
         *
         * \see tree::HMITree
         * \see evaluate_tree_dirtiness
         *
         * \param tree HMI tree to rerender.
         */
        void refresh_screen(tree::HMITree& tree);

        /*!
         * \brief Clear the cache table.
         */
        void clear_cache() noexcept;
    }  // namespace solution
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__SOLUTION__CACHING_HH

