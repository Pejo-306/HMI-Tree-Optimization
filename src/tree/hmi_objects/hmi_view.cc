#include "tree/hmi_objects/hmi_view.hh"

#include <string>
#include <sstream>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIView::HMIView(nid_t id) noexcept
            : Node(id) {
        }

        std::string HMIView::to_string() const {
            std::ostringstream res;

            res << "{V|" 
                << get_id() 
                << "|ch:" << get_children().size() 
                << '}';
            return res.str();
        }

        std::string HMIView::repr() const {
            std::ostringstream res;

            res << "HMIView{"
                << "id=" << get_id()
                << ", count(children)=" << get_children().size()
                << "}";
            return res.str();
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

