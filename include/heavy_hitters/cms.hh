#ifndef HMI_TREE_OPTIMIZATION__HEAVY_HITTERS__CMS_HH
#define HMI_TREE_OPTIMIZATION__HEAVY_HITTERS__CMS_HH

#include <cstdint>
#include <iostream>
#include <vector>
#include <functional>

namespace hmi_tree_optimization {
    namespace heavy_hitters {
        class CMS final {
        public:
            explicit CMS(size_t, double) noexcept;
            size_t count(int) noexcept;
            CMS& increment(int) noexcept;
            CMS& print(std::ostream& = std::cout) noexcept;
        private:
            const size_t k_;
            const double errpr_;  // target error probability
            const double epsilon_;  // additive factor
            const size_t nbuckets_;
            const size_t nhfuns_;
            std::vector<std::vector<size_t>> sketch_;
            const uint32_t p_;
            const std::vector<std::function<size_t(int)>> hfuns_;
            std::vector<std::function<size_t(int)>> generate_hash_functions() noexcept;
            uint32_t generate_random_prime() noexcept;
        };  // class CMS
    }  // namespace heavy_hitters
}  // namespace hmi_tree_optimization

#endif

