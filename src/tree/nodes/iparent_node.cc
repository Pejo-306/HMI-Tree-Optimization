#include "tree/nodes/iparent_node.hh"

#include "tree/nodes/node.hh"
#include "tree/nodes/ichild_node.hh"

namespace hmi_tree_optimization {
    namespace tree {
        IParentNode::IParentNode(nid_t id) noexcept
            : Node(id) {
        }

        IParentNode::~IParentNode() noexcept {
            const std::unordered_set<IChildNode *> children_copy(children_);

            for (auto& ch : children_copy) {
                remove_child(ch);
                if (ch->get_parents().size() == 0)
                    delete ch;
            }
        }

        IParentNode& IParentNode::add_child(IChildNode *child_node) {
            children_.insert(child_node);
            if (!child_node->has_parent(this))
                child_node->add_parent(this);
            return *this;
        }

        IParentNode& IParentNode::add_child(IChildNode& child_node) {
            return add_child(&child_node);
        }

        IParentNode& IParentNode::remove_child(IChildNode *child_node) {
            children_.erase(child_node);
            if (child_node->has_parent(this))
                child_node->remove_parent(this);
            return *this;
        }

        IParentNode& IParentNode::remove_child(IChildNode& child_node) {
            return remove_child(&child_node);
        }

        const std::unordered_set<IChildNode *>& IParentNode::get_children() const noexcept {
            return children_;
        }

        bool IParentNode::has_child(const IChildNode *child_node) const noexcept {
            return children_.find(const_cast<IChildNode *>(child_node)) != children_.end();
        }

        bool IParentNode::has_child(const IChildNode& child_node) const noexcept {
            return has_child(&child_node);
        }
    }  // namespace tree
}  // namespace hmi_tree_optimization

