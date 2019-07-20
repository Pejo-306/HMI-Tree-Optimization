#ifndef HMI_TREE_OPTIMIZATION__HEAVY_HITTERS__CMS_HH
#define HMI_TREE_OPTIMIZATION__HEAVY_HITTERS__CMS_HH

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <functional>

namespace hmi_tree_optimization {
    namespace heavy_hitters {
        template<typename T>
        class CMS final {
        public:
            explicit CMS(size_t k, double errpr) noexcept
                : k_(k),
                  errpr_(errpr),
                  epsilon_(1.0 / (2 * k)),
                  nbuckets_(exp(1) / epsilon_),
                  nhfuns_(ceil(log(1.0 / errpr))),
                  sketch_(nhfuns_, std::vector<size_t>(nbuckets_, 0)),
                  p_(generate_random_prime()),
                  hfuns_(generate_hash_functions()) {
            }

            size_t count(const T& x) noexcept {
                size_t min_count = std::numeric_limits<size_t>::max();

                for (size_t l = 0; l < nhfuns_; ++l)
                    min_count = std::min(sketch_[l][hfuns_[l](x)], min_count);
                return min_count;
            }

            CMS& increment(const T& x) noexcept {
                for (size_t l = 0; l < nhfuns_; ++l)
                    ++sketch_[l][hfuns_[l](x)];
                return *this;
            }

            CMS& print(std::ostream& out = std::cout) noexcept {
                out << "CMS{"
                    << "k=" << k_
                    << ", e=" << epsilon_
                    << ", l=" << nhfuns_
                    << ", b=" << nbuckets_
                    << ", d=" << errpr_
                    << ", p=" << p_
                    << "}" << std::endl;
                for (size_t l = 0; l < nhfuns_; ++l) {
                    for (size_t b = 0; b < nbuckets_; ++b) 
                        out << sketch_[l][b] << '|';
                    out << std::endl;
                }
                return *this;
            }

        private:
            const size_t k_;
            const double errpr_;  // target error probability
            const double epsilon_;  // additive factor
            const size_t nbuckets_;
            const size_t nhfuns_;
            std::vector<std::vector<size_t>> sketch_;
            const uint32_t p_;
            const std::vector<std::function<size_t(const T&)>> hfuns_;

            std::vector<std::function<size_t(const T&)>> generate_hash_functions() noexcept {
                std::vector<std::function<size_t(const T&)>> hfuns;
                uint32_t a;
                uint32_t b;

                for (size_t l = 0; l < nhfuns_; ++l) {
                    a = rand() % (p_ - 2) + 1;
                    b = rand() % (p_ - 1);
                    hfuns.push_back([a, b, this](const T& x) { 
                            return ((a * x + b) % p_) % nbuckets_; 
                            });
                }
                return hfuns;
            }

            uint32_t generate_random_prime() noexcept {
                static auto is_prime = [](uint32_t x) {
                    for (size_t i = 2; i < floor(sqrt(x)); ++i)
                        if (x % i == 0)
                            return false;
                    return true;
                };
                uint32_t res;

                do
                    res = rand() % (std::numeric_limits<uint32_t>::max() - 1) + 2;
                while (!is_prime(res));
                return res;
            }
        };  // class CMS
    }  // namespace heavy_hitters
}  // namespace hmi_tree_optimization

#endif

