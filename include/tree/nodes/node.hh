#ifndef HMI_TREE_OPTIMIZATION__TREE__NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__NODE_HH

#include <iostream>
#include <string>

namespace hmi_tree_optimization {
    namespace tree {
        using nid_t = unsigned int;

        class Node {
        public:
            explicit Node(nid_t) noexcept;
            virtual ~Node() noexcept;
            nid_t get_id() const noexcept;
            virtual std::string to_string() const noexcept = 0;
        private:
            nid_t id_;
        };  // class Node

        std::ostream& operator<<(std::ostream&, const Node&);
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

