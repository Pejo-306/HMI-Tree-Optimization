#ifndef HMI_TREE_OPTIMIZATION__TREE__LEAF_NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__LEAF_NODE_HH

#include <unordered_set>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        using data_t = int;

        class LeafNode: public Node {
        public:
            explicit LeafNode(nid_t, data_t) noexcept;
            explicit LeafNode(nid_t, const std::unordered_set<const ParentNode *>&, 
                    data_t) noexcept;
            data_t get_data() const noexcept;
            virtual std::string to_string() const noexcept final;
        private:
            data_t data_;
            virtual std::string get_typename() const noexcept final;
        };  // class LeafNode 
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

