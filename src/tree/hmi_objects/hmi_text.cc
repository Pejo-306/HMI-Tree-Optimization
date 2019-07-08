#include "tree/hmi_objects/hmi_text.hh"

#include <string>
#include <sstream>

#include "tree/nodes/node.hh"
#include "tree/nodes/idirty_node.hh"
#include "tree/nodes/ichild_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIText::HMIText(nid_t id) noexcept
            : Node(id),
              IDirtyNode(id),
              IChildNode(id) {
        }

        std::string HMIText::to_string() const noexcept {
            std::ostringstream res;

            res << "HMIText{"
                << "id=" << get_id()
                << ", dc=" << get_dirty_counter()
                << ", count(parents)=" << get_parents().size()
                << "}";
            return res.str();
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

