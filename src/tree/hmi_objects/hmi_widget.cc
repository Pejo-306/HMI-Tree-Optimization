#include "tree/hmi_objects/hmi_widget.hh"

#include <algorithm>
#include <string>
#include <sstream>
#include <unordered_set>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIWidget::HMIWidget(nid_t id) noexcept
            : Node(id) {
        }

        bool HMIWidget::is_very_dirty() const {
            const std::unordered_set<Node *>& children = get_children();

            if (std::any_of(children.begin(), children.end(), [](Node *child) {
                        return child->is_very_dirty();
                        })) {
                return true;
            }
            return Node::is_very_dirty();
        }

        std::string HMIWidget::to_string() const {
            std::ostringstream res;

            res << (is_dirty() ? "*" : "")
                << "{W|" 
                << get_id() 
                << "|par:" << get_parents().size()
                << "|ch:" << get_children().size()
                << '}';
            return res.str();
        }

        std::string HMIWidget::repr() const {
            std::ostringstream res;

            res << std::boolalpha
                << "HMIWidget{"
                << "id=" << get_id()
                << ", dc=" << get_dirty_counter()
                << ", d=" << is_dirty()
                << ", count(children)=" << get_children().size()
                << ", count(parents)=" << get_parents().size()
                << "}";
            return res.str();
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

