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
        protected:
            std::unordered_map<std::string, void *>& get_attributes() noexcept;
            virtual void must_implement() const noexcept = 0;
        private:
            std::unordered_map<std::string, void *> attributes_;
        };  // class CacheEntry
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

