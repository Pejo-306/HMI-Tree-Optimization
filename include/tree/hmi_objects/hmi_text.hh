#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_TEXT_HH

#include <string>
#include <vector>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMIText final: public Node {
        public:
            explicit HMIText(nid_t, const std::string&) noexcept;
            Node& update(const std::vector<std::string>&) final;
            std::string to_string() const final;
            std::string repr() const final;
            const std::string& get_content() const noexcept;
            HMIText& set_content(const std::string&) noexcept;
        private:
            std::string content_;
        };  // class HMIText
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

