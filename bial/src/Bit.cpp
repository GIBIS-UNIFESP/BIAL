/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/27 
 * @brief Bit wise opperations over integer numbers.
 */

#ifndef BIALBIT_C
#define BIALBIT_C

#include "Bit.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Bit )
#define BIAL_EXPLICIT_Bit
#endif

#if defined ( BIAL_EXPLICIT_Bit ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  template< class D >
  D Bit::Invert( D num, size_t bit ) {
    return( Bit::Flip( num, bit ) );
  }

  template< class D >
  D Bit::Set( D num, size_t bit, char val ) {
    COMMENT( "Input number: " << static_cast< unsigned int >( num ) << ", bit: " << bit, 4 );
    if( val == 0 ) {
      return( Reset( num, bit ) );
    }
    return( Preset( num, bit ) );
  }

  template< class D >
  D Bit::Preset( D num, size_t bit ) {
    COMMENT( "Input number: " << static_cast< unsigned int >( num ) << ", bit: " << bit, 4 );
    D msk = 1 << bit;
    return( num | msk );
  }

  template< class D >
  D Bit::Reset( D num, size_t bit ) {
    COMMENT( "Input number: " << static_cast< unsigned int >( num ) << ", bit: " << bit, 4 );
    D not_msk = 1 << bit;
    D msk = ~not_msk;
    return( num & msk );
  }

  template< class D >
  D Bit::Flip( D num, size_t bit ) {
    COMMENT( "Input number: " << static_cast< unsigned int >( num ) << ", bit: " << bit, 4 );
    D msk = 1 << bit;
    return( num ^ msk );
  }

  template< class D >
  bool Bit::IsOne( D num, size_t bit ) {
    COMMENT( "Input number: " << static_cast< unsigned int >( num ) << ", bit: " << bit, 4 );
    D msk = 1 << bit;
    return( ( num & msk ) >> bit == 1 );
  }

  template< class D >
  bool Bit::IsZero( D num, size_t bit ) {
    COMMENT( "Input number: " << static_cast< unsigned int >( num ) << ", bit: " << bit, 4 );
    D msk = 1 << bit;
    return( ( num & msk ) >> bit == 0 );
  }

  template< class D >
  D Bit::Value( D num, size_t bit ) {
    COMMENT( "Input number: " << static_cast< unsigned int >( num ) << ", bit: " << bit, 4 );
    D msk = 1 << bit;
    return( ( num & msk ) >> bit );
  }

#ifdef BIAL_EXPLICIT_Bit

  template int Bit::Invert( int num, size_t bit );
  template int Bit::Set( int num, size_t bit, char val );
  template int Bit::Preset( int num, size_t bit );
  template int Bit::Reset( int num, size_t bit );
  template int Bit::Flip( int num, size_t bit );
  template bool Bit::IsOne( int num, size_t bit );
  template bool Bit::IsZero( int num, size_t bit );
  template int Bit::Value( int num, size_t bit );

  template size_t Bit::Invert( size_t num, size_t bit );
  template size_t Bit::Set( size_t num, size_t bit, char val );
  template size_t Bit::Preset( size_t num, size_t bit );
  template size_t Bit::Reset( size_t num, size_t bit );
  template size_t Bit::Flip( size_t num, size_t bit );
  template bool Bit::IsOne( size_t num, size_t bit );
  template bool Bit::IsZero( size_t num, size_t bit );
  template size_t Bit::Value( size_t num, size_t bit );

  template llint Bit::Invert( llint num, size_t bit );
  template llint Bit::Set( llint num, size_t bit, char val );
  template llint Bit::Preset( llint num, size_t bit );
  template llint Bit::Reset( llint num, size_t bit );
  template llint Bit::Flip( llint num, size_t bit );
  template bool Bit::IsOne( llint num, size_t bit );
  template bool Bit::IsZero( llint num, size_t bit );
  template llint Bit::Value( llint num, size_t bit );

#endif

}

#endif

#endif
