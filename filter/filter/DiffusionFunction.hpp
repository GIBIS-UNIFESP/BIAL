/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/19 
 * @brief Diffusion functions to compute annisotropic filtering and other algorithms. 
 */

#ifndef BIALDIFFUSIONFUNCTION_H
#define BIALDIFFUSIONFUNCTION_H
#include "Common.hpp"

namespace Bial {

  /** @brief Diffusion functions to compute annisotropic
   * filtering and other algorithms.
   */
  class DiffusionFunction {

  public:

    const float _sqrt_5_ = 2.236067977f;

    /**
     * @date 2013/Nov/19 
     * @param none. 
     * @return none. 
     * @brief Virtual destructor required for virtual
     * classes. 
     * @warning none. 
     */
    virtual ~DiffusionFunction( ) {
    }

    /**
     * @date 2013/Nov/18 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @param grad: gradient value. 
     * @return The diffusion value of the given gradient. 
     * @brief Returns the diffusion value of the given
     * gradient. 
     * @warning none 
     */
    virtual float operator()( float kappa, float grad ) const = 0;

    /**
     * @date 2013/Nov/29 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @return Value of new kappa after reduction. 
     * @brief Computes the value of new kappa after reduction. 
     * @warning none 
     */
    virtual float Reduction( float kappa ) const = 0;

  };

  /* Power Diffusion Function ************************************************************************************** */

  class PowerDiffusionFunction : public DiffusionFunction {

  protected:

    float power; /* the power of the equation. Range: [1.0, 4.0] */

  public:

    /**
     * @date 2013/Nov/18 
     * @param new_power: the power of the equation. Range: [1.0, 4.0] 
     * @return none. 
     * @brief Basic constructor. 
     * @warning none. 
     */
    PowerDiffusionFunction( float new_power = 1.0 );

    /**
     * @date 2013/Nov/18 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @param grad: gradient value. 
     * @return The diffusion value of the given gradient. 
     * @brief Returns the diffusion value of the given
     * gradient. 
     * @warning none 
     */
    float operator()( float kappa, float grad ) const;

    /**
     * @date 2013/Nov/29 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @return Value of new kappa after reduction. 
     * @brief Computes the value of new kappa after reduction. 
     * @warning none 
     */
    float Reduction( float kappa ) const;

  };

  /* Gaussian Diffusion Function ************************************************************************************ */

  class GaussianDiffusionFunction : public DiffusionFunction {

  public:

    /**
     * @date 2013/Nov/18 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @param grad: gradient value. 
     * @return The diffusion value of the given gradient. 
     * @brief Returns the diffusion value of the given
     * gradient. 
     * @warning none 
     */
    float operator()( float kappa, float grad ) const;

    /**
     * @date 2013/Nov/29 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @return Value of new kappa after reduction. 
     * @brief Computes the value of new kappa after reduction. 
     * @warning none 
     */
    float Reduction( float kappa ) const;

  };

  /* Robust Diffusion Function ************************************************************************************** */

  class RobustDiffusionFunction : public DiffusionFunction {

  protected:

  public:

    /**
     * @date 2013/Nov/18 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @param grad: gradient value. 
     * @return The diffusion value of the given gradient. 
     * @brief Returns the diffusion value of the given
     * gradient. 
     * @warning none 
     */
    float operator()( float kappa, float grad ) const;

    /**
     * @date 2013/Nov/29 
     * @param kappa: constant to control the gradient range to be filtered. 
     * @return Value of new kappa after reduction. 
     * @brief Computes the value of new kappa after reduction. 
     * @warning none 
     */
    float Reduction( float kappa ) const;

  };

}

#endif
