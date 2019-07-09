#ifndef HMI_TREE_OPTIMIZATION__TREE__EXCEPTION_HH
#define HMI_TREE_OPTIMIZATION__TREE__EXCEPTION_HH

#include <exception>
#include <string>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIException: public std::exception {
        public:
            explicit HMIException(Node *);
            virtual const char *what() const noexcept override;
        private:
            Node *erroneous_node_;
            std::string error_msg_;
        };  // class HMIException
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

