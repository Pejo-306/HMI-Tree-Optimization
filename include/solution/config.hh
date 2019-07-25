/*!
 * \file include/solution/config.hh
 * \brief Global configuration variables for program
 *
 * \see src/solution/config.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__CONFIG_HH
#define HMI_TREE_OPTIMIZATION__TREE__CONFIG_HH

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace solution 
    //! \brief Solution to the presented optimization problem
    namespace solution {
        extern bool g_debug;  /*!< Display debug messages. */
    }  // namespace solution 
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__CONFIG_HH

