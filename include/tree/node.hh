#ifndef HMI_TREE_OPTIMIZATION__TREE__NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__NODE_HH

#include <iostream>
#include <unordered_set>
#include <string>

namespace hmi_tree_optimization {
    namespace tree {
        using nid_t = int;

        class ParentNode;

        class Node {
        public:
            explicit Node(nid_t ) noexcept;
            explicit Node(nid_t, const std::unordered_set<const ParentNode *>&) noexcept;
            virtual ~Node() noexcept;
            Node& add_parent(const ParentNode *);
            Node& add_parent(const ParentNode&);
            nid_t get_id() const noexcept;
            const std::unordered_set<const ParentNode *>& get_parents() const noexcept;
            int get_dirty_counter() const noexcept;
            friend std::ostream& operator<<(std::ostream&, const Node&);
        protected:
            virtual std::string get_typename() const noexcept;
        private:
            nid_t id_;
            std::unordered_set<const ParentNode *> parents_;
            int dirty_counter_;
        };  // class Node
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

