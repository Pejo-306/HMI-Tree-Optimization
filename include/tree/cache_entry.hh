#ifndef HMI_TREE_OPTIMIZATION__TREE__CACHE_ENTRY_HH
#define HMI_TREE_OPTIMIZATION__TREE__CACHE_ENTRY_HH

#include <string>
#include <unordered_map>

namespace hmi_tree_optimization {
    namespace tree {
        class CacheEntry {
        public:
            const std::unordered_map<std::string, void *>& get_attributes() const noexcept;
            virtual ~CacheEntry() noexcept;
            virtual std::string repr() const = 0;
        protected:
            std::unordered_map<std::string, void *>& get_attributes() noexcept;
        private:
            std::unordered_map<std::string, void *> attributes_;
        };  // class CacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

