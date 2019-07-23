/*!
 * \file include/tree/cache_entries/hmi_widget_cache_entry.hh
 * \brief Definition of the CacheEntry subclass for HMIWidget nodes.
 *
 * This class describes how to cache the state of an HMIWidget node.
 *
 * \see src/tree/cache_entries/hmi_widget_cache_entry.cc
 * \see hmi_tree_optimization::tree::CacheEntry
 * \see hmi_tree_optimization::tree::HMIWidget
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_CACHE_ENTRY_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_CACHE_ENTRY_HH

#include "tree/cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \class HMIWidgetCacheEntry
         *
         * \brief Cache entry for HMIWidget nodes.
         *
         * \details This cache entry does not cache any information.
         *
         * \see CacheEntry
         * \see HMIWidget
         */
        class HMIWidgetCacheEntry final: public CacheEntry {

        public:

            /*!
             * \brief Return a detailed string representation of this HMIWidgetCacheEntry.
             *
             * \see CacheEntry::repr
             *
             * \return Detailed string representation of this HMIWidgetCacheEntry.
             */
            std::string repr() const final;
        };  // class HMIWidgetCacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_CACHE_ENTRY_HH

