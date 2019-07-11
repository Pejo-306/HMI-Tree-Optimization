#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH

#include <string>
#include <vector>

#include "tree/node.hh"
#include "tree/cache_entry.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIText final: public Node {
        public:
            explicit HMIText(nid_t, const std::string&) noexcept;
            std::string to_string() const final;
            std::string repr() const final;
            const std::string& get_content() const noexcept;
            HMIText& set_content(const std::string&) noexcept;
        private:
            std::string content_;
            void apply_update(const std::vector<std::string>&) final;
            CacheEntry *generate_cache_entry() final;
            void use_cache_entry(const CacheEntry *) final;
        };  // class HMIText
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

