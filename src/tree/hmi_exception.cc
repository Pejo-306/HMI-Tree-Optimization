/*!
 * \file src/tree/hmi_exception.cc
 * \brief Implementation of HMIException
 *
 * \see include/tree/hmi_exception.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/hmi_exception.hh"

#include <sstream>

#include "tree/node.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \brief Construct HMIException
         */
        HMIException::HMIException(Node *erroneous_node)
            : erroneous_node_(erroneous_node) {
            std::ostringstream msg;
            
            msg << "An error occurred in the HMI tree (erroneous node is '"
                << (erroneous_node_ == nullptr ? "null" : erroneous_node_->repr())
                << "')";
            error_msg_ = msg.str();
        }

        /*!
         * \brief Return an appropriate error message
         */
        const char *HMIException::what() const noexcept {
            return error_msg_.c_str();
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

