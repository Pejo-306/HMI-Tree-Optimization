/*!
 * \file src/tree/hmi_objects/hmi_widget.cc
 * \brief Implementation of HMI widget node
 *
 * \see include/tree/hmi_objects/hmi_widget.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/hmi_objects/hmi_widget.hh"

#include <string>
#include <sstream>

#include "tree/node.hh"
#include "tree/cache_entry.hh"
#include "tree/cache_entries/hmi_widget_cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \brief Construct HMI widget node
         */
        HMIWidget::HMIWidget(nid_t id) noexcept
            : Node(id) {
        }

        /*!
         * \brief Return a string representation of this HMI widget 
         *
         * \details If the view is dirty, a '*' is prepended to the string.
         */
        std::string HMIWidget::to_string() const {
            std::ostringstream res;

            res << (is_dirty() ? "*" : "")
                << "{W|" 
                << get_id() 
                << "|par:" << get_parents().size()
                << "|ch:" << get_children().size()
                << '}';
            return res.str();
        }

        /*!
         * \brief Return a detailed string representation <!-- 
         * --> (exposing internal variables' values) of this HMI widget
         */
        std::string HMIWidget::repr() const {
            std::ostringstream res;

            res << std::boolalpha
                << "HMIWidget{"
                << "id=" << get_id()
                << ", d=" << is_dirty()
                << ", count(children)=" << get_children().size()
                << ", count(parents)=" << get_parents().size()
                << "}";
            return res.str();
        }

        /*!
         * \brief Return a character representation of this HMI widget
         *
         * \details Represents an HMI view with the letter 'W'.
         */
        char HMIWidget::to_letter() const noexcept {
            return 'W';
        }

        /*!
         * \brief Apply update parameters to this HMI widget
         * 
         * \details Does nothing.
         */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
        void HMIWidget::apply_update(const std::vector<std::string>& parameters) {
        }
#pragma GCC diagnostic pop

        /*!
         * \brief Generate an HMI widget cache entry
         *
         * \see HMIWidgetCacheEntry
         */
        CacheEntry *HMIWidget::generate_cache_entry() {
            return new HMIWidgetCacheEntry();
        }

        /*!
         * \brief Use a cache entry to restore the widget's state
         *
         * \details Does nothing.
         */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
        void HMIWidget::use_cache_entry(const CacheEntry *entry) {
        }
#pragma GCC diagnostic pop
    }  // namespace tree
}  // namespace hmi_tree_optimization

