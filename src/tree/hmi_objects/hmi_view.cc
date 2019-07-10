#include "tree/hmi_objects/hmi_view.hh"

#include <string>
#include <sstream>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIView::HMIView(nid_t id) noexcept
            : Node(id) {
        }

        bool HMIView::is_very_dirty() const {
            return true;  // never cache the root node
        }

        std::string HMIView::to_string() const {
            std::ostringstream res;

            res << (is_dirty() ? "*" : "")
                << "{V|" 
                << get_id() 
                << "|ch:" << get_children().size() 
                << '}';
            return res.str();
        }

        std::string HMIView::repr() const {
            std::ostringstream res;

            res << std::boolalpha
                << "HMIView{"
                << "id=" << get_id()
                << ", d=" << is_dirty()
                << ", count(children)=" << get_children().size()
                << "}";
            return res.str();
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
        void HMIView::apply_update(const std::vector<std::string>& parameters) {
        }
#pragma GCC diagnostic pop
    }  // namespace tree
}  // namespace hmi_tree_optimization

