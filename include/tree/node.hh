/*!
 * \file include/tree/node.hh
 * \brief Definition of HMI tree node structure
 *
 * The class 'Node' itself is abstract. It implements the functionality, required
 * to attach the node to an HMI tree and simulate an actual HMI tree node.
 * Content management and display is to be implemented by its subclasses.
 *
 * \see src/tree/node.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__NODE_HH
#define HMI_TREE_OPTIMIZATION__TREE__NODE_HH

#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "tree/cache_entry.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        using nid_t = unsigned int;  /*!< Alias type for a node's id. */

        /*!
         * \class Node
         *
         * \brief Stripped down implementation of an HMI tree node
         *
         * This node implementation supports only the bare essentials to simulate
         * a real HMI tree, i.e. it only expresses tree nodes' parent-child
         * relations, as well as addressing via node ids. Other important behavior
         * used in the workings of the optimization algorithm, is simulated
         * (e.g. the is_dirty() evaluation, rendering work, etc.).
         *
         * This class is abstract. As mentioned it only handles interactions
         * between itself and other tree nodes. The 'Node' class is not responsible
         * for managing any kind of content, which should be handled by its
         * subclasses.
         */
        class Node {

        public:

            /*!
             * \brief Construct HMI tree node
             *
             * \param id The node's assigned id.
             *
             * \warning The node's id should be unique for a given tree. <!--
             * --> Program behavior is undefined otherwise.
             */
            explicit Node(nid_t id) noexcept;

            /*!
             * \brief Destroy HMI tree node
             */
            virtual ~Node() noexcept;

            /*!
             * \brief Get the node's assigned id
             *
             * \return The node's assigned id.
             */
            nid_t get_id() const noexcept;

            /*!
             * \brief Access the node's associated child nodes
             *
             * \return The node's children.
             */
            std::unordered_set<Node *>& get_children() noexcept;

            /*!
             * \brief Get the node's associated child nodes
             *
             * \note It is called on immutable Node objects.
             *
             * \return The node's children (immutable).
             */
            const std::unordered_set<Node *>& get_children() const noexcept;

            /*!
             * \brief Associate another node as a child of this one
             *
             * \param child_node Pointer to already allocated node.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& add_child(Node *child_node);

            /*!
             * \brief Associate another node as a child of this one
             *
             * \param child_node Reference to already allocated node.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& add_child(Node& child_node);

            /*!
             * \brief Remove the parent-child association between this node and the given child
             *
             * \param child_node Pointer to a child node of this instance.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& remove_child(Node *child_node);

            /*!
             * \brief Remove the parent-child association between this node and the given child
             *
             * \param child_node Reference to a child node of this instance.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& remove_child(Node& child_node);

            /*!
             * \brief Check if a specified node is this node's direct child
             *
             * Indirect children nodes are not recursively looked up. This
             * method returns true only if the given node is this one's direct 
             * child.
             *
             * \param child_node Pointer to potential child node of this instance.
             *
             * \return True if node is a direct child node. False otherwise.
             */
            bool has_child(const Node *child_node) const noexcept;

            /*!
             * \brief Check if a specified node is this node's direct child
             *
             * Indirect children nodes are not recursively looked up. This
             * method returns true only if the given node is this one's direct 
             * child.
             *
             * \param child_node Reference to potential child node of this instance.
             *
             * \return True if node is a direct child node. False otherwise.
             */
            bool has_child(const Node& child_node) const noexcept;

            /*!
             * \brief Access the node's associated parent nodes
             *
             * \return The node's parents.
             */
            std::unordered_set<const Node *>& get_parents() noexcept;

            /*!
             * \brief Get the node's associated parent nodes
             *
             * \note It is called on immutable Node objects.
             *
             * \return The node's parents (immutable).
             */
            const std::unordered_set<const Node *>& get_parents() const noexcept;
            
            /*!
             * \brief Associate another node as a parent of this one
             *
             * \param parent_node Pointer to already allocated node.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& add_parent(Node *parent_node);

            /*!
             * \brief Associate another node as a parent of this one
             *
             * \param parent_node Reference to already allocated node.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& add_parent(Node& parent_node);

            /*!
             * \brief Remove the child-parent association between this node and the given parent 
             *
             * \param parent_node Pointer to a parent node of this instance.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& remove_parent(Node *parent_node);

            /*!
             * \brief Remove the child-parent association between this node and the given parent 
             *
             * \param parent_node Reference to a parent node of this instance.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& remove_parent(Node& parent_node);

            /*!
             * \brief Check if a specified node is this node's direct parent
             *
             * Indirect parent nodes are not recursively looked up. This
             * method returns true only if the given node is this one's direct
             * parent.
             *
             * \param child_node Pointer to potential parent node of this instance.
             *
             * \return True if node is a direct parent node. False otherwise.
             */
            bool has_parent(const Node *parent_node) const noexcept;

            /*!
             * \brief Check if a specified node is this node's direct parent
             *
             * Indirect parent nodes are not recursively looked up. This
             * method returns true only if the given node is this one's direct
             * parent.
             *
             * \param child_node Reference to potential parent node of this instance.
             *
             * \return True if node is a direct parent node. False otherwise.
             */
            bool has_parent(const Node& parent_node) const noexcept;

            /*!
             * \brief Check if node is dirty
             *
             * A dirty node is one that has been updated at least once between 
             * two rendered frames.
             *
             * \return True if node is dirty. False otherwise.
             */
            bool is_dirty() const noexcept;

            /*!
             * \brief Check if node is very dirty
             *
             * A very dirty node is one that is being updated very frequently 
             * in recent consecutive frames. Therefore, it isn't suitable for
             * caching.
             * 
             * \details This method may be overridden by a Node's subclass.
             * \details The node must manually be marked as very dirty from the outside.
             * 
             * \see Node::mark_as_very_dirty
             *
             * \return True if node is marked as very dirty. False otherwise.
             */
            virtual bool is_very_dirty() const;

            /*!
             * \brief Mark a node as very dirty
             *
             * \see Node::is_very_dirty
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& mark_as_very_dirty() noexcept;

            /*!
             * \brief Check if node is very clean
             *
             * A very clean node is one that has been updated seldom frequently 
             * in recent consecutive frames. Therefore, it is suitable for
             * caching.
             * 
             * \details A very clean node is one that is not very dirty. <!--
             * --> It may still be dirty from time to time.
             * 
             * \see Node::is_very_dirty
             * \see Node::mark_as_very_clean
             *
             * \return True if node is marked as very clean. False otherwise.
             */
            bool is_very_clean() const;

            /*!
             * \brief Mark a node as very clean
             *
             * \see Node::is_very_clean
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& mark_as_very_clean() noexcept;

            /*!
             * \brief Restore dirtiness state after a frame render
             *
             * Marks the node as not dirty. Should be called on every node after
             * a frame rendering has finished.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& clean_up() noexcept;

            /*!
             * \brief Update this node
             *
             * \param parameters String parameters to be processed by a <!--
             * --> Node's subclass when updating.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& update(const std::vector<std::string>& parameters);

            /*!
             * \brief Simulate time-consuming work when rendering a node
             *
             * \param debug Indicates whether to display debug messages.
             * \param out Output stream where debug messages are displayed.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& render(bool debug = false, std::ostream& out = std::cout) noexcept;

            /*!
             * \brief Cache this node and return an entry to be stored
             *
             * The caching process itself is specific to each tree node type.
             * Nonetheless, when caching a new CacheEntry should be created
             * and provided for managing from the outside.
             *
             * \param debug Indicates whether to display debug messages.
             * \param out Output stream where debug messages are displayed.
             *
             * \return A generated cache entry.
             */
            CacheEntry *cache(bool debug = false, std::ostream& out = std::cout);

            /*!
             * \brief Use a cache entry to restore a node's previous state
             *
             * Since the caching process is specific to the derived node type,
             * the restoration process is also specific to the Node's subclass.
             *
             * \param entry Cached node state which is to be restored.
             * \param debug Indicates whether to display debug messages.
             * \param out Output stream where debug messages are displayed.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            Node& load_from_cache(const CacheEntry *entry, bool debug = false, 
                    std::ostream& out = std::cout);

            /*!
             * \brief Count the number of all direct and indirect children
             *
             * \return Number of all children (direct and indirect).
             */
            size_t nall_children() const noexcept;

            /*!
             * \brief Count the number of all direct and indirect parents
             *
             * \return Number of all parents (direct and indirect).
             */
            size_t nall_parents() const noexcept;

            /*!
             * \brief Return a string representation of this node
             *
             * \details This method must be implemented by each Node subclass.
             *
             * \return String representation of this node.
             */
            virtual std::string to_string() const = 0;

            /*!
             * \brief Return a detailed string representation <!-- 
             * --> (exposing internal variables' values) of this node
             *
             * \details This method must be implemented by each Node subclass.
             *
             * \return Detailed string representation of this node.
             */
            virtual std::string repr() const = 0;

            /*!
             * \brief Return a character representation of this node
             *
             * \details This method must be implemented by each Node subclass.
             *
             * \return Character representation of this node.
             */
            virtual char to_letter() const noexcept = 0;

        protected:

            /*!
             * \brief Apply update parameters to this node
             *
             * \details This method must be implemented by each Node subclass.
             *
             * \param parameters String parameters to be processed when updating.
             */
            virtual void apply_update(const std::vector<std::string>& parameters) = 0;

            /*!
             * \brief Generate an appropriate cache entry
             *
             * The appropriate cache entry obviously depends on the derived
             * Node's subclass type.
             *
             * \details This method must be implemented by each Node subclass.
             *
             * \return A generated cache entry.
             */
            virtual CacheEntry *generate_cache_entry() = 0;

            /*!
             * \brief Use a cache entry to restore a node's state
             *
             * The appropriate type of cache entry must be parsed to this 
             * method in order to properly restore the node's state. The 
             * appropriate type obviously depends on the specific Node subclass.
             *
             * \details This method must be implemented by each Node subclass.
             *
             * \param entry Cached node state which is to be restored.
             */
            virtual void use_cache_entry(const CacheEntry *entry) = 0;

        private:

            static constexpr long render_time = 10;  /*!< Average (simulated) time for rendering. */
            nid_t id_;  /*!< The node's assigned id. */
            std::unordered_set<Node *> children_;  /*!< Set of all direct children. */
            std::unordered_set<const Node *> parents_;  /*!< Set of all direct parents. */
            bool dirty_;  /*!< Whether the node is dirty. */
            bool very_dirty_;  /*!< Whether the node has been marked as very dirty. */
        };  // class Node

        /*!
         * \brief Print a node to an output stream
         *
         * \param out Output stream where node is printed.
         */
        std::ostream& operator<<(std::ostream& out, const Node& node);
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__NODE_HH

