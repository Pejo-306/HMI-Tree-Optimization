/*!
 * \file include/tree/hmi_exception.hh
 * \brief Definition of HMIException
 *
 * HMIExceptions are exceptions that are raised when an error occurs int the
 * operation of an HMITree instance.
 *
 * \see src/tree/hmi_exception.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__EXCEPTION_HH
#define HMI_TREE_OPTIMIZATION__TREE__EXCEPTION_HH

#include <exception>
#include <string>

#include "tree/node.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \class HMIException
         *
         * \brief Exception to throw when an error occurs in the HMI tree
         *
         * This exception can optionally be tied to a tree's node. It is
         * then apparent that the specified node was the cause of the exception.
         *
         * \see Node
         * \see HMITree
         */
        class HMIException: public std::exception {

        public:

            /*!
             * \brief Construct HMIException
             *
             * \param erroneous_node The node which was the (supposed) cause of the exception.
             */
            explicit HMIException(Node *erroneous_node);

            /*!
             * \brief Return an appropriate error message
             *
             * \return Error message, detailing the exception.
             */
            virtual const char *what() const noexcept override;

        private:

            Node *erroneous_node_;  /*!< Pointer to optional erroneous node. */
            std::string error_msg_;  /*!< Error message to display. */
        };  // class HMIException
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__EXCEPTION_HH

