/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Morphological erosion over images. 
 */

#ifndef BIALMORPHOLOGYEROSION_C
#define BIALMORPHOLOGYEROSION_C

#include "MorphologyErosion.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MorphologyErosion )
#define BIAL_EXPLICIT_MorphologyErosion
#endif
#if defined ( BIAL_EXPLICIT_MorphologyErosion ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Morphology::Erode( const Image< D > &image ) {
    try {
      return( Erode( image, AdjacencyType::HyperSpheric( 1.0, image.Dims( ) ) ) );
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
  Image< D > Morphology::Erode( const Image< D > &image, const Adjacency &adjacency ) {
    try {
      if( image.Dims( ) != adjacency.Dims( ) ) {
        std::string msg( std::string( __FILE__ ) + ": " + std::to_string( __LINE__ ) + ": " + std::string(
													  __FUNCTION__ ) +
                         ": error: Image and adjacency relation dimensions do not match." );
        throw( std::logic_error( msg ) );
      }
      Image< D > result( image );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads.push_back( std::thread( &Morphology::ErodeThreads< D >, std::ref( image ), std::ref( adjacency ),
                                          std::ref( result ), thd, total_threads ) );
        }
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads( thd ).join( );
        }
      }
      catch( std::exception &e ) {
        std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
        Morphology::ErodeThreads( image, adjacency, result, 0, 1 );
      }
      return( result );
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
  void Morphology::ErodeThreads( const Image< D > &image, const Adjacency &adjacency, Image< D > &result,
                                 size_t thread, size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 3 );
      size_t min_index = thread * image.Size( ) / total_threads;
      size_t max_index = ( thread + 1 ) * image.Size( ) / total_threads;
      for( size_t img_index = min_index; img_index < max_index; ++img_index ) {
        for( AdjacencyIterator adj_it = begin( adjacency, image, img_index ); *adj_it != image.size( ); ++adj_it ) {
          size_t adj_index = *adj_it;
          if( result[ img_index ] > image[ adj_index ] ) {
            result[ img_index ] = image[ adj_index ];
          }
        }
      }
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
  Image< D > Morphology::ErodeBin( const Image< D > &image, const Adjacency &adjacency ) {
    try {
      COMMENT( "Inserting pixels into the priority queue.", 2 );
      Vector< size_t > seeds;
      for( size_t pxl = 0; pxl < image.size( ); ++pxl ) {
        if( image[ pxl ] != 0 ) {
          seeds.push_back( pxl );
        }
      }
      COMMENT( "Computing erosion result.", 2 );
      Image< D > res( image );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads.push_back( std::thread( &Morphology::ErodeBinThreads< D >, std::ref( image ), std::ref( adjacency ),
                                          std::ref( seeds ), std::ref( res ), thd, total_threads ) );
        }
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads( thd ).join( );
        }
      }
      catch( std::exception &e ) {
        std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
        Morphology::ErodeBinThreads( image, adjacency, seeds, res, 0, 1 );
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
  void Morphology::ErodeBinThreads( const Image< D > &image, const Adjacency &adjacency, const Vector< size_t > &seeds,
                                    Image< D > &result, size_t thread, size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 3 );
      size_t min_index = thread * seeds.size( ) / total_threads;
      size_t max_index = ( thread + 1 ) * seeds.size( ) / total_threads;
      for( size_t pxl = min_index; pxl < max_index; ++pxl ) {
        for( AdjacencyIterator itr = begin( adjacency, image, seeds( pxl ) ); *itr != image.size( ); ++itr ) {
          size_t adj_pxl = *itr;
          if( image[ adj_pxl ] == 0 ) {
            result[ seeds( pxl ) ] = 0;
            break;
          }
        }
      }
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

#ifdef BIAL_EXPLICIT_MorphologyErosion

  template Image< int > Morphology::Erode( const Image< int > &image );
  template void Morphology::ErodeThreads( const Image< int > &image, const Adjacency &adjacency, 
                                          Image< int > &result, size_t thread, size_t total_threads );
  template Image< int > Morphology::Erode( const Image< int > &image, const Adjacency &adjacency );
  template Image< int > Morphology::ErodeBin( const Image< int > &image, const Adjacency &adjacency );
  template void Morphology::ErodeBinThreads( const Image< int > &image, const Adjacency &adjacency, 
                                             const Vector< size_t > &seeds, Image< int > &result, 
                                             size_t thread, size_t total_threads );

  template Image< llint > Morphology::Erode( const Image< llint > &image );
  template Image< llint > Morphology::Erode( const Image< llint > &image, const Adjacency &adjacency );
  template Image< llint > Morphology::ErodeBin( const Image< llint > &image, const Adjacency &adjacency );
  template void Morphology::ErodeBinThreads( const Image< llint > &image, const Adjacency &adjacency, 
                                             const Vector< size_t > &seeds, Image< llint > &result, 
                                             size_t thread, size_t total_threads );

  template Image< float > Morphology::Erode( const Image< float > &image );
  template void Morphology::ErodeThreads( const Image< float > &image, const Adjacency &adjacency, 
                                          Image< float > &result, size_t thread, size_t total_threads );
  template Image< float > Morphology::Erode( const Image< float > &image, const Adjacency &adjacency );
  template Image< float > Morphology::ErodeBin( const Image< float > &image, const Adjacency &adjacency );
  template void Morphology::ErodeBinThreads( const Image< float > &image, const Adjacency &adjacency, 
                                             const Vector< size_t > &seeds, Image< float > &result, 
                                             size_t thread, size_t total_threads );

  template Image< double > Morphology::Erode( const Image< double > &image );
  template Image< double > Morphology::Erode( const Image< double > &image, const Adjacency &adjacency );
  template Image< double > Morphology::ErodeBin( const Image< double > &image, const Adjacency &adjacency );
  template void Morphology::ErodeBinThreads( const Image< double > &image, const Adjacency &adjacency, 
                                             const Vector< size_t > &seeds, Image< double > &result, 
                                             size_t thread, size_t total_threads );

#endif

}

#endif

#endif


