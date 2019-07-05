#ifndef HMI_TREE_OPTIMIZATION__TREE__PARENT_NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__PARENT_NODE_HH

#include <unordered_set>

#include "tree/node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class ParentNode: public Node {
        public:
            explicit ParentNode(nid_t) noexcept;
            explicit ParentNode(nid_t, const std::unordered_set<const ParentNode *>&) noexcept;
            ParentNode& add_child(Node *);
            ParentNode& add_child(Node&);
            const std::unordered_set<Node *>& get_children() const noexcept;
        protected:
            virtual std::string get_typename() const noexcept final;
        private:
            std::unordered_set<Node *> children_;
        };  // class ParentNode
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

