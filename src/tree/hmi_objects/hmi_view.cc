/*!
 * \file src/tree/hmi_objects/hmi_view.cc
 * \brief Implementation of HMI view node
 *
 * \see include/tree/hmi_objects/hmi_view.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/hmi_objects/hmi_view.hh"

#include <string>
#include <sstream>

#include "tree/node.hh"
#include "tree/cache_entry.hh"
#include "tree/cache_entries/hmi_view_cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \brief Construct HMI view node
         */
        HMIView::HMIView(nid_t id) noexcept
            : Node(id) {
        }

        /*!
         * \brief Return true (view is not suitable for caching)
         */
        bool HMIView::is_very_dirty() const {
            return true;  // never cache the root node
        }

        /*!
         * \brief Return a string representation of this HMI view 
         *
         * \details If the view is dirty, a '*' is prepended to the string.
         */
        std::string HMIView::to_string() const {
            std::ostringstream res;

            res << (is_dirty() ? "*" : "")
                << "{V|" 
                << get_id() 
                << "|ch:" << get_children().size() 
                << '}';
            return res.str();
        }

        /*!
         * \brief Return a detailed string representation <!-- 
         * --> (exposing internal variables' values) of this HMI view
         */
        std::string HMIView::repr() const {
            std::ostringstream res;

            res << std::boolalpha
                << "HMIView{"
                << "id=" << get_id()
                << ", d=" << is_dirty()
                << ", count(children)=" << get_children().size()
                << "}";
            return res.str();
        }

        /*!
         * \brief Return a character representation of this HMI view
         *
         * \details Represents an HMI view with the letter 'V'.
         */
        char HMIView::to_letter() const noexcept {
            return 'V';
        }

        /*!
         * \brief Apply update parameters to this HMI view
         *
         * \details Does nothing.
         */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
        void HMIView::apply_update(const std::vector<std::string>& parameters) {
        }
#pragma GCC diagnostic pop

        /*!
         * \brief Generate an HMI view cache entry
         *
         * \see HMIViewCacheEntry
         */
        CacheEntry *HMIView::generate_cache_entry() {
            return new HMIViewCacheEntry();
        }

        /*!
         * \brief Use a cache entry to restore the view's state
         *
         * \details Does nothing.
         */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
        void HMIView::use_cache_entry(const CacheEntry *entry) {
        }
#pragma GCC diagnostic pop
    }  // namespace tree
}  // namespace hmi_tree_optimization

