/*!
 * \file src/tree/hmi_tree.cc
 * \brief Implementation of the HMI tree class
 *
 * \see include/tree/hmi_tree.hh
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include "tree/hmi_tree.hh"

#include <cstdlib>
#include <iostream>
#include <limits>

#include "tree/node.hh"
#include "tree/hmi_exception.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        namespace {
            /*!
             * \brief Print a branch of a tree to an output stream.
             *
             * \param node Top node of branch to print.
             * \param out Output stream where node is printed.
             */
            void print_branch(const Node& node, std::ostream& out) noexcept {
                if (node.get_id() != 0) {
                    for (size_t p = 0; p < node.nall_parents() - 1; ++p)
                        out << " │   ";
                    out << " └──";
                }
                out << (node.is_very_dirty() ? "%" : "")
                    << (node.is_dirty() ? "*" : "") 
                    << node.to_letter() << "│" << node.get_id() 
                    << std::endl;
                for (const auto& child : node.get_children())
                    print_branch(*child, out);
            }
        }  // anonymous namespace

        using dfs_iterator = HMITree::dfs_iterator;
        using bfs_iterator = HMITree::bfs_iterator;

        /*!
         * \brief Access iterator element
         *
         * If the pointed-to element is null, an HMIException is thrown.
         *
         * \see HMIException
         * \see Node
         */
        Node& HMITree::base_iterator::operator*() const {
            if (element_ == nullptr)
                throw HMIException(element_);
            return *element_;
        }

        /*!
         * \brief Get pointer to iterator element
         */
        Node *HMITree::base_iterator::operator->() const noexcept {
            return element_;
        }

        /*!
         * \brief Get address of iterator element
         */
        Node *HMITree::base_iterator::address() const noexcept {
            return element_;
        }

        /*!
         * \brief Compare equality of two iterators
         */
        bool HMITree::base_iterator::operator==(const base_iterator& other) const noexcept {
            return element_ == other.element_;
        }

        /*!
         * \brief Compare inequality of two iterators
         *
         * \see HMITree::base_iterator::operator==
         */
        bool HMITree::base_iterator::operator!=(const base_iterator& other) const noexcept {
            return !operator==(other);
        }

        /*!
         * \brief Destroy tree iterator
         */
        HMITree::base_iterator::~base_iterator() noexcept {
        }

        /*!
         * \brief Construct tree iterator
         */
        HMITree::base_iterator::base_iterator(HMITree& owner, Node *element) noexcept
            : owner_(owner),
              element_(element) {
        }

        /*!
         * \brief Increment the iterator (preincrement)
         *
         * After advancing the iterator forward, the new current element's
         * children nodes are shoved to the DFS data structure.
         */
        dfs_iterator& dfs_iterator::operator++() {
            element_ = get_top_node();
            shove_children_to_stack();
            return *this;
        }

        /*!
         * \brief Increment the iterator (postincrement)
         *
         * To advance the iterator whilst still returning its current state,
         * a copy is created. The latter is returned, whilst the advanced
         * iterator adds all of its children to the DFS structure.
         */
        dfs_iterator dfs_iterator::operator++(int) {
            dfs_iterator res(owner_, element_, false);
            element_ = get_top_node();
            shove_children_to_stack();
            return res;
        }

        /*!
         * \brief Construct DFS tree iterator
         *
         * \see HMITree::dfs_iterator::shove_children_to_stack
         */
        dfs_iterator::dfs_iterator(HMITree& owner, Node *element, 
                bool shove_children) noexcept
            : base_iterator(owner, element) {
            if (shove_children)
                shove_children_to_stack();
        }

        /*!
         * \brief Provide current node's children for iteration
         *
         * \see Node
         */
        void dfs_iterator::shove_children_to_stack() {
            if (element_ == nullptr)
                return;

            for (Node *ch : element_->get_children())
                owner_.it_container_.push_back(ch);
        }

        /*!
         * \brief Get next node for iteration
         */
        Node *dfs_iterator::get_top_node() {
            Node *res;

            if (owner_.it_container_.empty()) {
                res = nullptr;
            } else {
                res = owner_.it_container_.back();
                owner_.it_container_.pop_back();
            }
            return res;
        }

        /*!
         * \brief Increment the iterator (preincrement)
         *
         * After advancing the iterator forward, the new current element's
         * children nodes are shoved to the BFS data structure.
         */
        bfs_iterator& bfs_iterator::operator++() {
            element_ = get_front_node();
            shove_children_to_queue();
            return *this;
        }

        /*!
         * \brief Increment the iterator (postincrement)
         *
         * To advance the iterator whilst still returning its current state,
         * a copy is created. The latter is returned, whilst the advanced
         * iterator adds all of its children to the BFS structure.
         */
        bfs_iterator bfs_iterator::operator++(int) {
            bfs_iterator res(owner_, element_, false);
            element_ = get_front_node();
            shove_children_to_queue();
            return res;
        }

        /*!
         * \brief Construct BFS tree iterator
         *
         * \see HMITree::dfs_iterator::shove_children_to_stack
         */
        bfs_iterator::bfs_iterator(HMITree& owner, Node *element, 
                bool shove_children) noexcept
            : base_iterator(owner, element) {
            if (shove_children)
                shove_children_to_queue();
        }

        /*!
         * \brief Provide current node's children for iteration
         *
         * \see Node
         */
        void bfs_iterator::shove_children_to_queue() {
            if (element_ == nullptr)
                return;

            for (Node *ch : element_->get_children())
                owner_.it_container_.push_back(ch);
        }

        /*!
         * \brief Get next node for iteration
         */
        Node *bfs_iterator::get_front_node() {
            Node *res;

            if (owner_.it_container_.empty()) {
                res = nullptr;
            } else {
                res = owner_.it_container_.front();
                owner_.it_container_.pop_front();
            }
            return res;
        }

        /*!
         * \brief Construct HMI tree
         *
         * \details Leases the root node's id (which is '0' by default).
         */
        HMITree::HMITree() noexcept
            : root_(0) {
            id_pool_.insert(0);
        }

        /*!
         * \brief Attach a new child node to one of the tree's other existing nodes
         *
         * \see Node::add_child
         */
        HMITree& HMITree::add_node(nid_t parent_id, Node *node) {
            get_node(parent_id).add_child(node);
            return *this;
        }

        /*!
         * \brief Attach a new child node to one of the tree's other existing nodes
         */
        HMITree& HMITree::add_node(nid_t parent_id, Node& node) {
            return add_node(parent_id, &node);
        }

        /*!
         * \brief Attach a new child node to one of the tree's other existing nodes
         */
        HMITree& HMITree::add_node(const Node& parent_node, Node *node) {
            return add_node(parent_node.get_id(), node);
        }

        /*!
         * \brief Attach a new child node to one of the tree's other existing nodes
         */
        HMITree& HMITree::add_node(const Node& parent_node, Node& node) {
            return add_node(parent_node.get_id(), &node);
        }

        /*!
         * \brief Provide access to an existing tree node.
         *
         * \see HMITree::bfs_iterator
         * \see HMITree::bfs_begin
         * \see HMITree::bfs_end
         */
        Node& HMITree::get_node(nid_t id) {
            for (bfs_iterator it = bfs_begin(); it != bfs_end(); ++it) {
                if (id == it->get_id()) {
                    it_container_.clear();
                    return *it;
                }
            }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"
            throw HMIException(nullptr);  // node has not been found
#pragma GCC diagnostic pop
        }

        /*!
         * \brief Provide access to the root node of a tree.
         */
        HMIView& HMITree::get_root() noexcept {
            return root_;
        }

        /*!
         * \brief Get the root node of a tree.
         */
        const HMIView& HMITree::get_root() const noexcept {
            return root_;
        }

        /*!
         * \brief Lease a free node id to address a tree's node.
         *
         * \note A random sequence seed must be initialized before invoking <!--
         * --> this function. Otherwise, the generated ids will be predictable.
         */
        nid_t HMITree::lease_free_nid(nid_t preferred_id) noexcept {
            nid_t res;

            if (id_pool_.find(preferred_id) == id_pool_.end()) {  // can lease prefered_id
                res = preferred_id;
            } else {  // generate a random node id
                do
                    res = rand() % std::numeric_limits<nid_t>::max() + 1;
                while (id_pool_.find(res) != id_pool_.end());
            }
            id_pool_.insert(res);
            return res;
        }

        /*!
         * \brief Begin DFS iteration. Get iterator to first element.
         *
         * \warning When creating a beginning iterator, the DFS/BFS iteration <!--
         * --> data structure is cleared. Therefore, multiple tree iterations <!--
         * --> cannot be intertwined.
         */
        dfs_iterator HMITree::dfs_begin() {
            it_container_.clear();
            return dfs_iterator(*this, &root_);
        }

        /*!
         * \brief Get ending DFS iterator.
         *
         * The ending iterator is indicated via a null pointer.
         */
        dfs_iterator HMITree::dfs_end() {
            return dfs_iterator(*this, nullptr, false);
        }

        /*!
         * \brief Begin BFS iteration. Get iterator to first element.
         *
         * \warning When creating a beginning iterator, the DFS/BFS iteration <!--
         * --> data structure is cleared. Therefore, multiple tree iterations <!--
         * --> cannot be intertwined.
         */
        bfs_iterator HMITree::bfs_begin() {
            it_container_.clear();
            return bfs_iterator(*this, &root_);
        }

        /*!
         * \brief Get ending BFS iterator.
         *
         * The ending iterator is indicated via a null pointer.
         */
        bfs_iterator HMITree::bfs_end() {
            return bfs_iterator(*this, nullptr, false);
        }
        
        /*!
         * \brief Print a tree to an output stream.
         *
         * Utilizes the internal recursive function print_branch() to output 
         * the given HMITree object. See implementation file for details.
         *
         * \see src/tree/hmi_tree.cc
         */
        std::ostream& operator<<(std::ostream& out, const HMITree& hmi_tree) {
            print_branch(hmi_tree.get_root(), out);
            return out;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

