/*!
 * \file src/solution/main.cc
 * \brief Main program function.
 *
 * The program is a stripped-down simulation of an HMI panel. The main
 * function is responsible for building the simulated HMI tree datastructure,
 * defining a simulated runtime environment of an HMI panel, as well as 
 * implementing the approximate Heavy Hitters algorithm to dynamically determine
 * nodes to be cached, nodes to be loaded from cache and nodes to be updated
 * and rerendered.
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

#include "solution/config.hh"
#include "solution/caching.hh"
#include "std_helper/split.hh"
#include "tree/hmi_tree.hh"
#include "tree/hmi_objects/hmi_view.hh"
#include "tree/hmi_objects/hmi_widget.hh"
#include "tree/hmi_objects/hmi_text.hh"
#include "tree/hmi_exception.hh"
#include "heavy_hitters/cms.hh"

using namespace hmi_tree_optimization;
using namespace hmi_tree_optimization::solution;
using namespace hmi_tree_optimization::tree;

namespace {
    void add_node_from_csv(const std::string&, HMITree&);
}  // anonymous namespace

/*!
 * \brief Main program function
 *
 * The main function is responsible for handling all user input (both 
 * command-line arguments and the standard input).
 * Command-line arguments are used to set several program settings. The
 * program must be run in the following way (assuming the user runs the program
 * from the project's root directory):
 *
 * \code
 * ./bin/solution {debug} {k} {errpr} {leeway}
 * \endcode
 *
 * where:
 *   - \a debug: indicate whether debug messages should be printed; value is
 *   boolean (either 0 or 1);
 *   - \a k: the maximum amount of heavy hitters that can be present in the input
 *   stream; value is a positive integer;
 *   - \a errpr: the target error probability when approximating frequency count
 *   of input items; value is between 0 and 1 (between 0% and 100%);
 *   - \a leeway: how much the frequency count of a heavy hitter may drop below the
 *   heavy hitter threshold (m / k); value is between 0 and 1 (between 0% 
 *   and 100%).
 *
 * example (using one of the provided exemplary input files):
 *
 * \code
 * ./bin/solution 1 3 0.01 0.01 < tmp/inputs/input0.txt
 * \endcode
 *
 * \warning If the user does not provided the required arguments the function <!--
 * --> immediately terminates with exit status '-1'.
 *
 * Afterwards, the user proceeds to define the simulated HMI tree on which the
 * optimization algorithm is executed. The tree is described via the standard
 * input and must follow the following format:
 *
 * \code
 * {n}
 * {pnid},{type},{nid},[arg1, arg2...]
 * <input omitted: (another n-1 lines as the above)>
 * \endcode
 *
 * where:
 *   - \a n: number of tree nodes;
 *   - \a pnid: parent id of an inputted node;
 *   - \a type: Type of HMI tree node (Widget, Text...);
 *   - \a nid: id of inputted node;
 *   - [\a arg1, \a arg2...]: additional argumentss (depends on the \a type 
 *   of this HMI tree node).
 *
 * As shown above, the user must first specify the number of nodes that the
 * HMI tree has (excluding the root View node). Then each node must be defined
 * by one line via its parent node's id, its type, its own node id and with
 * additional arguments (which depend on the HMI tree node type). The node's
 * information is presented in CSV format.
 * \note The HMI tree's root node (which is always an HMIView) should not be <!--
 * --> explicitly specified in the user's input. It always has an id of 0.
 *
 * Afterwards the simulated program starts. The user is to continuously input
 * single line commands on the standard input. The available commands are
 * the following:
 * - \b print: print the current state of the HMI tree (highlights the dirty
 *   and very dirty nodes);
 * - \b refresh: simulate a frame refresh, i.e. it resets the dirtiness state
 *   of each node and runs the optimization algorithm. The latter determines
 *   which nodes are rerendered, which may be cached and which are loaded from
 *   the cache;
 * - {\a nid},[\a arg1, \a arg2,...]: updates the node with id \a nid. Any
 *   additional arguments are parsed to the node's update procedure;
 * - \b end: stops the simulation program.
 *
 * \note For exemplary input check the text files in \b tmp/inputs/.
 *
 * This function utilizes the count-min sketch to implement the approximate
 * Heavy Hitters algorithm. Furthermore, it uses two other data structures:
 * a priority queue (\b heavy_hitters_pq) and an unordered set/hash set 
 * (\b heavy_hitters_set).\n
 * The variable \b m stores the size of the input stream, required by the
 * Heavy Hitters algorithm. It is only incremented once for each dirty node
 * when refreshing the frame, i.e. it does not matter how many times a node
 * is updated between two frames - the algorithm treats this as one update.
 * Therefore, between two frames, a node's frequency count in the CMS can
 * only be incremented once.\n 
 * The size of the input stream is used to evaluate
 * each node and determine the heavy hitters (i.e. all nodes which occur more
 * frequently than \b m/k). All heavy hitters are added both to the set and
 * the priority queue.\n
 * Nodes are no longer considered heavy hitters once their frequency count
 * falls below \b ((1+leeway)*m)/k. They are removed both from
 * the set and the priority queue.\n
 * The set of all heavy hitters is then used to determine which nodes are suitable
 * for caching.
 *
 * \see evaluate_tree_dirtiness()
 * \see refresh_screen()
 * \see hmi_tree_optimization::tree::HMITree
 * \see hmi_tree_optimization::heavy_hitters::CMS
 */
int main(int argc, char *argv[]) {
    srand(time(NULL));  // initialize random seed
    if (argc < 5) 
        return -1;
    g_debug = std::stoi(argv[1]);

    int frame = 1;
    HMITree tree;
    size_t nnodes;
    std::string line;
    nid_t node_id;
    nid_t hitter;
    std::vector<std::string> items;
    size_t m = 0;  // current size of input stream
    size_t k = std::stoul(argv[2]);
    double errpr = std::stod(argv[3]);
    double leeway = std::stod(argv[4]);  // in %
    heavy_hitters::CMS<nid_t> cms(k, errpr);
    auto cmp = [&cms](nid_t left, nid_t right) {
        return cms.count(left) - cms.count(right);
    };
    std::priority_queue<nid_t, std::vector<nid_t>, decltype(cmp)> heavy_hitters_pq(cmp);
    std::unordered_set<nid_t> heavy_hitters_set;

    // define the simulated HMI tree
    std::cin >> nnodes;
    std::getline(std::cin, line);  // skip newline char
    for (size_t i = 0; i < nnodes; ++i) {
        std::getline(std::cin, line);
        add_node_from_csv(line, tree);
    }

    // simulation program
    while (!std::getline(std::cin, line).eof()
            && line.compare("end") != 0) {
        if (line.compare("print") == 0) {  // print the current state of the tree
            std::cout << tree << std::endl;
        } else if (line.compare("refresh") == 0) {  // refresh the frame
            if (g_debug)
                std::cout << ">>>>> Frame " << frame << " <<<<<" << std::endl;
            evaluate_tree_dirtiness(tree, heavy_hitters_set);
            refresh_screen(tree);
            ++frame;
            if (g_debug) {
                std::cout << ">>>>> HMI Tree <<<<<" << std::endl;
                for (HMITree::bfs_iterator it = tree.bfs_begin(); it != tree.bfs_end(); ++it)
                    std::cout << it->nall_children() << '|'
                        << it->is_very_dirty() << ' '
                        << it->repr() << std::endl;
                std::cout << ">>>>> Cache Table <<<<<" << std::endl;
                for (auto& entry : g_cache_table)
                    std::cout << "NID: " << entry.first
                        << " | Entry: " << entry.second->repr() 
                        << std::endl;
                std::cout << std::endl;
            }
        } else {  // update a node
            items = std_helper::split(line, ",");
            node_id = std::stoul(items[0]);
            items.erase(items.begin());

            if (!tree.get_node(node_id).is_dirty()) {
                hitter = node_id;
                cms.increment(hitter);
                ++m;
                // add new heavy hitters
                if (cms.count(hitter) >= static_cast<double>(m) / k
                        && heavy_hitters_set.find(hitter) == heavy_hitters_set.end()) {
                    heavy_hitters_pq.push(hitter);
                    heavy_hitters_set.insert(hitter);
                }
                // remove irrelevant nodes which are no longer heavy hitters
                while (!heavy_hitters_pq.empty()) {
                    hitter = heavy_hitters_pq.top();
                    if (cms.count(hitter) < ((1 + leeway) * m) / k) {
                        heavy_hitters_pq.pop();
                        heavy_hitters_set.erase(hitter);
                    } else {
                        break;
                    }
                }
            }

            tree.get_node(node_id).update(items);
        }
    }

    // clean up and final debug messages
    clear_cache();
    if (g_debug) {
        std::cout << ">>>>> CMS <<<<<" << std::endl;
        cms.print();
        std::cout << ">>>>> Heavy hitters <<<<<" << std::endl;
        while (!heavy_hitters_pq.empty()) {
            node_id = heavy_hitters_pq.top();
            std::cout << node_id << ' ';
            heavy_hitters_pq.pop();
            heavy_hitters_set.erase(node_id);
        }
        std::cout << std::endl;
    }
    return 0;
}

namespace {
    /*!
     * \brief Create an HMI tree node from a line of input
     *
     * That line of input must contain all relevant information to an HMI
     * tree node in CSV format.
     *
     * \see hmi_tree_optimization::std_helper::split()
     * \see hmi_tree_optimization::tree::HMIWidget
     * \see hmi_tree_optimization::tree::HMIText
     * \see hmi_tree_optimization::tree::HMITree
     *
     * \note For exemplary input check the text files in \b tmp/inputs/.
     */
    void add_node_from_csv(const std::string& line, HMITree& tree) {
        Node *node;
        std::vector<std::string> items = std_helper::split(line, ",");
        nid_t parent_id = std::stoul(items[0]);
        const std::string& type = items[1];
        nid_t node_id = std::stoul(items[2]);

        if (type.compare("W") == 0) {
            node = new HMIWidget(node_id);
        } else if (type.compare("T") == 0) {
            const std::string& content = items[3];

            node = new HMIText(node_id, content);
        } else {
            throw HMIException(nullptr);
        }
        tree.add_node(parent_id, node);
    }
}  // anonymous namespace

