/*!
 * \file include/tree/cache_entry.hh
 * \brief Definition of the abstract CacheEntry class
 *
 * The caching process used by this project is only an exemplary simulation
 * process. It is not intended to actually cache a node's state via the 
 * methodology described by this class.
 *
 * \see src/tree/cache_entry.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__CACHE_ENTRY_HH
#define HMI_TREE_OPTIMIZATION__TREE__CACHE_ENTRY_HH

#include <string>
#include <unordered_map>

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \class CacheEntry
         *
         * \brief Caches and stores a node's state.
         *
         * This abstract class only provides the interface through which a
         * Node can be cached, stored and then extracted and restored to a
         * previous state. The specifics of caching nodes' properties must be
         * described by a specific subclass, appropriate for a given Node type.
         *
         * The presented caching method works as follows: a cache
         * entry contains a map which associates an attribute's name to a
         * copy of its value. The latter is stored in heap memory and may be
         * referenced by a void *. Only the most important attributes should
         * be stored in this cache entry (node addressing and neighboring 
         * information is not meant to be stored within an entry).
         *
         * \see Node
         */
        class CacheEntry {

        public:

            /*!
             * \brief Get map of attribute values copies.
             *
             * \note It is called on immutable Node objects.
             *
             * \return Map of attribute values copies.
             */
            const std::unordered_map<std::string, void *>& get_attributes() const noexcept;

            /*!
             * \brief Destroy cache entry.
             */
            virtual ~CacheEntry() noexcept;

            /*!
             * \brief Return a detailed string representation of this CacheEntry.
             *
             * \details This method must be implemented by each CacheEntry subclass.
             *
             * \return Detailed string representation of this CacheEntry.
             */
            virtual std::string repr() const = 0;

        protected:

            /*!
             * \brief Access map of attribute values copies.
             *
             * \return Map of attribute values copies.
             */
            std::unordered_map<std::string, void *>& get_attributes() noexcept;

        private:

            std::unordered_map<std::string, void *> attributes_;  /*!< Map of attribute values copies. */
        };  // class CacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__CACHE_ENTRY_HH

