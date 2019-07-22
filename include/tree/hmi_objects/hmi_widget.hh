/*!
 * \file include/tree/hmi_objects/hmi_widget.hh
 * \brief Definition of HMI widget node
 *
 * In the context of the optimization algorithm HMI widgets are the main targets
 * which are to be cached - prerendering widgets would achieve the goal of the
 * algorithm by caching a whole branch of the graphical HMI tree.
 *
 * \see src/tree/hmi_objects/hmi_widget.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_HH

#include <unordered_set>
#include <string>

#include "tree/node.hh"
#include "tree/cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \class HMIWidget
         *
         * \brief Simulated HMI tree widget node
         *
         * A widget node is one that in a way represents a container of other HMI
         * tree nodes. It is meant to house data nodes (such as glyphs, images, LEDs, etc.)
         * and possibly other widgets.
         * It subclasses and fully implements the Node class.
         *
         * \see Node
         */
        class HMIWidget final: public Node {

        public:

            /*!
             * \brief Construct HMI widget node
             *
             * \param id The node's assigned id.
             */
            explicit HMIWidget(nid_t id) noexcept;

            /*!
             * \brief Return a string representation of this HMI widget 
             *
             * \see Node::to_string
             *
             * \return String representation of this widget node.
             */
            std::string to_string() const final;

            /*!
             * \brief Return a detailed string representation <!-- 
             * --> (exposing internal variables' values) of this HMI widget
             *
             * \see Node::repr
             *
             * \return Detailed string representation of this widget node.
             */
            std::string repr() const final;

            /*!
             * \brief Return a character representation of this HMI widget
             *
             * \see Node::to_letter
             *
             * \return Character representation of this widget node.
             */
            char to_letter() const noexcept final;

        private:

            /*!
             * \brief Apply update parameters to this HMI widget
             *
             * \see Node::apply_update
             *
             * \param parameters String parameters to be processed when updating.
             */
            void apply_update(const std::vector<std::string>& parameters) final;

            /*!
             * \brief Generate an HMI widget cache entry
             *
             * \see Node::generate_cache_entry
             *
             * \return A generated cache entry.
             */
            CacheEntry *generate_cache_entry() final;

            /*!
             * \brief Use a cache entry to restore the widget's state
             *
             * \see Node::use_cache_entry
             *
             * \param entry Cached HMI widget state which is to be restored.
             */
            void use_cache_entry(const CacheEntry *entry) final;
        };  // class HMIWidget
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__HMI_WIDGET_HH

