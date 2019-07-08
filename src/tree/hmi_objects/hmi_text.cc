#include "tree/hmi_objects/hmi_text.hh"

#include <string>
#include <sstream>

#include "tree/nodes/node.hh"
#include "tree/nodes/idirty_node.hh"
#include "tree/nodes/ichild_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIText::HMIText(nid_t id, const std::string& content) noexcept
            : Node(id),
              IDirtyNode(id),
              IChildNode(id),
              content_(content) {
        }

        std::string HMIText::to_string() const {
            std::ostringstream res;

            res << "{T|" 
                << get_id() 
                << "|'" << content_ 
                << "'}";
            return res.str();
        }

        std::string HMIText::repr() const {
            std::ostringstream res;

            res << "HMIText{"
                << "id=" << get_id()
                << ", dc=" << get_dirty_counter()
                << ", count(parents)=" << get_parents().size()
                << ", content='" << content_ << '\''
                << "}";
            return res.str();
        }

        const std::string& HMIText::get_content() const noexcept {
            return content_;
        }

        HMIText& HMIText::set_content(const std::string& content) noexcept {
            content_ = content;
            return *this;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

