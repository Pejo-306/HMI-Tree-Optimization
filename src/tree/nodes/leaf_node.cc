#include "tree/nodes/leaf_node.hh"

#include <string>
#include <sstream>

namespace hmi_tree_optimization {
    namespace tree {
        LeafNode::LeafNode(nid_t id, data_t data) noexcept
            : Node(id),
              data_(data) {
        }

        LeafNode::LeafNode(nid_t id, const std::unordered_set<const ParentNode *>& parents, 
                data_t data) noexcept
            : Node(id, parents),
              data_(data) {
        }

        data_t LeafNode::get_data() const noexcept {
            return data_;
        }

        std::string LeafNode::to_string() const noexcept {
            std::ostringstream result;
            
            result << get_typename() << "{" 
                << "id=" << get_id()
                << ", dc=" << get_dirty_counter() 
                << ", data=" << get_data()
                << "}";
            return result.str();
        }

        std::string LeafNode::get_typename() const noexcept {
            return "LeafNode";
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

