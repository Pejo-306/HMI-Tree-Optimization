#ifndef HMI_TREE_OPTIMIZATION__TREE__NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__NODE_HH

#include <iostream>
#include <string>
#include <unordered_set>

namespace hmi_tree_optimization {
    namespace tree {
        using nid_t = unsigned int;

        class Node {
        public:
            explicit Node(nid_t) noexcept;
            virtual ~Node() noexcept;
            nid_t get_id() const noexcept;
            std::unordered_set<Node *>& get_children() noexcept;
            const std::unordered_set<Node *>& get_children() const noexcept;
            Node& add_child(Node *);
            Node& add_child(Node&);
            Node& remove_child(Node *);
            Node& remove_child(Node&);
            bool has_child(const Node *) const noexcept;
            bool has_child(const Node&) const noexcept;
            std::unordered_set<const Node *>& get_parents() noexcept;
            const std::unordered_set<const Node *>& get_parents() const noexcept;
            Node& add_parent(Node *);
            Node& add_parent(Node&);
            Node& remove_parent(Node *);
            Node& remove_parent(Node&);
            bool has_parent(const Node *) const noexcept;
            bool has_parent(const Node&) const noexcept;
            int get_dirty_counter() const noexcept;
            virtual std::string to_string() const = 0;
            virtual std::string repr() const = 0;
        private:
            nid_t id_;
            std::unordered_set<Node *> children_;
            std::unordered_set<const Node *> parents_;
            int dirty_counter_;
        };  // class Node

        std::ostream& operator<<(std::ostream&, const Node&);
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

