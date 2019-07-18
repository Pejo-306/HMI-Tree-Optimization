#include "heavy_hitters/cms.hh"

#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>

namespace hmi_tree_optimization {
    namespace heavy_hitters {
        CMS::CMS(size_t k, double errpr) noexcept
            : k_(k),
              errpr_(errpr),
              epsilon_(1.0 / (2 * k)),
              nbuckets_(exp(1) / epsilon_),
              nhfuns_(ceil(log(1.0 / errpr))),
              sketch_(nhfuns_, std::vector<size_t>(nbuckets_, 0)),
              p_(generate_random_prime()),
              hfuns_(generate_hash_functions()) {
        }

        size_t CMS::count(int x) noexcept {
            size_t min_count = std::numeric_limits<size_t>::max();

            for (size_t l = 0; l < nhfuns_; ++l)
                min_count = std::min(sketch_[l][hfuns_[l](x)], min_count);
            return min_count;
        }

        CMS& CMS::increment(int x) noexcept {
            for (size_t l = 0; l < nhfuns_; ++l)
                ++sketch_[l][hfuns_[l](x)];
            return *this;
        }

        CMS& CMS::print(std::ostream& out) noexcept {
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
                    out << sketch_[l][b];
                out << std::endl;
            }
            return *this;
        }

        std::vector<std::function<size_t(int)>> CMS::generate_hash_functions() noexcept {
            std::vector<std::function<size_t(int)>> hfuns;
            uint32_t a;
            uint32_t b;

            for (size_t l = 0; l < nhfuns_; ++l) {
                a = rand() % (p_ - 2) + 1;
                b = rand() % (p_ - 1);
                hfuns.push_back([a, b, this](int x) { return ((a * x + b) % p_) % nbuckets_; });
            }
            return hfuns;
        }

        uint32_t CMS::generate_random_prime() noexcept {
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
    }  // namespace heavy_hitters
}  // namespace hmi_tree_optimization
