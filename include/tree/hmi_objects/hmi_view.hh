/*!
 * \file include/tree/hmi_objects/hmi_view.hh
 * \brief Definition of HMI view node
 *
 * An HMI view node is intended to be used as the root node of an HMI tree.
 * It subclasses and fully implements the Node class.
 *
 * \see src/tree/hmi_objects/hmi_view.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH

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
         * \class HMIView
         *
         * \brief Root node of an HMI tree
         *
         * This type of node has only aesthetic meaning in the context of an
         * HMI tree instance as it represents the latter's root. HMI view nodes
         * do not hold any specific data and are supposed to have only children
         * nodes.
         * Furthermore, in the context of the optimization algorithm view nodes
         * are always considered very dirty (i.e. not cacheable).
         *
         * \see Node
         */
        class HMIView final: public Node {

        public:

            /*!
             * \brief Construct HMI view node
             *
             * \param id The node's assigned id.
             */
            explicit HMIView(nid_t id) noexcept;

            /*!
             * \brief Return true (view is not suitable for caching)
             *
             * \see Node::is_very_dirty
             *
             * \return True (always).
             */
            bool is_very_dirty() const final;

            /*!
             * \brief Return a string representation of this HMI view 
             *
             * \see Node::to_string
             *
             * \return String representation of this view node.
             */
            std::string to_string() const final;

            /*!
             * \brief Return a detailed string representation <!-- 
             * --> (exposing internal variables' values) of this HMI view
             *
             * \see Node::repr
             *
             * \return Detailed string representation of this view node.
             */
            std::string repr() const final;

            /*!
             * \brief Return a character representation of this HMI view
             *
             * \see Node::to_letter
             *
             * \return Character representation of this view node.
             */
            char to_letter() const noexcept final;

        private:

            /*!
             * \brief Apply update parameters to this HMI view
             *
             * \see Node::apply_update
             *
             * \param parameters String parameters to be processed when updating.
             */
            void apply_update(const std::vector<std::string>& parameters) final;

            /*!
             * \brief Generate an HMI view cache entry
             *
             * \see Node::generate_cache_entry
             *
             * \return A generated cache entry.
             */
            CacheEntry *generate_cache_entry() final;

            /*!
             * \brief Use a cache entry to restore the view's state
             *
             * \see Node::use_cache_entry
             *
             * \param entry Cached HMI view state which is to be restored.
             */
            void use_cache_entry(const CacheEntry *entry) final;
        };  // class HMIView
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH

