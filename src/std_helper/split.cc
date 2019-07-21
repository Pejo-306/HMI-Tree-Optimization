/**
 * @file src/std_helper/split.cc
 * @brief Implement function to split a string by a given delimiter
 *
 * @author Petar Nikolov
 * @version v1.0.0
 */

#include "std_helper/split.hh"

#include <string>
#include <vector>

/// @namespace hmi_tree_optimization
/// @brief Project namespace
namespace hmi_tree_optimization {

    /// @namespace std_helper
    /// @brief Contains functionality which extends the C++ standard library
    namespace std_helper {
        // Split a string by a delimiter
        std::vector<std::string> split(const std::string& s, const std::string& delim) {
            std::vector<std::string> items;
            std::string s_cp = s;
            size_t pos;
            std::string token;
            
            while ((pos = s_cp.find(delim)) != std::string::npos) {
                token = s_cp.substr(0, pos);
                items.push_back(token);
                s_cp.erase(0, pos + delim.length());
            }
            items.push_back(s_cp);  // insert the last remaining token
            return items;
        }
    }  // namespace std_helper
}  // namespace hmi_tree_optimization

