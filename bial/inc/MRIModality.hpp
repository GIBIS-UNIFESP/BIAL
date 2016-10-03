/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2014/Apr/16 
 * @brief class used to discriminate MRI modality.
 */

#ifndef BIALMRIModality_H
#define BIALMRIModality_H
#include "Common.hpp"

namespace Bial {

  /** @brief  MRI modality */
  enum class MRIModality : char {
    UNKNOWN = 0,
      T1 = 1,
      T2 = 2,
      PD = 3
      /* others */
      };

}

#endif
