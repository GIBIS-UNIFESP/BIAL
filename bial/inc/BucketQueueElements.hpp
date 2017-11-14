/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file BucketQueue.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2012/Sep/06 
 * @version 1.0.00
 * @brief
 * Content: Bucket-sort queue class 
 * <br> Description: A bucket-sort queue. It is useful to sort a large set
 * of elements whose weight domain is not very large,
 * e.g. IFT and OPF algorithms over MR images. 
 * <br> Future add-on's: Deal with negative values. 
 *                  Add fast operation functions in which queue does not grow. 
 * Warning: In Boost library, there is a class named circular_buffer
 * that may do exactly the same as Vector< WeightNode >. 
 */

#ifndef BIALBUCKETQUEUEELEMENTS_H
#define BIALBUCKETQUEUEELEMENTS_H

#include "Common.hpp"

namespace Bial {

  enum class BucketState : char {
    NOT_VISITED = 0,
      INSERTED = 1,
      UPDATED = 2,
      REMOVED = 3
      };

  class IdentityNode {
  public:
    /** @brief  next node */
    int next;
    /** @brief  prev node */
    int prev;
    BucketState state;
    IdentityNode( ) {
      next = -1;
      prev = -1;
      state = BucketState::NOT_VISITED;
    }
  };

  class WeightNode {
  public:
    int first; /* first node */
    int last; /* last node */
    WeightNode( ) {
      first = -1;
      last = -1;
    }
  };

}

#endif
