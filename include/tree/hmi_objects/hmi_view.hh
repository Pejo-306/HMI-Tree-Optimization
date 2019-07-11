#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_VIEW_HH

#include <string>

#include "tree/node.hh"

#include "tree/cache_entry.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIView final: public Node {
        public:
            explicit HMIView(nid_t) noexcept;
            bool is_very_dirty() const final;
            std::string to_string() const final;
            std::string repr() const final;
            char to_letter() const noexcept final;
        private:
            void apply_update(const std::vector<std::string>&) final;
            CacheEntry *generate_cache_entry() final;
            void use_cache_entry(const CacheEntry *) final;
        };  // class HMIView
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

