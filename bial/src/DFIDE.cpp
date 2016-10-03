/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * Content: Distance function interface for generic algorithm implementation.
 */

#ifndef BIALDFIDE_C
#define BIALDFIDE_C

#include "DFIDE.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DFIDE )
#define BIAL_EXPLICIT_DFIDE
#endif

#if defined ( BIAL_EXPLICIT_DFIDE ) || ( BIAL_IMPLICIT_BIN )

#include "ChessBoardDistanceFunction.hpp"
#include "CityBlockDistanceFunction.hpp"
#include "EuclideanDistanceFunction.hpp"
#include "Feature.hpp"
#include "Image.hpp"
#include "PreEuclideanDistanceFunction.hpp"
#include "SquareEuclideanDistanceFunction.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  double DFIDE::Distance( const C< D > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    switch( distance_function_type ) {
        case DistanceFunctionType::Euclidean: {
        return( EuclideanDistanceFunction::Distance< D >( src.data( ) + orig, dms ) );
      }
        case DistanceFunctionType::PreEuclidean: {
        return( PreEuclideanDistanceFunction::Distance< D >( src.data( ) + orig, dms ) );
      }
        case DistanceFunctionType::SquareEuclidean: {
        return( SquareEuclideanDistanceFunction::Distance< D >( src.data( ) + orig, dms ) );
      }
        case DistanceFunctionType::CityBlock: {
        return( CityBlockDistanceFunction::Distance< D >( src.data( ) + orig, dms ) );
      }
        case DistanceFunctionType::ChessBoard:
        return( ChessBoardDistanceFunction::Distance< D >( src.data( ) + orig, dms ) );
    }
    return( 0.0 );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< char > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *CharDistancePtr )( src.data( ) + orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< uchar > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *UCharDistancePtr )( src.data( ) + orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< short > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *ShortDistancePtr )( src.data( ) + orig, dms ) );
  }

/*
 *   template< template< class D > class C >
 *   double DFIDE::Distance( const C< unsigned short > &src, const size_t orig, const size_t dms ) {
 * #ifdef BIAL_DEBUG
 *     if( src.size( ) < orig + dms ) {
 *       std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
 *                               ". Maximum index given: " + std::to_string( orig + dms ) ) );
 *       throw( std::out_of_range( msg ) );
 *     }
 * #endif
 *     return( ( *UShortDistancePtr )( src.data( ) + orig, dms ) );
 *   }
 */

  template< template< class D > class C >
  double DFIDE::Distance( const C< int > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *IntDistancePtr )( src.data( ) + orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< unsigned int > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *UIntDistancePtr )( src.data( ) + orig, dms ) );
  }

/*
 *   template< template< class D > class C >
 *   double DFIDE::Distance( const C< long > &src, const size_t orig, const size_t dms ) {
 * #ifdef BIAL_DEBUG
 *     if( src.size( ) < orig + dms ) {
 *       std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
 *                               ". Maximum index given: " + std::to_string( orig + dms ) ) );
 *       throw( std::out_of_range( msg ) );
 *     }
 * #endif
 *     return( ( *LongDistancePtr )( src.data( ) + orig, dms ) );
 *   }
 */

  template< template< class D > class C >
  double DFIDE::Distance( const C< ulint > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *ULongDistancePtr )( src.data( ) + orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< llint > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *LLongDistancePtr )( src.data( ) + orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< ullint > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *ULLongDistancePtr )( src.data( ) + orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< float > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *FloatDistancePtr )( src.data( ) + orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< double > &src, const size_t orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( src.size( ) < orig + dms ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum index given: " + std::to_string( orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DoubleDistancePtr )( src.data( ) + orig, dms ) );
  }

  template< template< class D > class C, class D >
  double DFIDE::Distance( const C< D > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    switch( distance_function_type ) {
        case DistanceFunctionType::Euclidean: {
        return( EuclideanDistanceFunction::Distance< D >( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
      }
        case DistanceFunctionType::PreEuclidean: {
        return( PreEuclideanDistanceFunction::Distance< D >( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
      }
        case DistanceFunctionType::SquareEuclidean: {
        return( SquareEuclideanDistanceFunction::Distance< D >( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
      }
        case DistanceFunctionType::CityBlock: {
        return( CityBlockDistanceFunction::Distance< D >( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
      }
        case DistanceFunctionType::ChessBoard:
        return( ChessBoardDistanceFunction::Distance< D >( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
    }
    return( 0.0 );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< char > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *CharDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< uchar > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *UCharDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< short > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *ShortDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

/*
 *   template< template< class D > class C >
 *     double DFIDE::Distance( const C< unsigned short > &src, const size_t src_orig, const size_t tgt_orig,
 *                             const size_t dms ) {
 * #ifdef BIAL_DEBUG
 *     if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
 *       std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
 *                               ". Maximum source index given: " + std::to_string( src_orig + dms ) +
 *                               ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
 *       throw( std::out_of_range( msg ) );
 *     }
 * #endif
 *     return( ( *UShortDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
 *   }
 */

  template< template< class D > class C >
  double DFIDE::Distance( const C< int > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *IntDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< unsigned int > &src, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *UIntDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

/*
 *   template< template< class D > class C >
 *   double DFIDE::Distance( const C< long > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
 * #ifdef BIAL_DEBUG
 *     if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
 *       std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
 *                               ". Maximum source index given: " + std::to_string( src_orig + dms ) +
 *                               ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
 *       throw( std::out_of_range( msg ) );
 *     }
 * #endif
 *     return( ( *DFIDE::LongDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
 *     //return( 0.0 );
 *   }
 */

  template< template< class D > class C >
  double DFIDE::Distance( const C< ulint > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::ULongDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
    /* return( 0.0 ); */
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< llint > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *LLongDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< ullint > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *ULLongDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< float > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *FloatDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< double > &src, const size_t src_orig, const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DoubleDistancePtrTgt )( src.data( ) + src_orig, src.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C, class D >
  double DFIDE::Distance( const C< D > &src, const C< D > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( tgt.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum source index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index: " + std::to_string( tgt.size( ) - 1 ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    switch( distance_function_type ) {
        case DistanceFunctionType::Euclidean: {
        return( EuclideanDistanceFunction::Distance< D >( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
      }
        case DistanceFunctionType::PreEuclidean: {
        return( PreEuclideanDistanceFunction::Distance< D >( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
      }
        case DistanceFunctionType::SquareEuclidean: {
        return( SquareEuclideanDistanceFunction::Distance< D >( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
      }
        case DistanceFunctionType::CityBlock: {
        return( CityBlockDistanceFunction::Distance< D >( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
      }
        case DistanceFunctionType::ChessBoard:
        return( ChessBoardDistanceFunction::Distance< D >( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
    }
    return( 0.0 );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< char > &src, const C< char > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::CharDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< uchar > &src, const C< uchar > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::UCharDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< short > &src, const C< short > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::ShortDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

/*
 *   template< template< class D > class C >
 *   double DFIDE::Distance( const C< unsigned short > &src, const C< unsigned short > &tgt, const size_t src_orig,
 *                           const size_t tgt_orig, const size_t dms ) {
 * #ifdef BIAL_DEBUG
 *     if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
 *       std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
 *                               ". Maximum source index given: " + std::to_string( src_orig + dms ) +
 *                               ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
 *       throw( std::out_of_range( msg ) );
 *     }
 * #endif
 *     return( ( *DFIDE::UShortDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
 *   }
 */

  template< template< class D > class C >
  double DFIDE::Distance( const C< int > &src, const C< int > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::IntDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< unsigned int > &src, const C< unsigned int > &tgt, const size_t src_orig,
                          const size_t tgt_orig, const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::UIntDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

/*
 *   template< template< class D > class C >
 *   double DFIDE::Distance( const C< long > &src, const C< long > &tgt, const size_t src_orig, const size_t tgt_orig,
 *                           const size_t dms ) {
 * #ifdef BIAL_DEBUG
 *     if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
 *       std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
 *                               ". Maximum source index given: " + std::to_string( src_orig + dms ) +
 *                               ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
 *       throw( std::out_of_range( msg ) );
 *     }
 * #endif
 *     return( ( *DFIDE::LongDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
 *     //return( 0.0 );
 *   }
 */

  template< template< class D > class C >
  double DFIDE::Distance( const C< ulint > &src, const C< ulint > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::ULongDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
    /* return( 0.0 ); */
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< llint > &src, const C< llint > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::LLongDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< ullint > &src, const C< ullint > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::ULLongDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< float > &src, const C< float > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::FloatDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

  template< template< class D > class C >
  double DFIDE::Distance( const C< double > &src, const C< double > &tgt, const size_t src_orig, const size_t tgt_orig,
                          const size_t dms ) {
#ifdef BIAL_DEBUG
    if( ( src.size( ) < src_orig + dms ) || ( src.size( ) < tgt_orig + dms ) ) {
      std::string msg( BIAL_ERROR( "Index out of range. Maximum index: " + std::to_string( src.size( ) - 1 ) +
                                   ". Maximum source index given: " + std::to_string( src_orig + dms ) +
                                   ". Maximum target index given: " + std::to_string( tgt_orig + dms ) ) );
      throw( std::out_of_range( msg ) );
    }
#endif
    return( ( *DFIDE::DoubleDistancePtrTgt )( src.data( ) + src_orig, tgt.data( ) + tgt_orig, dms ) );
  }

  inline void DFIDE::SetEuclideanDistanceFunction( ) {
    distance_function_type = DistanceFunctionType::Euclidean;
    CharDistancePtr = &EuclideanDistanceFunction::Distance;
    CharDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    UCharDistancePtr = &EuclideanDistanceFunction::Distance;
    UCharDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    ShortDistancePtr = &EuclideanDistanceFunction::Distance;
    ShortDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    /*
     * UShortDistancePtr = &EuclideanDistanceFunction::Distance;
     * UShortDistancePtrTgt = &EuclideanDistanceFunction::Distance;
     */
    IntDistancePtr = &EuclideanDistanceFunction::Distance;
    IntDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    UIntDistancePtr = &EuclideanDistanceFunction::Distance;
    UIntDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    /*
     * LongDistancePtr = &EuclideanDistanceFunction::Distance;
     * LongDistancePtrTgt = &EuclideanDistanceFunction::Distance;
     */
    ULongDistancePtr = &EuclideanDistanceFunction::Distance;
    ULongDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    LLongDistancePtr = &EuclideanDistanceFunction::Distance;
    LLongDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    ULLongDistancePtr = &EuclideanDistanceFunction::Distance;
    ULLongDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    FloatDistancePtr = &EuclideanDistanceFunction::Distance;
    FloatDistancePtrTgt = &EuclideanDistanceFunction::Distance;
    DoubleDistancePtr = &EuclideanDistanceFunction::Distance;
    DoubleDistancePtrTgt = &EuclideanDistanceFunction::Distance;
  }

  inline void DFIDE::SetPreEuclideanDistanceFunction( size_t maximum_distance ) {
    distance_function_type = DistanceFunctionType::PreEuclidean;
    PreEuclideanDistanceFunction::SetPreEuclideanDistanceFunction( maximum_distance );
    CharDistancePtr = PreEuclideanDistanceFunction::Distance;
    CharDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    UCharDistancePtr = PreEuclideanDistanceFunction::Distance;
    UCharDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    ShortDistancePtr = PreEuclideanDistanceFunction::Distance;
    ShortDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    /*
     * UShortDistancePtr = PreEuclideanDistanceFunction::Distance;
     * UShortDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
     */
    IntDistancePtr = PreEuclideanDistanceFunction::Distance;
    IntDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    UIntDistancePtr = PreEuclideanDistanceFunction::Distance;
    UIntDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    /*
     * LongDistancePtr = PreEuclideanDistanceFunction::Distance;
     * LongDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
     */
    ULongDistancePtr = PreEuclideanDistanceFunction::Distance;
    ULongDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    LLongDistancePtr = PreEuclideanDistanceFunction::Distance;
    LLongDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    ULLongDistancePtr = PreEuclideanDistanceFunction::Distance;
    ULLongDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    FloatDistancePtr = PreEuclideanDistanceFunction::Distance;
    FloatDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
    DoubleDistancePtr = PreEuclideanDistanceFunction::Distance;
    DoubleDistancePtrTgt = PreEuclideanDistanceFunction::Distance;
  }

  inline void DFIDE::SetSquareEuclideanDistanceFunction( ) {
    distance_function_type = DistanceFunctionType::SquareEuclidean;
    CharDistancePtr = SquareEuclideanDistanceFunction::Distance;
    CharDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    UCharDistancePtr = SquareEuclideanDistanceFunction::Distance;
    UCharDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    ShortDistancePtr = SquareEuclideanDistanceFunction::Distance;
    ShortDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    /*
     * UShortDistancePtr = SquareEuclideanDistanceFunction::Distance;
     * UShortDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
     */
    IntDistancePtr = SquareEuclideanDistanceFunction::Distance;
    IntDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    UIntDistancePtr = SquareEuclideanDistanceFunction::Distance;
    UIntDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    /*
     * LongDistancePtr = SquareEuclideanDistanceFunction::Distance;
     * LongDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
     */
    ULongDistancePtr = SquareEuclideanDistanceFunction::Distance;
    ULongDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    LLongDistancePtr = SquareEuclideanDistanceFunction::Distance;
    LLongDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    ULLongDistancePtr = SquareEuclideanDistanceFunction::Distance;
    ULLongDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    FloatDistancePtr = SquareEuclideanDistanceFunction::Distance;
    FloatDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
    DoubleDistancePtr = SquareEuclideanDistanceFunction::Distance;
    DoubleDistancePtrTgt = SquareEuclideanDistanceFunction::Distance;
  }

  inline void DFIDE::SetCityBlockDistanceFunction( ) {
    distance_function_type = DistanceFunctionType::CityBlock;
    CharDistancePtr = CityBlockDistanceFunction::Distance;
    CharDistancePtrTgt = CityBlockDistanceFunction::Distance;
    UCharDistancePtr = CityBlockDistanceFunction::Distance;
    UCharDistancePtrTgt = CityBlockDistanceFunction::Distance;
    ShortDistancePtr = CityBlockDistanceFunction::Distance;
    ShortDistancePtrTgt = CityBlockDistanceFunction::Distance;
    /*
     * UShortDistancePtr = CityBlockDistanceFunction::Distance;
     * UShortDistancePtrTgt = CityBlockDistanceFunction::Distance;
     */
    IntDistancePtr = CityBlockDistanceFunction::Distance;
    IntDistancePtrTgt = CityBlockDistanceFunction::Distance;
    UIntDistancePtr = CityBlockDistanceFunction::Distance;
    UIntDistancePtrTgt = CityBlockDistanceFunction::Distance;
    /*
     * LongDistancePtr = CityBlockDistanceFunction::Distance;
     * LongDistancePtrTgt = CityBlockDistanceFunction::Distance;
     */
    ULongDistancePtr = CityBlockDistanceFunction::Distance;
    ULongDistancePtrTgt = CityBlockDistanceFunction::Distance;
    LLongDistancePtr = CityBlockDistanceFunction::Distance;
    LLongDistancePtrTgt = CityBlockDistanceFunction::Distance;
    ULLongDistancePtr = CityBlockDistanceFunction::Distance;
    ULLongDistancePtrTgt = CityBlockDistanceFunction::Distance;
    FloatDistancePtr = CityBlockDistanceFunction::Distance;
    FloatDistancePtrTgt = CityBlockDistanceFunction::Distance;
    DoubleDistancePtr = CityBlockDistanceFunction::Distance;
    DoubleDistancePtrTgt = CityBlockDistanceFunction::Distance;
  }

  inline void DFIDE::SetChessBoardDistanceFunction( ) {
    distance_function_type = DistanceFunctionType::ChessBoard;
    CharDistancePtr = ChessBoardDistanceFunction::Distance;
    CharDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    UCharDistancePtr = ChessBoardDistanceFunction::Distance;
    UCharDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    ShortDistancePtr = ChessBoardDistanceFunction::Distance;
    ShortDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    /*
     * UShortDistancePtr = ChessBoardDistanceFunction::Distance;
     * UShortDistancePtrTgt = ChessBoardDistanceFunction::Distance;
     */
    IntDistancePtr = ChessBoardDistanceFunction::Distance;
    IntDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    UIntDistancePtr = ChessBoardDistanceFunction::Distance;
    UIntDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    /*
     * LongDistancePtr = ChessBoardDistanceFunction::Distance;
     * LongDistancePtrTgt = ChessBoardDistanceFunction::Distance;
     */
    ULongDistancePtr = ChessBoardDistanceFunction::Distance;
    ULongDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    LLongDistancePtr = ChessBoardDistanceFunction::Distance;
    LLongDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    ULLongDistancePtr = ChessBoardDistanceFunction::Distance;
    ULLongDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    FloatDistancePtr = ChessBoardDistanceFunction::Distance;
    FloatDistancePtrTgt = ChessBoardDistanceFunction::Distance;
    DoubleDistancePtr = ChessBoardDistanceFunction::Distance;
    DoubleDistancePtrTgt = ChessBoardDistanceFunction::Distance;
  }

  /* Initializing distance function. */
  DistanceFunctionType DFIDE::distance_function_type = DistanceFunctionType::Euclidean;
  double ( *DFIDE::CharDistancePtr )( const char *src, const size_t dms ) = &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::CharDistancePtrTgt )( const char *src, const char *tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::UCharDistancePtr )( const uchar * src, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::UCharDistancePtrTgt )( const uchar * src, const uchar * tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::ShortDistancePtr )( const short *src, const size_t dms ) = &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::ShortDistancePtrTgt )( const short *src, const short *tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  /*
   * double ( *DFIDE::UShortDistancePtr )( const unsigned short *src, const size_t dms ) =
   *   &EuclideanDistanceFunction::Distance;
   * double ( *DFIDE::UShortDistancePtrTgt )( const unsigned short *src, const unsigned short *tgt, const size_t dms ) =
   *   &EuclideanDistanceFunction::Distance;
   */
  double ( *DFIDE::IntDistancePtr )( const int *src, const size_t dms ) = &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::IntDistancePtrTgt )( const int *src, const int *tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::UIntDistancePtr )( const unsigned int *src, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::UIntDistancePtrTgt )( const unsigned int *src, const unsigned int *tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  /*
   * double ( *DFIDE::LongDistancePtr )( const long *src, const size_t dms ) = &EuclideanDistanceFunction::Distance;
   * double ( *DFIDE::LongDistancePtrTgt )( const long *src, const long *tgt, const size_t dms ) =
   *   &EuclideanDistanceFunction::Distance;
   */
  double ( *DFIDE::ULongDistancePtr )( const ulint *src, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::ULongDistancePtrTgt )( const ulint *src, const ulint * tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::LLongDistancePtr )( const llint * src, const size_t dms ) = &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::LLongDistancePtrTgt )( const llint * src, const llint * tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::ULLongDistancePtr )( const ullint * src, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::ULLongDistancePtrTgt )( const ullint * src,
                                           const ullint * tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::FloatDistancePtr )( const float *src, const size_t dms ) = &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::FloatDistancePtrTgt )( const float *src, const float *tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::DoubleDistancePtr )( const double *src, const size_t dms ) = &EuclideanDistanceFunction::Distance;
  double ( *DFIDE::DoubleDistancePtrTgt )( const double *src, const double *tgt, const size_t dms ) =
    &EuclideanDistanceFunction::Distance;

  /* Explicit instantiation of template functions. ------------------------------------------------------------------ */

#ifdef BIAL_EXPLICIT_DFIDE

  template double DFIDE::Distance( const Feature< int > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Feature< int > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Feature< int > &src, const Feature< int > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Feature< llint > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Feature< llint > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Feature< llint > &src, const Feature< llint > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Feature< float > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Feature< float > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Feature< float > &src, const Feature< float > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Feature< double > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Feature< double > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Feature< double > &src, const Feature< double > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );

  template double DFIDE::Distance( const Image< int > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Image< int > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Image< int > &src, const Image< int > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Image< llint > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Image< llint > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Image< llint > &src, const Image< llint > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Image< float > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Image< float > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Image< float > &src, const Image< float > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Image< double > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Image< double > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Image< double > &src, const Image< double > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );

  template double DFIDE::Distance( const Matrix< int > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Matrix< int > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Matrix< int > &src, const Matrix< int > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Matrix< llint > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Matrix< llint > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Matrix< llint > &src, const Matrix< llint > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Matrix< float > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Matrix< float > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Matrix< float > &src, const Matrix< float > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Matrix< double > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Matrix< double > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Matrix< double > &src, const Matrix< double > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );

  template double DFIDE::Distance( const Vector< char > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Vector< char > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Vector< char > &src, const Vector< char > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Vector< int > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Vector< int > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Vector< int > &src, const Vector< int > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Vector< uint > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Vector< uint > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Vector< uint > &src, const Vector< uint > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Vector< ulint > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Vector< ulint > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Vector< ulint > &src, const Vector< ulint > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Vector< llint > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Vector< llint > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Vector< llint > &src, const Vector< llint > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Vector< float > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Vector< float > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Vector< float > &src, const Vector< float > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );
  template double DFIDE::Distance( const Vector< double > &src, const size_t orig, const size_t dms );
  template double DFIDE::Distance( const Vector< double > &src, const size_t src_orig, const size_t tgt_orig,
                                   const size_t dms );
  template double DFIDE::Distance( const Vector< double > &src, const Vector< double > &tgt, const size_t src_orig,
                                   const size_t tgt_orig, const size_t dms );

#endif

}

#endif

#endif
