#include "tree/cache_entries/hmi_text_cache_entry.hh"

#include <string>
#include <unordered_map>

namespace hmi_tree_optimization {
    namespace tree {
        HMITextCacheEntry::HMITextCacheEntry(const std::string& content) {
            std::unordered_map<std::string, void *>& attributes = get_attributes();

            attributes.insert(std::make_pair("content_", new std::string(content)));
        }

        HMITextCacheEntry::~HMITextCacheEntry() noexcept {
            std::unordered_map<std::string, void *>& attributes = get_attributes();

            delete static_cast<std::string *>(attributes["content_"]);
        }

        void HMITextCacheEntry::must_implement() const noexcept {
        }
    }  // namespace tree 
}  // namespace hmi_tree_optimization

