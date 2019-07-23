/*!
 * \file include/tree/cache_entries/hmi_view_cache_entry.hh
 * \brief Definition of the CacheEntry subclass for HMIView nodes.
 *
 * This class describes how to cache the state of an HMIView node.
 *
 * \see src/tree/cache_entries/hmi_view_cache_entry.cc
 * \see hmi_tree_optimization::tree::CacheEntry
 * \see hmi_tree_optimization::tree::HMIView
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_CACHE_ENTRY_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_CACHE_ENTRY_HH

#include "tree/cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \class HMIViewCacheEntry
         *
         * \brief Cache entry for HMIView nodes.
         *
         * \details This cache entry does not cache any information.
         *
         * \see CacheEntry
         * \see HMIView
         */
        class HMIViewCacheEntry final: public CacheEntry {

        public:

            /*!
             * \brief Return a detailed string representation of this HMIViewCacheEntry.
             *
             * \see CacheEntry::repr
             *
             * \return Detailed string representation of this HMIViewCacheEntry.
             */
            std::string repr() const final;
        };  // class HMIViewCacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_CACHE_ENTRY_HH

