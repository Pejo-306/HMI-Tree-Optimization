#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_TREE_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_TREE_HH

#include <iostream>
#include <unordered_set>
#include <deque>

#include "tree/node.hh"
#include "tree/hmi_objects/hmi_view.hh"

namespace hmi_tree_optimization {
    namespace tree {
        class HMITree final {
            class base_iterator {
                friend class HMITree;

            public:
                Node& operator*() const;
                Node *operator->() const noexcept;
                Node *address() const noexcept;
                bool operator==(const base_iterator&) const noexcept;
                bool operator!=(const base_iterator&) const noexcept;
            protected:
                ~base_iterator() noexcept;
            private:
                HMITree& owner_;
                Node *element_;
                base_iterator(HMITree&, Node *) noexcept;
            };  // class HMITree::base_iterator

        public:
            class dfs_iterator: public base_iterator {
                friend class base_iterator;
                friend class HMITree;

            public:
                dfs_iterator& operator++();
                dfs_iterator operator++(int);
            private:
                dfs_iterator(HMITree&, Node *, bool = true) noexcept;
                void shove_children_to_stack();
                Node *get_top_node();
            };  // class HMITree::dfs_iterator

            class bfs_iterator: public base_iterator {
                friend class base_iterator;
                friend class HMITree;

            public:
                bfs_iterator& operator++();
                bfs_iterator operator++(int);
            private:
                bfs_iterator(HMITree&, Node *, bool = true) noexcept;
                void shove_children_to_queue();
                Node *get_front_node();
            };  // class HMITree::bfs_iterator

            explicit HMITree() noexcept;
            HMITree& add_node(nid_t, Node *);
            HMITree& add_node(nid_t, Node&);
            HMITree& add_node(const Node&, Node *);
            HMITree& add_node(const Node&, Node&);
            Node& get_node(nid_t);
            HMIView& get_root() noexcept;
            const HMIView& get_root() const noexcept;
            nid_t lease_free_nid() noexcept;
            dfs_iterator dfs_begin();
            dfs_iterator dfs_end();
            bfs_iterator bfs_begin();
            bfs_iterator bfs_end();
        private:
            HMIView root_;
            std::unordered_set<nid_t> id_pool_;
            std::deque<Node *> it_container_;
        };  // class HMITree

        std::ostream& operator<<(std::ostream&, const HMITree&);
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif

