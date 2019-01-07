/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/19 
 * @brief Diffusion functions to compute annisotropic filtering and other algorithms. 
 */

#ifndef BIALDIFFUSIONFUNCTION_C
#define BIALDIFFUSIONFUNCTION_C

#include "DiffusionFunction.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DiffusionFunction )
#define BIAL_EXPLICIT_DiffusionFunction
#endif

#if defined ( BIAL_EXPLICIT_DiffusionFunction ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  /* Power Diffusion Function *****************************************************************************************
  **/

  PowerDiffusionFunction::PowerDiffusionFunction( float new_power ) : power( new_power ) {
    if( ( power < 1.0 ) || ( power > 4.0 ) ) {
      std::string msg( BIAL_ERROR( "Power must be in range: [1.0, 4.0]. Given: " + std::to_string( power ) ) );
      throw( std::logic_error( msg ) );
    }
  }

  float PowerDiffusionFunction::operator()( float kappa, float grad ) const {
    return( 1.0 / ( 1.0 + pow( grad / kappa, 1.0 + power ) ) );
  }

  float PowerDiffusionFunction::Reduction( float kappa ) const {
    return( 0.375 * kappa );
  }

  /* Gaussian Diffusion Function **************************************************************************************/

  float GaussianDiffusionFunction::operator()( float kappa, float grad ) const {
    return( exp( 0.5 * -pow( grad / kappa, 2.0 ) ) );
  }

  float GaussianDiffusionFunction::Reduction( float kappa ) const {
    return( 0.75 * kappa * std::exp( -0.5 ) );
  }

  /* Robust Diffusion Function ****************************************************************************************/

  float RobustDiffusionFunction::operator()( float kappa, float grad ) const {
    float flow = grad / ( kappa * _sqrt_5_ );
    flow *= flow;
    flow = std::max( 0.0f, 1.0f - flow );
    return( flow * flow );
  }

  float RobustDiffusionFunction::Reduction( float kappa ) const {
    return( 0.64 * kappa ); // Maximum reduction: 0.64 kappa. 3/4 of that is 0.48 kappa
  }

}

#endif

#endif
