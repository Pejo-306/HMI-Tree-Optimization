/*!
 * \file include/tree/hmi_tree.hh
 * \brief Definition of the HMI tree class
 *
 * The tree data structure, defined in this file, simulates the real HMI tree
 * data structure which is made up of various types of nodes (widgets, glyphs,
 * LEDs, reference nodes, etc.). This stripped-down version provides various
 * facilities for accessing and modifying its elements, as well as iterating
 * over the tree via either DFS or BFS.
 *
 * \see src/tree/hmi_tree.cc
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#ifndef HMI_TREE_OPTIMIZATION__TREE__HMI_TREE_HH
#define HMI_TREE_OPTIMIZATION__TREE__HMI_TREE_HH

#include <iostream>
#include <unordered_set>
#include <deque>

#include "tree/node.hh"
#include "tree/hmi_objects/hmi_view.hh"

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace tree
    //! \brief Contains a bare bones simulated HMI tree
    namespace tree {
        /*!
         * \class HMITree
         *
         * \brief Simulated tree data structure of HMI graphical environment
         *
         * This tree data structure simulates the bare essentials of the real
         * HMI tree - addressing and manipulating nodes, as well as tree traversal.
         * It is the preferred way to interact with HMI nodes (i.e. bare instances
         * of Node should not be stored and manipulated within the optimization
         * algorithm's codebase).
         *
         * Also, The HMITree can be iterated over via the use of iterators.
         * Two different types of tree traversals are implemented - DFS and BFS
         * respectively implemented via the HMITree::dfs_iterator and
         * HMITree::bfs_iterator classes.
         *
         * \see Node
         * \see HMITree::dfs_iterator
         * \see HMITree::bfs_iterator
         */
        class HMITree final {

            /*!
             * \class HMITree::base_iterator
             *
             * \brief Abstract tree iterator class
             *
             * This class is internal and is to be subclassed internally
             * by the tree's publicly available iterators.
             *
             * \see HMITree
             * \see HMITree::dfs_iterator
             * \see HMITree::bfs_iterator
             */
            class base_iterator {
                friend class HMITree;

            public:

                /*!
                 * \brief Access iterator element
                 *
                 * \return Reference to iterator tree node.
                 */
                Node& operator*() const;

                /*!
                 * \brief Get pointer to iterator element
                 *
                 * \return Pointer to iterator tree node.
                 */
                Node *operator->() const noexcept;

                /*!
                 * \brief Get address of iterator element
                 * 
                 * \return Pointer to iterator tree node.
                 */
                Node *address() const noexcept;

                /*!
                 * \brief Compare equality of two iterators
                 *
                 * Two iterators are equal if they point to the same tree node.
                 *
                 * \return True if iterators are equal. False otherwise.
                 */
                bool operator==(const base_iterator& other) const noexcept;

                /*!
                 * \brief Compare inequality of two iterators
                 *
                 * Two iterators are not equal if they point to different tree nodes.
                 *
                 * \return True if iterators are not equal. False otherwise.
                 */
                bool operator!=(const base_iterator& other) const noexcept;

            protected:

                /*!
                 * \brief Destroy tree iterator
                 *
                 * \note This destructor is marked protected in order to make the class abstract.
                 */
                ~base_iterator() noexcept;

            private:

                HMITree& owner_;  /*!< Owner tree which is iterated over. */
                Node *element_;  /*!< Pointer to current element. */

                /*!
                 * \brief Construct tree iterator
                 *
                 * \details This constructor is to be used internally by the <!--
                 * --> HMITree's implementation.
                 *
                 * \see Node
                 * \see HMITree
                 *
                 * \param owner HMITree instance which owns this iterator.
                 * \param element Pointer to current element.
                 */
                base_iterator(HMITree& owner, Node *element) noexcept;
            };  // class HMITree::base_iterator

        public:

            /*!
             * \class HMITree::dfs_iterator
             *
             * \brief Iterates over the tree by utilizing DFS.
             *
             * This iterator subclasses and fully implements the
             * HMITree::base_iterator internal private class.
             *
             * \see HMITree
             * \see HMITree::base_iterator
             */
            class dfs_iterator: public base_iterator {
                friend class base_iterator;
                friend class HMITree;

            public:

                /*!
                 * \brief Increment the iterator (preincrement)
                 *
                 * This operation advances the iterator forward to the next node.
                 *
                 * \return Reference to this iterator (which has been modified).
                 */
                dfs_iterator& operator++();

                /*!
                 * \brief Increment the iterator (postincrement)
                 *
                 * This operation advances the iterator forward to the next node.
                 *
                 * \return Reference to copy of this iterator (before modification).
                 */
                dfs_iterator operator++(int);

            private:

                /*!
                 * \brief Construct DFS tree iterator
                 *
                 * This constructor is to be used internally by the HMITree's 
                 * implementation.
                 *
                 * \see Node
                 * \see HMITree
                 * \see HMITree::base_iterator
                 *
                 * \param owner HMITree instance which owns this iterator.
                 * \param element Pointer to current element.
                 * \param shove_children Indicates whether to iterate over children.
                 */
                dfs_iterator(HMITree& owner, Node *element, 
                        bool shove_children = true) noexcept;

                /*!
                 * \brief Provide current node's children for iteration
                 *
                 * Shove children to DFS data structure.
                 * If there is no current element, the method fails silently.
                 */
                void shove_children_to_stack();

                /*!
                 * \brief Get next node for iteration
                 *
                 * If there are children available for iteration, this method returns
                 * the top one from the DFS data structure. Otherwise, a null pointer
                 * is returned.
                 * 
                 * \see Node
                 *
                 * \return Top node of DFS data structure.
                 */
                Node *get_top_node();
            };  // class HMITree::dfs_iterator

            /*!
             * \class HMITree::dfs_iterator
             *
             * \brief Iterates over the tree by utilizing BFS.
             *
             * This iterator subclasses and fully implements the
             * HMITree::base_iterator internal private class.
             *
             * \see HMITree
             * \see HMITree::base_iterator
             */
            class bfs_iterator: public base_iterator {
                friend class base_iterator;
                friend class HMITree;

            public:

                /*!
                 * \brief Increment the iterator (preincrement)
                 *
                 * This operation advances the iterator forward to the next node.
                 *
                 * \return Reference to this iterator (which has been modified).
                 */
                bfs_iterator& operator++();

                /*!
                 * \brief Increment the iterator (postincrement)
                 *
                 * This operation advances the iterator forward to the next node.
                 *
                 * \return Reference to copy of this iterator (before modification).
                 */
                bfs_iterator operator++(int);

            private:

                /*!
                 * \brief Construct BFS tree iterator
                 *
                 * This constructor is to be used internally by the HMITree's 
                 * implementation.
                 *
                 * \see Node
                 * \see HMITree
                 * \see HMITree::base_iterator
                 *
                 * \param owner HMITree instance which owns this iterator.
                 * \param element Pointer to current element.
                 * \param shove_children Indicates whether to iterate over children.
                 */
                bfs_iterator(HMITree& owner, Node *element, 
                        bool shove_children = true) noexcept;

                /*!
                 * \brief Provide current node's children for iteration
                 *
                 * Shove children to BFS data structure.
                 * If there is no current element, the method fails silently.
                 */
                void shove_children_to_queue();

                /*!
                 * \brief Get next node for iteration
                 *
                 * If there are children available for iteration, this method returns
                 * the front one from the BFS data structure. Otherwise, a null pointer
                 * is returned.
                 * 
                 * \see Node
                 *
                 * \return Front node of BFS data structure.
                 */
                Node *get_front_node();
            };  // class HMITree::bfs_iterator

            /*!
             * \brief Construct HMI tree
             *
             * \note The root node is always initialized with an id of '0'.
             */
            explicit HMITree() noexcept;

            /*!
             * \brief Attach a new child node to one of the tree's other existing nodes
             *
             * This method creates a new parent-child association between the
             * existing tree parent node, denoted by the parsed parent id, and
             * the newly provided node.
             *
             * \warning Do not pass a pointer to a statically allocated Node <!--
             * --> instance. Program will cause a Segmentation fault.
             * 
             * \see Node
             *
             * \param parent_id Node id of existing tree node.
             * \param node Pointer to new tree node. Must have been <!--
             * --> dynamically allocated beforehand.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            HMITree& add_node(nid_t parent_id, Node *node);

            /*!
             * \brief Attach a new child node to one of the tree's other existing nodes
             *
             * \warning Do not pass a reference to a statically allocated Node <!--
             * --> instance. Program will cause a Segmentation fault.
             * 
             * \see Node
             *
             * \param parent_id Node id of existing tree node.
             * \param node Reference to new tree node. Must have been <!--
             * --> dynamically allocated beforehand.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            HMITree& add_node(nid_t parent_id, Node& node);

            /*!
             * \brief Attach a new child node to one of the tree's other existing nodes
             *
             * \warning Do not pass a pointer to a statically allocated Node <!--
             * --> instance. Program will cause a Segmentation fault.
             * 
             * \see Node
             *
             * \param parent_node Reference to existing tree parent node.
             * \param node Pointer to new tree node. Must have been <!--
             * --> dynamically allocated beforehand.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            HMITree& add_node(const Node& parent_node, Node *node);

            /*!
             * \brief Attach a new child node to one of the tree's other existing nodes
             *
             * \warning Do not pass a reference to a statically allocated Node <!--
             * --> instance. Program will cause a Segmentation fault.
             * 
             * \see Node
             *
             * \param parent_node Reference to existing tree parent node.
             * \param node Reference to new tree node. Must have been <!--
             * --> dynamically allocated beforehand.
             *
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            HMITree& add_node(const Node& parent_node, Node& node);

            /*!
             * \brief Provide access to an existing tree node.
             *
             * \note If node is not found, a HMIException is thrown.
             *
             * \see Node
             *
             * \param id ID of wanted node.
             *
             * \return Reference to existing tree node.
             */
            Node& get_node(nid_t id);

            /*!
             * \brief Provide access to the root node of a tree.
             *
             * \note This node is always an HMIView instance.
             *
             * \see HMIView
             *
             * \return Reference to tree's root HMI view.
             */
            HMIView& get_root() noexcept;

            /*!
             * \brief Get the root node of a tree.
             *
             * \note This node is always an HMIView instance.
             * \note It is called on immutable Node objects.
             *
             * \see HMIView
             *
             * \return Constant reference to tree's root HMI view.
             */
            const HMIView& get_root() const noexcept;

            /*!
             * \brief Lease a free node id to address a tree's node.
             *
             * The caller may optionally specify a preferred id to be leased
             * for a node. If it is available (not already taken), the given
             * id is leased and returned. Otherwise, a random node id is
             * generated and returned to the caller.
             * 
             * \param preferred_id Preferred id to lease if it hasn't already been taken.
             *
             * \return Available leased node id.
             */
            nid_t lease_free_nid(nid_t preferred_id = 0) noexcept;

            /*!
             * \brief Begin DFS iteration. Get iterator to first element.
             *
             * \details The beginning element is always the tree's root element.
             *
             * \see HMITree::dfs_iterator
             *
             * \return DFS iterator to first element (root).
             */
            dfs_iterator dfs_begin();

            /*!
             * \brief Get ending DFS iterator.
             *
             * The end iterator is to point to after the last element. Therefore,
             * when it is reached, the iteration process should be stopped.
             *
             * \see HMITree::dfs_iterator
             *
             * \return Ending DFS iterator.
             */
            dfs_iterator dfs_end();

            /*!
             * \brief Begin BFS iteration. Get iterator to first element.
             *
             * \details The beginning element is always the tree's root element.
             *
             * \see HMITree::bfs_iterator
             *
             * \return BFS iterator to first element (root).
             */
            bfs_iterator bfs_begin();

            /*!
             * \brief Get ending BFS iterator.
             *
             * The end iterator is to point to after the last element. Therefore,
             * when it is reached, the iteration process should be stopped.
             *
             * \see HMITree::bfs_iterator
             *
             * \return Ending BFS iterator.
             */
            bfs_iterator bfs_end();

        private:

            HMIView root_;  /*!< Root node of HMI tree. */
            std::unordered_set<nid_t> id_pool_;  /*!< Pool of leased node ids. */
            std::deque<Node *> it_container_;  /*!< DFS and BFS iteration data structure. */
        };  // class HMITree

        /*!
         * \brief Print a tree to an output stream.
         * 
         * \param out Output stream where node is printed.
         * \param hmi_tree HMITree instance to be printed.
         *
         * \return Modified output stream.
         */
        std::ostream& operator<<(std::ostream& out, const HMITree& hmi_tree);
    }  // namespace tree
}  // namespace hmi_tree_optimization

#endif  // HMI_TREE_OPTIMIZATION__TREE__HMI_TREE_HH

