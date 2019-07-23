/*!
 * \file src/tree/cache_entries/hmi_text_cache_entry.cc
 * \brief Implementation of the CacheEntry subclass for HMIText nodes.
 *
 * \see include/tree/cache_entries/hmi_text_cache_entry.hh
 * \see hmi_tree_optimization::tree::CacheEntry
 * \see hmi_tree_optimization::tree::HMIText
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/cache_entries/hmi_text_cache_entry.hh"

#include <string>
#include <sstream>
#include <unordered_map>

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \brief Create HMIText cache entry.
         *
         * Stores the values of the node's attributes to the attributes map
         * in heap memory. The allocated memory should only be freed by the
         * created cache entry.
         *
         * \see CacheEntry
         * \see HMIText
         * \see HMITextCacheEntry::~HMITextCacheEntry
         */
        HMITextCacheEntry::HMITextCacheEntry(const std::string& content) {
            std::unordered_map<std::string, void *>& attributes = get_attributes();

            attributes.insert(std::make_pair("content_", new std::string(content)));
        }

        /*!
         * \brief Destroy HMIText cache entry.
         *
         * Frees all the allocated dynamic memory for the copies of a node's
         * attributes' values.
         *
         * \see CacheEntry
         * \see HMIText
         * \see HMITextCacheEntry::HMITextCacheEntry
         */
        HMITextCacheEntry::~HMITextCacheEntry() noexcept {
            std::unordered_map<std::string, void *>& attributes = get_attributes();

            delete static_cast<std::string *>(attributes.at("content_"));
        }

        /*!
         * \brief Return a detailed string representation of this HMITextCacheEntry.
         *
         * Displays the cached attribute values of an HMIText instance.
         */
        std::string HMITextCacheEntry::repr() const {
            const std::unordered_map<std::string, void *>& attributes = get_attributes();
            std::ostringstream res;
            const std::string& content = *static_cast<std::string *>(attributes.at("content_"));
            
            res << "HMITextCacheEntry{"
                << "content=" << content
                << "}";
            return res.str();
        }
    }  // namespace tree 
}  // namespace hmi_tree_optimization

