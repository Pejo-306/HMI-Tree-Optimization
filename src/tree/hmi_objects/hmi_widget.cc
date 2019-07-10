#include "tree/hmi_objects/hmi_widget.hh"

#include <string>
#include <sstream>

#include "tree/node.hh"
#include "tree/cache_entry.hh"
#include "tree/cache_entries/hmi_widget_cache_entry.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIWidget::HMIWidget(nid_t id) noexcept
            : Node(id) {
        }

        CacheEntry *HMIWidget::cache() {
            return new HMIWidgetCacheEntry();
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
        void HMIWidget::apply_update(const std::vector<std::string>& parameters) {
        }
#pragma GCC diagnostic pop
    }  // namespace tree
}  // namespace hmi_tree_optimization

