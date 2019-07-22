/*!
 * \file include/heavy_hitters/cms.hh
 * \brief Define count-min sketch to solve the approximate Heavy Hitters problem
 *
 * The Heavy Hitters problem is described as follows: given an input sequence
 * (such as a stream) of size 'n' and a parameter 'k', find the elements which occur
 * at least 'n/k' times in the sequence. There can be at most 'k' such elements.
 *
 * It is assumed that the input sequence is very large (in the order of millions
 * or billions) and 'k' is a modest value (10-1000). Since the Heavy Hitters problem
 * requires linear space to solve, the approximate Heavy Hitters problem is
 * instead solved via the count-min sketch defined below. The latter uses a
 * sublinear amount of memory to approximately find the most frequent elements.
 *
 * \author Petar Nikolov
 * \version v1.0.0
 */

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

//! \namespace hmi_tree_optimization
//! \brief Project namespace
namespace hmi_tree_optimization {
    //! \namespace heavy_hitters
    //! \brief Defines data structure and the required functionality to solve the Heavy Hitters problem
    namespace heavy_hitters {
        /*!
         * \class CMS
         *
         * \brief Stores the approximate frequency count of an element of an input stream
         *
         * The count-min sketch is a small-space data structure which is used
         * to solve the approximate Heavy Hitters problem. It has 'b' buckets
         * and 'l' hash functions of the same universal family and supports
         * two main operations: increment and count. The 'increment' operation
         * hashes a given element with each hash function and increments the
         * counter located at the function's resulting bucket. Since the amount
         * of space utilized is sublinear and does not depend on the size of the
         * input stream, collisions may occur when counting elements. However,
         * notice that this data structure only overestimates the frequency 
         * of an element. This is why the 'count' operation returns the minimum 
         * count of each bucket where a given element's counter is located.
         *
         * \see http://timroughgarden.org/s17/l/l2.pdf
         *
         * \tparam T Type of counted numeric elements.
         *
         * \note The template parameter T must be a numeric type.
         */
        template<typename T>
        class CMS final {

        public:

            /*!
             * \brief Constructor for count-min sketch
             *
             * The constructor calculates and initializes all internal parameters,
             * utilized by the sketch.
             *
             * \param k Maximum amount of heavy hitters.
             * \param errpr The target counting error probability.
             */
            explicit CMS(size_t k, double errpr) noexcept
                : k_(k),
                  errpr_(errpr),
                  epsilon_(1.0 / (2 * k)),
                  nbuckets_(exp(1) / epsilon_),
                  nhfuns_(ceil(log(1.0 / errpr))),
                  sketch_(nhfuns_, std::vector<size_t>(nbuckets_, 0)),
                  hfuns_(generate_hash_functions()) {
            }

            /*!
             * \brief Return the approximate frequency count of an element
             *
             * This sketch operation finds the approximate frequency count of
             * a given element by hashing the latter and retrieving the min
             * counter, associated with the parsed element.
             *
             * \param x Element whose count is required.
             * \return Approximate frequency count of an element.
             */
            size_t count(const T& x) noexcept {
                size_t min_count = std::numeric_limits<size_t>::max();

                for (size_t l = 0; l < nhfuns_; ++l)
                    min_count = std::min(sketch_[l][hfuns_[l](x)], min_count);
                return min_count;
            }

            /*!
             * \brief Increment all counters, associated with a given element
             *
             * \param x Element whose counters are incremented.
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            CMS& increment(const T& x) noexcept {
                for (size_t l = 0; l < nhfuns_; ++l)
                    ++sketch_[l][hfuns_[l](x)];
                return *this;
            }

            /*!
             * \brief Print this sketch to an output stream.
             *
             * \param out Output stream where the sketch is printed.
             * \return Reference to this instance.
             *
             * \remark The returned reference to this may be used for method chaining.
             */
            CMS& print(std::ostream& out = std::cout) noexcept {
                out << "CMS{"
                    << "k=" << k_
                    << ", e=" << epsilon_
                    << ", l=" << nhfuns_
                    << ", b=" << nbuckets_
                    << ", d=" << errpr_
                    << "}" << std::endl;
                for (size_t l = 0; l < nhfuns_; ++l) {
                    for (size_t b = 0; b < nbuckets_; ++b) 
                        out << sketch_[l][b] << '|';
                    out << std::endl;
                }
                return *this;
            }

        private:

            const size_t k_;  /*!< Maximum amount of heavy hitters. */
            const double errpr_;  /*!< Target counting error probability. */
            const double epsilon_;  /*!< Additive factor. */
            const size_t nbuckets_;  /*!< Number of buckets. */
            const size_t nhfuns_;  /*!< Number of hashing functions. */
            std::vector<std::vector<size_t>> sketch_;  /*!< 2D sketch array. */
            const std::vector<std::function<size_t(const T&)>> hfuns_;  /*!< Universal hashing functions. */

            /*!
             * \brief Generate a set of hash functions from the same universal family.
             *
             * All generated hashing functions can be used only with elements of
             * a numeric type as all functions are of the form:
             *     ((ax + b) mod p) mod b
             * where:
             *     a - random 32-bit positive number; cannot be 0;
             *     b - random 32-bit positive number;
             *     p - random 32-bit prime number; must be the same for all 
             *     generated functions;
             *     b - number of buckets in the sketch.
             *
             * \remark All hashing functions are C++ lambda functions.
             *
             * \return Collection of generated hashing lambdas.
             */
            std::vector<std::function<size_t(const T&)>> generate_hash_functions() noexcept {
                std::vector<std::function<size_t(const T&)>> hfuns;
                uint32_t a;
                uint32_t b;
                uint32_t p = generate_random_prime();

                for (size_t l = 0; l < nhfuns_; ++l) {
                    a = rand() % (p - 2) + 1;
                    b = rand() % (p - 1);
                    hfuns.push_back([a, b, p, this](const T& x) { 
                            return ((a * x + b) % p) % nbuckets_; 
                            });
                }
                return hfuns;
            }

            /*!
             * \brief Generate a random prime number.
             *
             * \return Random 32-bit prime number.
             */
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

#endif  // HMI_TREE_OPTIMIZATION__HEAVY_HITTERS__CMS_HH

