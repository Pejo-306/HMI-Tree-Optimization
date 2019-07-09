#ifndef HMI_TREE_OPTIMIZATION__TREE__SPLIT_HH
#define HMI_TREE_OPTIMIZATION__TREE__SPLIT_HH

#include <string>
#include <sstream>
#include <vector>

namespace hmi_tree_optimization {
    namespace std_helper {
        template<typename Out_T>
        void split(const std::string& s, char delim, Out_T result) {
            std::string item;
            std::stringstream ss(s);

            while (std::getline(ss, item, delim))
                *(result++) = item;
        }

        std::vector<std::string> split(const std::string&, char);
    }  // namespace std_helper
}  // namespace hmi_tree_optimization

#endif

