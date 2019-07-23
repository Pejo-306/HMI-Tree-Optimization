/*!
 * \file src/tree/cache_entries/hmi_widget_cache_entry.cc
 * \brief Implementation of the CacheEntry subclass for HMIWidget nodes.
 *
 * \see include/tree/cache_entries/hmi_widget_cache_entry.hh
 * \see hmi_tree_optimization::tree::CacheEntry
 * \see hmi_tree_optimization::tree::HMIWidget
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/cache_entries/hmi_widget_cache_entry.hh"

#include <string>
#include <sstream>

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \brief Return a detailed string representation of this HMIWidgetCacheEntry.
         */
        std::string HMIWidgetCacheEntry::repr() const {
            std::ostringstream res;
            
            res << "HMIWidgetCacheEntry{"
                << "}";
            return res.str();
        }
    }  // namespace tree 
}  // namespace hmi_tree_optimization

