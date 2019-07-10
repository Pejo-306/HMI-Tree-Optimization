#include "tree/hmi_objects/hmi_text.hh"

#include <string>
#include <sstream>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        HMIText::HMIText(nid_t id, const std::string& content) noexcept
            : Node(id),
              content_(content) {
        }

        std::string HMIText::to_string() const {
            std::ostringstream res;

            res << (is_dirty() ? "*" : "")
                << "{T|" 
                << get_id() 
                << "|'" << content_ 
                << "'}";
            return res.str();
        }

        std::string HMIText::repr() const {
            std::ostringstream res;

            res << std::boolalpha
                << "HMIText{"
                << "id=" << get_id()
                << ", dc=" << get_dirty_counter()
                << ", d=" << is_dirty()
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

        void HMIText::apply_update(const std::vector<std::string>& parameters) {
            // The new string content should be located at parameters[0]
            content_ = parameters[0];
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

