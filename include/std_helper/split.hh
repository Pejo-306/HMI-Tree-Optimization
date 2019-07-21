/*!
 * \file include/std_helper/split.hh
 * \brief Declare function to split a string by a given delimiter
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__SPLIT_HH
#define HMI_TREE_OPTIMIZATION__TREE__SPLIT_HH

#include <string>
#include <vector>

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace std_helper
    //! \brief Contains functionality which extends the C++ standard library
    namespace std_helper {
        /*!
         * \brief Split a string by a delimiter
         *
         * \param s String to split.
         * \param delim Delimiter by which to split the given string.
         * \return A std::vector, containing the split string's elements.
         */
        std::vector<std::string> split(const std::string& s, const std::string& delim);
    }  // namespace std_helper
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__SPLIT_HH

