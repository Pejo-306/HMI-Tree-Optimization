/*!
 * \file src/solution/config.cc
 * \brief Default values of global configuration variables
 *
 * \see include/solution/config.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "solution/config.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace solution 
    //! \brief Solution to the presented optimization problem
    namespace solution {
        bool g_debug = false;
    }  // namespace solution
}  // namespace hmi_tree_optimization

