/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/08
 * @brief Functions to compute intensity local minima or maxima.
 */

#ifndef BIALINTENSITYLOCALS_C
#define BIALINTENSITYLOCALS_C

#include "IntensityLocals.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_IntensityLocals )
#define BIAL_EXPLICIT_IntensityLocals
#endif

#if defined ( BIAL_EXPLICIT_IntensityLocals ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"

namespace Bial {

  template< class D >
  Vector< bool > Intensity::LocalMinima( const Image< D > &image, const Adjacency &adjacency ) {
    try {
      Vector< bool > res( image.size( ) );
      for( size_t img_idx = 0; img_idx < image.size( ); ++img_idx ) {
        bool minima = true;
        for( size_t adj_it = 1; adj_it < adjacency.size( ); ++adj_it ) {
          size_t adj_idx = adjacency( image, img_idx, adj_it );
          if( ( adj_idx < image.size( ) ) && ( image[ img_idx ] > image[ adj_idx ] ) ) {
            minima = false;
            break;
          }
        }
        res( img_idx ) = minima;
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  Vector< bool > Intensity::LocalMaxima( const Image< D > &image, const Adjacency &adjacency ) {
    try {
      Vector< bool > res( image.size( ) );
      for( size_t img_idx = 0; img_idx < image.size( ); ++img_idx ) {
        bool maxima = true;
        for( size_t adj_it = 1; adj_it < adjacency.size( ); ++adj_it ) {
          size_t adj_idx = adjacency( image, img_idx, adj_it );
          if( ( adj_idx < image.size( ) ) && ( image[ img_idx ] < image[ adj_idx ] ) ) {
            maxima = false;
            break;
          }
        }
        res( img_idx ) = maxima;
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D, class D2 >
  Vector< bool > Intensity::LocalMinima( const Image< D > &image, const Image< D2 > &mask, 
                                         const Adjacency &adjacency ) {
    try {
      Vector< bool > res( image.size( ) );
      for( size_t img_idx = 0; img_idx < image.size( ); ++img_idx ) {
        if( mask[ img_idx ] != 0 ) {
          bool minima = true;
          for( size_t adj_it = 1; adj_it < adjacency.size( ); ++adj_it ) {
            size_t adj_idx = adjacency( image, img_idx, adj_it );
            if( ( adj_idx < image.size( ) ) && ( image[ img_idx ] > image[ adj_idx ] ) ) {
              minima = false;
              break;
            }
          }
          res( img_idx ) = minima;
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D, class D2 >
  Vector< bool > Intensity::LocalMaxima( const Image< D > &image, const Image< D2 > &mask, 
                                         const Adjacency &adjacency ) {
    try {
      Vector< bool > res( image.size( ) );
      for( size_t img_idx = 0; img_idx < image.size( ); ++img_idx ) {
        if( mask[ img_idx ] != 0 ) {
          bool maxima = true;
          for( size_t adj_it = 1; adj_it < adjacency.size( ); ++adj_it ) {
            size_t adj_idx = adjacency( image, img_idx, adj_it );
            if( ( adj_idx < image.size( ) ) && ( image[ img_idx ] < image[ adj_idx ] ) ) {
              maxima = false;
              break;
            }
          }
          res( img_idx ) = maxima;
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR(
                         "Image, window end, and/or window size dimensions do not match." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_IntensityLocals

  template Vector< bool > Intensity::LocalMinima( const Image< int > &image, const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< int > &image, const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< llint > &image, const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< llint > &image, const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< float > &image, const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< float > &image, const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< double > &image, const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< double > &image, const Adjacency &adjacency );

  template Vector< bool > Intensity::LocalMinima( const Image< int > &image, const Image< int > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< int > &image, const Image< int > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< llint > &image, const Image< int > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< llint > &image, const Image< int > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< float > &image, const Image< int > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< float > &image, const Image< int > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< double > &image, const Image< int > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< double > &image, const Image< int > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< int > &image, const Image< llint > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< int > &image, const Image< llint > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< llint > &image, const Image< llint > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< llint > &image, const Image< llint > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< float > &image, const Image< llint > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< float > &image, const Image< llint > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< double > &image, const Image< llint > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< double > &image, const Image< llint > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< int > &image, const Image< float > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< int > &image, const Image< float > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< llint > &image, const Image< float > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< llint > &image, const Image< float > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< float > &image, const Image< float > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< float > &image, const Image< float > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< double > &image, const Image< float > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< double > &image, const Image< float > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< int > &image, const Image< double > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< int > &image, const Image< double > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< llint > &image, const Image< double > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< llint > &image, const Image< double > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< float > &image, const Image< double > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< float > &image, const Image< double > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMinima( const Image< double > &image, const Image< double > &mask,
                                                  const Adjacency &adjacency );
  template Vector< bool > Intensity::LocalMaxima( const Image< double > &image, const Image< double > &mask,
                                                  const Adjacency &adjacency );

#endif

}

#endif

#endif
