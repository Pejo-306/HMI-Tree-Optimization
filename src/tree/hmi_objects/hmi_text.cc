/*!
 * \file src/tree/hmi_objects/hmi_text.cc
 * \brief Implementation of HMI text node
 *
 * \see include/tree/hmi_objects/hmi_text.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/hmi_objects/hmi_text.hh"

#include <string>
#include <sstream>

#include "tree/node.hh"
#include "tree/cache_entry.hh"
#include "tree/cache_entries/hmi_text_cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \brief Construct HMI text node
         */
        HMIText::HMIText(nid_t id, const std::string& content) noexcept
            : Node(id),
              content_(content) {
        }

        /*!
         * \brief Return a string representation of this HMI text 
         *
         * \details If the view is dirty, a '*' is prepended to the string.
         */
        std::string HMIText::to_string() const {
            std::ostringstream res;

            res << (is_dirty() ? "*" : "")
                << "{T|" 
                << get_id() 
                << "|'" << content_ 
                << "'}";
            return res.str();
        }

        /*!
         * \brief Return a detailed string representation <!-- 
         * --> (exposing internal variables' values) of this HMI text
         */
        std::string HMIText::repr() const {
            std::ostringstream res;

            res << std::boolalpha
                << "HMIText{"
                << "id=" << get_id()
                << ", d=" << is_dirty()
                << ", count(parents)=" << get_parents().size()
                << ", content='" << content_ << '\''
                << "}";
            return res.str();
        }

        /*!
         * \brief Return a character representation of this HMI text
         *
         * \details Represents an HMI text with the letter 'T'.
         */
        char HMIText::to_letter() const noexcept {
            return 'T';
        }

        /*!
         * \brief Get text content of HMI text node
         */
        const std::string& HMIText::get_content() const noexcept {
            return content_;
        }

        /*!
         * \brief Set text content of HMI text node
         */
        HMIText& HMIText::set_content(const std::string& content) noexcept {
            content_ = content;
            return *this;
        }

        /*!
         * \brief Apply update parameters to this HMI text
         *
         * \details parameters[0] is expected to be the new text content
         */
        void HMIText::apply_update(const std::vector<std::string>& parameters) {
            content_ = parameters[0];
        }

        /*!
         * \brief Generate an HMI text cache entry
         */
        CacheEntry *HMIText::generate_cache_entry() {
            return new HMITextCacheEntry(content_);
        }

        /*!
         * \brief Use a cache entry to restore the text's state
         */
        void HMIText::use_cache_entry(const CacheEntry *entry) {
            const std::unordered_map<std::string, void *>& attributes = entry->get_attributes();

            content_ = *static_cast<std::string *>(attributes.at("content_"));
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

