#ifndef HMI_TREE_OPTIMIZATION__SOLUTION__CACHING_HH
#define HMI_TREE_OPTIMIZATION__SOLUTION__CACHING_HH

#include "tree/hmi_tree.hh"

namespace hmi_tree_optimization {
    namespace solution {
        void evaluate_tree_dirtiness(tree::HMITree&);
        void refresh_screen(tree::HMITree&);
    }  // namespace solution
}  // namespace hmi_tree_optimization

#endif

