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

    std::cin >> nnodes;
    std::getline(std::cin, line);  // skip newline char
    for (size_t i = 0; i < nnodes; ++i) {
        std::getline(std::cin, line);
        add_node_from_csv(line, tree);
    }

    while (!std::getline(std::cin, line).eof()
            && line.compare("end") != 0) {
        if (line.compare("print") == 0) {  // print the current state of the tree
            std::cout << tree << std::endl;
        } else if (line.compare("refresh") == 0) {
            if (g_debug)
                std::cout << ">>>>> Frame " << frame << " <<<<<" << std::endl;
            evaluate_tree_dirtiness(tree);
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
        } else {
            items = std_helper::split(line, ',');
            node_id = std::stoul(items[0]);
            items.erase(items.begin());

            if (!tree.get_node(node_id).is_dirty()) {
                hitter = node_id;
                cms.increment(hitter);
                ++m;
                if (cms.count(hitter) >= static_cast<double>(m) / k
                        && heavy_hitters_set.find(hitter) == heavy_hitters_set.end()) {
                    heavy_hitters_pq.push(hitter);
                    heavy_hitters_set.insert(hitter);
                }
                while (!heavy_hitters_pq.empty()) {
                    hitter = heavy_hitters_pq.top();
                    if (cms.count(hitter) -  leeway * m < static_cast<double>(m) / k) {
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

