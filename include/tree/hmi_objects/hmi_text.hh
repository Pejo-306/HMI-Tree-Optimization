/*!
 * \file include/tree/hmi_objects/hmi_text.hh
 * \brief Definition of HMI text node
 *
 * Text nodes represent leaf nodes of an HMI tree - nodes which contain the
 * displayed data (i.e. images, text, LEDs, etc.).
 * It is not desirable to cache these nodes by the optimization algorithm.
 * Nonetheless, their dirtiness state is still takes a pivotal place in the
 * aforementioned algorithm. These data nodes are updated the most frequently
 * and are most likely to be marked as very dirty. And if any one of a branch's 
 * leaf nodes is marked as non-cacheable, the branch itself cannot be cached.
 *
 * \see src/tree/hmi_objects/hmi_text.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH

#include <string>
#include <vector>

#include "tree/node.hh"
#include "tree/cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \class HMIText
         *
         * \brief Data leaf node which stores text content
         *
         * While these text nodes have the capability to have children nodes,
         * they are meant to represent the leaf nodes of an HMI tree. Therefore,
         * it is not recommended to attach any child nodes to these text ones.
         *
         * \see Node
         */
        class HMIText final: public Node {
            
        public:

            /*!
             * \brief Construct HMI text node
             *
             * \param id The node's assigned id.
             * \param content Text content of node.
             */
            explicit HMIText(nid_t id, const std::string& content) noexcept;

            /*!
             * \brief Return a string representation of this HMI text 
             *
             * \see Node::to_string
             *
             * \return String representation of this text node.
             */
            std::string to_string() const final;

            /*!
             * \brief Return a detailed string representation <!-- 
             * --> (exposing internal variables' values) of this HMI text
             *
             * \see Node::repr
             *
             * \return Detailed string representation of this text node.
             */
            std::string repr() const final;

            /*!
             * \brief Return a character representation of this HMI text
             *
             * \see Node::to_letter
             *
             * \return Character representation of this text node.
             */
            char to_letter() const noexcept final;

            /*!
             * \brief Get text content of HMI text node
             *
             * \return Text content of this node.
             */
            const std::string& get_content() const noexcept;

            /*!
             * \brief Set text content of HMI text node
             *
             * \param content Newly assigned text content of node.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            HMIText& set_content(const std::string& content) noexcept;

        private:

            std::string content_;  /*!< Text content of node. */

            /*!
             * \brief Apply update parameters to this HMI text
             *
             * \see Node::apply_update
             *
             * \param parameters String parameters to be processed when updating.
             */
            void apply_update(const std::vector<std::string>& parameters) final;

            /*!
             * \brief Generate an HMI text cache entry
             *
             * \see Node::generate_cache_entry
             *
             * \return A generated cache entry.
             */
            CacheEntry *generate_cache_entry() final;

            /*!
             * \brief Use a cache entry to restore the text's state
             *
             * \see Node::use_cache_entry
             *
             * \param entry Cached HMI text state which is to be restored.
             */
            void use_cache_entry(const CacheEntry *entry) final;
        };  // class HMIText
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH

