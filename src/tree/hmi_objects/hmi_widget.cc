#include "tree/hmi_objects/hmi_widget.hh"

#include <string>
#include <sstream>

#include "tree/nodes/node.hh"
#include "tree/nodes/idirty_node.hh"
#include "tree/nodes/iparent_node.hh"
#include "tree/nodes/ichild_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIWidget::HMIWidget(nid_t id) noexcept
            : Node(id),
              IDirtyNode(id),
              IParentNode(id),
              IChildNode(id) {
        }

        std::string HMIWidget::to_string() const noexcept {
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

