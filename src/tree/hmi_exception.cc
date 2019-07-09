#include "tree/hmi_exception.hh"

#include <sstream>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIException::HMIException(Node *erroneous_node)
            : erroneous_node_(erroneous_node) {
            std::ostringstream msg;
            
            msg << "An error occurred in the HMI tree (erroneous node is '"
                << (erroneous_node_ == nullptr ? "null" : erroneous_node_->repr())
                << "')";
            error_msg_ = msg.str();
        }

        const char *HMIException::what() const noexcept {
            return error_msg_.c_str();
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

