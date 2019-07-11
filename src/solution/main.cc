#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "solution/caching.hh"
#include "std_helper/split.hh"
#include "tree/hmi_tree.hh"
#include "tree/hmi_objects/hmi_view.hh"
#include "tree/hmi_objects/hmi_widget.hh"
#include "tree/hmi_objects/hmi_text.hh"
#include "tree/hmi_exception.hh"

using namespace hmi_tree_optimization;
using namespace hmi_tree_optimization::solution;
using namespace hmi_tree_optimization::tree;

namespace {
    void add_node_from_csv(const std::string&, HMITree&);
}  // anonymous namespace

int main() {
    srand(time(NULL));  // initialize random seed

    int frame = 1;
    HMITree tree;
    size_t nnodes;
    std::string line;
    nid_t node_id;
    std::vector<std::string> items;

    std::cin >> nnodes;
    std::getline(std::cin, line);  // skip newline char
    for (size_t i = 0; i < nnodes; ++i) {
        std::getline(std::cin, line);
        add_node_from_csv(line, tree);
    }

    while (!std::getline(std::cin, line).eof() &&
            line.compare("end") != 0) {
        if (line.compare("print") == 0) {  // print the current state of the tree
            std::cout << tree << std::endl;
        } else if (line.compare("refresh") == 0) {
            std::cout << ">>>>> Frame " << frame << " <<<<<" << std::endl;
            evaluate_tree_dirtiness(tree);
            refresh_screen(tree);
            ++frame;
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
        } else {
            items = std_helper::split(line, ',');
            node_id = std::stoul(items[0]);
            items.erase(items.begin());
            tree.get_node(node_id).update(items);
        }
    }
    clear_cache();
    return 0;
}

namespace {
    void add_node_from_csv(const std::string& line, HMITree& tree) {
        Node *node;
        std::vector<std::string> items = std_helper::split(line, ',');
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

