#include "tree/hmi_objects/hmi_widget.hh"

#include <string>
#include <sstream>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIWidget::HMIWidget(nid_t id) noexcept
            : Node(id) {
        }

        std::string HMIWidget::to_string() const {
            std::ostringstream res;

            res << "{W|" 
                << get_id() 
                << "|par:" << get_parents().size()
                << "|ch:" << get_children().size()
                << '}';
            return res.str();
        }

        std::string HMIWidget::repr() const {
            std::ostringstream res;

            res << "HMIWidget{"
                << "id=" << get_id()
                << ", dc=" << get_dirty_counter()
                << ", count(children)=" << get_children().size()
                << ", count(parents)=" << get_parents().size()
                << "}";
            return res.str();
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

