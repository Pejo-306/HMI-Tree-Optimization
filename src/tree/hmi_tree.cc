#include "tree/hmi_tree.hh"

#include <cstdlib>
#include <limits>

#include "tree/node.hh"
#include "tree/hmi_exception.hh"

namespace hmi_tree_optimization {
    namespace tree {
        using dfs_iterator = HMITree::dfs_iterator;
        using bfs_iterator = HMITree::bfs_iterator;

        Node& HMITree::base_iterator::operator*() const {
            if (element_ == nullptr)
                throw HMIException(element_);
            return *element_;
        }

        Node *HMITree::base_iterator::operator->() const {
            return element_;
        }

        bool HMITree::base_iterator::operator==(const base_iterator& other) const {
            return element_ == other.element_;
        }

        bool HMITree::base_iterator::operator!=(const base_iterator& other) const {
            return !operator==(other);
        }

        HMITree::base_iterator::~base_iterator() noexcept {
        }

        HMITree::base_iterator::base_iterator(HMITree& owner, Node *element) noexcept
            : owner_(owner),
              element_(element) {
        }

        dfs_iterator& dfs_iterator::operator++() {
            element_ = get_top_node();
            shove_children_to_stack();
            return *this;
        }

        dfs_iterator dfs_iterator::operator++(int) {
            dfs_iterator res(owner_, element_, false);
            element_ = get_top_node();
            shove_children_to_stack();
            return res;
        }

        dfs_iterator::dfs_iterator(HMITree& owner, Node *element, 
                bool shove_children) noexcept
            : base_iterator(owner, element) {
            if (shove_children)
                shove_children_to_stack();
        }

        void dfs_iterator::shove_children_to_stack() {
            if (element_ == nullptr)
                return;

            for (Node *ch : element_->get_children())
                owner_.it_container_.push_back(ch);
        }

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

        bfs_iterator& bfs_iterator::operator++() {
            element_ = get_front_node();
            shove_children_to_queue();
            return *this;
        }

        bfs_iterator bfs_iterator::operator++(int) {
            bfs_iterator res(owner_, element_, false);
            element_ = get_front_node();
            shove_children_to_queue();
            return res;
        }

        bfs_iterator::bfs_iterator(HMITree& owner, Node *element, 
                bool shove_children) noexcept
            : base_iterator(owner, element) {
            if (shove_children)
                shove_children_to_queue();
        }

        void bfs_iterator::shove_children_to_queue() {
            if (element_ == nullptr)
                return;

            for (Node *ch : element_->get_children())
                owner_.it_container_.push_back(ch);
        }

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

        HMITree::HMITree() noexcept
            : root_(0) {
        }

        HMITree& HMITree::add_node(nid_t parent_id, Node *node) {
            get_node(parent_id).add_child(node);
            return *this;
        }

        HMITree& HMITree::add_node(nid_t parent_id, Node& node) {
            return add_node(parent_id, &node);
        }

        HMITree& HMITree::add_node(const Node& parent_node, Node *node) {
            return add_node(parent_node.get_id(), node);
        }

        HMITree& HMITree::add_node(const Node& parent_node, Node& node) {
            return add_node(parent_node.get_id(), &node);
        }

        Node& HMITree::get_node(nid_t id) noexcept {
            for (bfs_iterator it = bfs_begin(); it != bfs_end(); ++it) {
                if (id == (*it).get_id()) {
                    it_container_.clear();
                    return *it;
                }
            }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wterminate"
            throw HMIException(nullptr);  // node has not been found
#pragma GCC diagnostic pop
        }

        HMIView& HMITree::get_root() noexcept {
            return root_;
        }

        const HMIView& HMITree::get_root() const noexcept {
            return root_;
        }

        // NOTE: a random seed should be initialized before invoking this function.
        nid_t HMITree::lease_free_nid() noexcept {
            nid_t res;

            do
                res = rand() % std::numeric_limits<nid_t>::max() + 1;
            while (id_pool_.find(res) != id_pool_.end());
            id_pool_.insert(res);
            return res;
        }

        dfs_iterator HMITree::dfs_begin() {
            it_container_.clear();
            return dfs_iterator(*this, &root_);
        }

        dfs_iterator HMITree::dfs_end() {
            return dfs_iterator(*this, nullptr, false);
        }

        bfs_iterator HMITree::bfs_begin() {
            it_container_.clear();
            return bfs_iterator(*this, &root_);
        }

        bfs_iterator HMITree::bfs_end() {
            return bfs_iterator(*this, nullptr, false);
        }
        
        std::ostream& operator<<(std::ostream& out, const HMITree& hmi_tree) {
            return out;
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

