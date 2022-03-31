#ifndef RANDOM_WALK_HPP
#define RANDOM_WALK_HPP

// #include <cmath>

#include "Kokkos_Core.hpp"
#include "Kokkos_Random.hpp"
#include "containers.hpp"
#include "random.hpp"
#include "type_defs.hpp"

namespace particles {

// functor for random-walking particles, templated on the GeneratorPool type
// Note: RandPoolType is currently hard-coded in the particle class
// GeneratorPool type
template <class RandPool>
struct RandomWalk {
  // Output View for the random numbers
  ko::View<Real*> pvec;

  // The GeneratorPool
  RandPool rand_pool;

  typedef Real Scalar;
  typedef typename RandPool::generator_type gen_type;

  // mean and std deviation of the normal random variable
  Scalar mean, stddev;

  KOKKOS_INLINE_FUNCTION
  void operator()(int i) const {
    // Get a random number state from the pool for the active thread
    gen_type rgen = rand_pool.get_state();

    // draw random normal numbers, with mean and std deviation provided
    pvec(i) = pvec(i) + rgen.normal(mean, stddev);

    // Give the state back, which will allow another thread to acquire it
    rand_pool.free_state(rgen);
  }

  // Constructor, Initialize all members
  RandomWalk(ko::View<Real*> pvec_, const RandPool& rand_pool_,
             const Scalar& mean_, const Scalar& stddev_)
      : pvec(pvec_), rand_pool(rand_pool_), mean(mean_), stddev(stddev_) {}

};  // end RandomWalk functor

void random_walk(ko::View<Real*>& X, const Params& params,
                 RandPoolType& rand_pool);

}  // namespace particles

#endif
