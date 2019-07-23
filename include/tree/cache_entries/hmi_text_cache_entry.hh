/*!
 * \file include/tree/cache_entries/hmi_text_cache_entry.hh
 * \brief Definition of the CacheEntry subclass for HMIText nodes.
 *
 * This class describes how to cache the state of an HMIText node.
 *
 * \see src/tree/cache_entries/hmi_text_cache_entry.cc
 * \see hmi_tree_optimization::tree::CacheEntry
 * \see hmi_tree_optimization::tree::HMIText
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_CACHE_ENTRY_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_CACHE_ENTRY_HH

#include <string>

#include "tree/cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \class HMITextCacheEntry 
         *
         * \brief Cache entry for HMIText nodes.
         *
         * This cache entry stores the following information from HMIText:
         * - "content_": std::string
         *
         * \see CacheEntry
         * \see HMIText
         */
        class HMITextCacheEntry final: public CacheEntry {

        public:

            /*!
             * \brief Create HMIText cache entry.
             *
             * \param content Reference to 'content_' attribute of an HMIText node.
             */
            explicit HMITextCacheEntry(const std::string& content);

            /*!
             * \brief Destroy HMIText cache entry.
             */
            ~HMITextCacheEntry() noexcept;

            /*!
             * \brief Return a detailed string representation of this HMITextCacheEntry.
             *
             * \see CacheEntry::repr
             *
             * \return Detailed string representation of this HMITextCacheEntry.
             */
            std::string repr() const final;
        };  // class HMITextCacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_CACHE_ENTRY_HH

