#include "tree/hmi_objects/hmi_view.hh"

#include <string>
#include <sstream>

#include "tree/nodes/node.hh"
#include "tree/nodes/iparent_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIView::HMIView(nid_t id) noexcept
            : Node(id),
              IParentNode(id) {
        }

        std::string HMIView::to_string() const noexcept {
            std::ostringstream res;

            res << "HMIView{"
                << "id=" << get_id()
                << ", count(children)=" << get_children().size()
                << "}";
            return res.str();
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

