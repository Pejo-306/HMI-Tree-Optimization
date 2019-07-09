#include "std_helper/split.hh"

#include <iterator>
#include <string>
#include <vector>

namespace hmi_tree_optimization {
    namespace std_helper {
        std::vector<std::string> split(const std::string& s, char delim) {
            std::vector<std::string> items;
            
            split(s, delim, std::back_inserter(items));
            return items;
        }
    }  // namespace std_helper
}  // namespace hmi_tree_optimization

