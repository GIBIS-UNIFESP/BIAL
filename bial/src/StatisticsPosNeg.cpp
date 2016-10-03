/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Dec/04
 * @brief Basic true and false positive and negative metrics.
 */

#ifndef BIALSTATISTICSPOSNEG_C
#define BIALSTATISTICSPOSNEG_C

#include "StatisticsPosNeg.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_StatisticsPosNeg )
#define BIAL_EXPLICIT_StatisticsPosNeg
#endif
#if defined ( BIAL_EXPLICIT_StatisticsPosNeg ) || ( BIAL_IMPLICIT_BIN )

#include "Image.hpp"
#include "TransformEuclDist.hpp"

namespace Bial {

  template< template< class D > class C, class D >
  std::tuple< size_t, size_t, size_t, size_t > Statistics::PositiveNegative( const C< D > &src_label, 
                                                                             const C< D > &tgt_label ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t TP = 0, TN = 0, FP = 0, FN = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( ( src_label( pxl ) != 0 ) && ( tgt_label( pxl ) != 0 ) ) {
          ++TP;
        }
        if( ( src_label( pxl ) == 0 ) && ( tgt_label( pxl ) == 0 ) ) {
          ++TN;
        }
        if( ( src_label( pxl ) != 0 ) && ( tgt_label( pxl ) == 0 ) ) {
          ++FP;
        }
        if( ( src_label( pxl ) == 0 ) && ( tgt_label( pxl ) != 0 ) ) {
          ++FN;
        }
      }
      return( std::tie( TP, TN, FP, FN ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  std::tuple< size_t, size_t, size_t, size_t > Statistics::PositiveNegative( const Image< D > &src_label, 
                                                                             const Image< D > &tgt_label, float dist ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      Image< float > src_dist( Transform::EDT( src_label ) );
      // DEBUG_WRITE( src_dist, "src-edt", 0 );
      Image< float > tgt_dist( Transform::EDT( tgt_label ) );
      // DEBUG_WRITE( tgt_dist, "tgt-edt", 0 );
      size_t TP = 0, TN = 0, FP = 0, FN = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( src_label[ pxl ] != 0 ) {
          if( tgt_dist[ pxl ] <= dist ) {
            ++TN;
          }
          else {
            ++FN;
          }
        }
        else if( tgt_label[ pxl ] != 0 ) {
          if( src_dist[ pxl ] <= dist ) {
            ++TP;
          }
          else {
            ++FP;
          }
        }
      }
      return( std::tie( TP, TN, FP, FN ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< template< class D > class C, class D >
  size_t Statistics::TruePositive( const C< D > &src_label, const C< D > &tgt_label ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t res = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( ( src_label( pxl ) != 0 ) && ( tgt_label( pxl ) != 0 ) ) {
          ++res;
        }
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t Statistics::TruePositive( const Image< D > &src_label, const Image< D > &tgt_label, float dist ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      Image< float > src_dist( Transform::EDT( src_label ) );
      // DEBUG_WRITE( src_dist, "src-edt", 0 );
      size_t TP = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( tgt_label[ pxl ] != 0 ) {
          if( src_dist[ pxl ] <= dist ) {
            ++TP;
          }
        }
      }
      return( TP );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< template< class D > class C, class D >
  size_t Statistics::TrueNegative( const C< D > &src_label, const C< D > &tgt_label ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t res = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( ( src_label( pxl ) == 0 ) && ( tgt_label( pxl ) == 0 ) ) {
          ++res;
        }
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t Statistics::TrueNegative( const Image< D > &src_label, const Image< D > &tgt_label, float dist ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      Image< float > tgt_dist( Transform::EDT( tgt_label ) );
      // DEBUG_WRITE( tgt_dist, "tgt-edt", 0 );
      size_t TN = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( src_label[ pxl ] != 0 ) {
          if( tgt_dist[ pxl ] <= dist ) {
            ++TN;
          }
        }
      }
      return( TN );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< template< class D > class C, class D >
  size_t Statistics::FalsePositive( const C< D > &src_label, const C< D > &tgt_label ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t res = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( ( src_label( pxl ) != 0 ) && ( tgt_label( pxl ) == 0 ) ) {
          ++res;
        }
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t Statistics::FalsePositive( const Image< D > &src_label, const Image< D > &tgt_label, float dist ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      Image< float > src_dist( Transform::EDT( src_label ) );
      // DEBUG_WRITE( src_dist, "src-edt", 0 );
      size_t FP = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( tgt_label[ pxl ] != 0 ) {
          if( src_dist[ pxl ] > dist ) {
            ++FP;
          }
        }
      }
      return( FP );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< template< class D > class C, class D >
  size_t Statistics::FalseNegative( const C< D > &src_label, const C< D > &tgt_label ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      size_t res = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( ( src_label( pxl ) == 0 ) && ( tgt_label( pxl ) != 0 ) ) {
          ++res;
        }
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t Statistics::FalseNegative( const Image< D > &src_label, const Image< D > &tgt_label, float dist ) {
    try {
      if( src_label.size( ) != tgt_label.size( ) ) {
        std::string msg( BIAL_ERROR( "Input data dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      Image< float > tgt_dist( Transform::EDT( tgt_label ) );
      // DEBUG_WRITE( tgt_dist, "tgt-edt", 0 );
      size_t FN = 0;
      for( size_t pxl = 0; pxl < src_label.size( ); ++pxl ) {
        if( src_label[ pxl ] != 0 ) {
          if( tgt_dist[ pxl ] > dist ) {
            ++FN;
          }
        }
      }
      return( FN );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_StatisticsPosNeg

  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Image< int > &src_label, const Image< int > &tgt_label );
  template size_t Statistics::TruePositive( const Image< int > &src_label, const Image< int > &tgt_label );
  template size_t Statistics::TrueNegative( const Image< int > &src_label, const Image< int > &tgt_label );
  template size_t Statistics::FalsePositive( const Image< int > &src_label, const Image< int > &tgt_label );
  template size_t Statistics::FalseNegative( const Image< int > &src_label, const Image< int > &tgt_label );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Image< llint > &src_label, const Image< llint > &tgt_label );
  template size_t Statistics::TruePositive( const Image< llint > &src_label, const Image< llint > &tgt_label );
  template size_t Statistics::TrueNegative( const Image< llint > &src_label, const Image< llint > &tgt_label );
  template size_t Statistics::FalsePositive( const Image< llint > &src_label, const Image< llint > &tgt_label );
  template size_t Statistics::FalseNegative( const Image< llint > &src_label, const Image< llint > &tgt_label );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Image< float > &src_label, const Image< float > &tgt_label );
  template size_t Statistics::TruePositive( const Image< float > &src_label, const Image< float > &tgt_label );
  template size_t Statistics::TrueNegative( const Image< float > &src_label, const Image< float > &tgt_label );
  template size_t Statistics::FalsePositive( const Image< float > &src_label, const Image< float > &tgt_label );
  template size_t Statistics::FalseNegative( const Image< float > &src_label, const Image< float > &tgt_label );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Image< double > &src_label, const Image< double > &tgt_label );
  template size_t Statistics::TruePositive( const Image< double > &src_label, const Image< double > &tgt_label );
  template size_t Statistics::TrueNegative( const Image< double > &src_label, const Image< double > &tgt_label );
  template size_t Statistics::FalsePositive( const Image< double > &src_label, const Image< double > &tgt_label );
  template size_t Statistics::FalseNegative( const Image< double > &src_label, const Image< double > &tgt_label );

  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Vector< int > &src_label, const Vector< int > &tgt_label );
  template size_t Statistics::TruePositive( const Vector< int > &src_label, const Vector< int > &tgt_label );
  template size_t Statistics::TrueNegative( const Vector< int > &src_label, const Vector< int > &tgt_label );
  template size_t Statistics::FalsePositive( const Vector< int > &src_label, const Vector< int > &tgt_label );
  template size_t Statistics::FalseNegative( const Vector< int > &src_label, const Vector< int > &tgt_label );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Vector< llint > &src_label, const Vector< llint > &tgt_label );
  template size_t Statistics::TruePositive( const Vector< llint > &src_label, const Vector< llint > &tgt_label );
  template size_t Statistics::TrueNegative( const Vector< llint > &src_label, const Vector< llint > &tgt_label );
  template size_t Statistics::FalsePositive( const Vector< llint > &src_label, const Vector< llint > &tgt_label );
  template size_t Statistics::FalseNegative( const Vector< llint > &src_label, const Vector< llint > &tgt_label );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Vector< float > &src_label, const Vector< float > &tgt_label );
  template size_t Statistics::TruePositive( const Vector< float > &src_label, const Vector< float > &tgt_label );
  template size_t Statistics::TrueNegative( const Vector< float > &src_label, const Vector< float > &tgt_label );
  template size_t Statistics::FalsePositive( const Vector< float > &src_label, const Vector< float > &tgt_label );
  template size_t Statistics::FalseNegative( const Vector< float > &src_label, const Vector< float > &tgt_label );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Vector< double > &src_label, const Vector< double > &tgt_label );
  template size_t Statistics::TruePositive( const Vector< double > &src_label, const Vector< double > &tgt_label );
  template size_t Statistics::TrueNegative( const Vector< double > &src_label, const Vector< double > &tgt_label );
  template size_t Statistics::FalsePositive( const Vector< double > &src_label, const Vector< double > &tgt_label );
  template size_t Statistics::FalseNegative( const Vector< double > &src_label, const Vector< double > &tgt_label );

  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Image< int > &src_label, const Image< int > &tgt_label, float dist );
  template size_t Statistics::TruePositive( const Image< int > &src_label, const Image< int > &tgt_label, float dist );
  template size_t Statistics::TrueNegative( const Image< int > &src_label, const Image< int > &tgt_label, float dist );
  template size_t Statistics::FalsePositive( const Image< int > &src_label, const Image< int > &tgt_label, float dist );
  template size_t Statistics::FalseNegative( const Image< int > &src_label, const Image< int > &tgt_label, float dist );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Image< llint > &src_label, const Image< llint > &tgt_label, float dist );
  template size_t Statistics::TruePositive( const Image< llint > &src_label, const Image< llint > &tgt_label, 
                                            float dist );
  template size_t Statistics::TrueNegative( const Image< llint > &src_label, const Image< llint > &tgt_label, 
                                            float dist );
  template size_t Statistics::FalsePositive( const Image< llint > &src_label, const Image< llint > &tgt_label, 
                                             float dist );
  template size_t Statistics::FalseNegative( const Image< llint > &src_label, const Image< llint > &tgt_label, 
                                             float dist );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Image< float > &src_label, const Image< float > &tgt_label, float dist );
  template size_t Statistics::TruePositive( const Image< float > &src_label, const Image< float > &tgt_label, 
                                            float dist );
  template size_t Statistics::TrueNegative( const Image< float > &src_label, const Image< float > &tgt_label, 
                                            float dist );
  template size_t Statistics::FalsePositive( const Image< float > &src_label, const Image< float > &tgt_label, 
                                             float dist );
  template size_t Statistics::FalseNegative( const Image< float > &src_label, const Image< float > &tgt_label, 
                                             float dist );
  template std::tuple< size_t, size_t, size_t, size_t > 
  Statistics::PositiveNegative( const Image< double > &src_label, const Image< double > &tgt_label, float dist );
  template size_t Statistics::TruePositive( const Image< double > &src_label, const Image< double > &tgt_label, 
                                            float dist );
  template size_t Statistics::TrueNegative( const Image< double > &src_label, const Image< double > &tgt_label, 
                                            float dist );
  template size_t Statistics::FalsePositive( const Image< double > &src_label, const Image< double > &tgt_label, 
                                             float dist );
  template size_t Statistics::FalseNegative( const Image< double > &src_label, const Image< double > &tgt_label, 
                                             float dist );

#endif

}

#endif

#endif
