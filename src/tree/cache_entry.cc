/*!
 * \file src/tree/cache_entry.cc
 * \brief Implementation of the abstract CacheEntry class
 *
 * \see include/tree/cache_entry.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/cache_entry.hh"

#include <unordered_map>
#include <string>

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \brief Get map of attribute values copies.
         */
        const std::unordered_map<std::string, void *>& 
                CacheEntry::get_attributes() const noexcept {
            return attributes_;
        }

        /*!
         * \brief Destroy cache entry.
         */
        CacheEntry::~CacheEntry() noexcept {
        }

        /*!
         * \brief Access map of attribute values copies.
         */
        std::unordered_map<std::string, void *>& CacheEntry::get_attributes() noexcept {
            return attributes_;
        }
    }  // namespace tree 
}  // namespace hmi_tree_optimization

