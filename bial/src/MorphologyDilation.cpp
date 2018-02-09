/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Morphological dilation over images. 
 */

#ifndef BIALMORPHOLOGYDILATION_C
#define BIALMORPHOLOGYDILATION_C

#include "MorphologyDilation.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MorphologyDilation )
#define BIAL_EXPLICIT_MorphologyDilation
#endif
#if defined ( BIAL_EXPLICIT_MorphologyDilation ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyRound.hpp"
#include "AdjacencyIterator.hpp"
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Morphology::Dilate( const Image< D > &image ) {
    try {
      /* cout << "adj size: " << AdjacencyType::HyperSpheric( 1.0, image.Dims( ) ).Size( ) << std::endl; */
      return( Dilate( image, AdjacencyType::HyperSpheric( 1.0, image.Dims( ) ) ) );
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
  Image< D > Morphology::Dilate( const Image< D > &image, const Adjacency &adjacency ) {
    try {
      if( image.Dims( ) != adjacency.Dims( ) ) {
        std::string msg( std::string( __FILE__ ) + ": " + std::to_string( __LINE__ ) + ": " + 
                         std::string( __FUNCTION__ ) +
                         ": error: Image and adjacency relation dimensions do not match." );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Computing dilation.", 2 );
      Image< D > result( image );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads.push_back( std::thread( &Morphology::DilateThreads< D >, std::ref( image ), std::ref( adjacency ),
                                          std::ref( result ), thd, total_threads ) );
        }
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads( thd ).join( );
        }
      }
      catch( std::exception &e ) {
        std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
        Morphology::DilateThreads( image, adjacency, result, 0, 1 );
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
  void Morphology::DilateThreads( const Image< D > &image, const Adjacency &adjacency, Image< D > &result, 
                                  size_t thread, size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 3 );
      AdjacencyIterator adj_itr( image, adjacency );
      size_t adj_size = adjacency.size( );
      size_t adj_index;
      size_t min_index = thread * image.Size( ) / total_threads;
      size_t max_index = ( thread + 1 ) * image.Size( ) / total_threads;
      for( size_t src_index = min_index; src_index < max_index; ++src_index ) {
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( adj_itr.AdjIdx( src_index, idx, adj_index ) ) {
            if( result[ src_index ] < image[ adj_index ] )
              result[ src_index ] = image[ adj_index ];
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
  Image< D > Morphology::DilateBin( const Image< D > &image, const Adjacency &adjacency ) {
    try {
      COMMENT( "Inserting pixels into the priority queue.", 2 );
      Vector< size_t > seeds;
      for( size_t pxl = 0; pxl < image.size( ); ++pxl ) {
        if( image[ pxl ] == 0 )
          seeds.push_back( pxl );
      }
      COMMENT( "Computing dilation result.", 2 );
      Image< D > res( image );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads.push_back( std::thread( &Morphology::DilateBinThreads< D >, std::ref( image ), std::ref( adjacency ),
                                          std::ref( seeds ), std::ref( res ), thd, total_threads ) );
        }
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads( thd ).join( );
        }
      }
      catch( std::exception &e ) {
        std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
        Morphology::DilateBinThreads( image, adjacency, seeds, res, 0, 1 );
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
  void Morphology::DilateBinThreads( const Image< D > &image, const Adjacency &adjacency, const Vector< size_t > &seeds,
                                     Image< D > &result, size_t thread, size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 3 );
      size_t min_index = thread * seeds.size( ) / total_threads;
      size_t max_index = ( thread + 1 ) * seeds.size( ) / total_threads;
      AdjacencyIterator adj_itr( image, adjacency );
      size_t adj_size = adjacency.size( );
      size_t adj_pxl;
      for( size_t sed = min_index; sed < max_index; ++sed ) {
        size_t pxl = seeds[ sed ];
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( adj_itr.AdjIdx( pxl, idx, adj_pxl ) ) {
            if( image[ adj_pxl ] != 0 ) {
              result[ pxl ] = image[ adj_pxl ];
              break;
            }
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

#ifdef BIAL_EXPLICIT_MorphologyDilation

  template  Image< int > Morphology::Dilate( const Image< int > &image );
  template void Morphology::DilateThreads( const Image< int > &image, const Adjacency &adjacency, 
                                            Image< int > &result, size_t thread, size_t total_threads );
  template Image< int > Morphology::Dilate( const Image< int > &image, const Adjacency &adjacency );
  template Image< int > Morphology::DilateBin( const Image< int > &image, const Adjacency &adjacency );
  template void Morphology::DilateBinThreads( const Image< int > &image, const Adjacency &adjacency, 
                                              const Vector< size_t > &seeds, Image< int > &result, 
                                              size_t thread, size_t total_threads );

  template  Image< llint > Morphology::Dilate( const Image< llint > &image );
  template void Morphology::DilateThreads( const Image< llint > &image, const Adjacency &adjacency, 
                                            Image< llint > &result, size_t thread, size_t total_threads );
  template Image< llint > Morphology::Dilate( const Image< llint > &image, const Adjacency &adjacency );
  template Image< llint > Morphology::DilateBin( const Image< llint > &image, const Adjacency &adjacency );
  template void Morphology::DilateBinThreads( const Image< llint > &image, const Adjacency &adjacency, 
                                              const Vector< size_t > &seeds, Image< llint > &result, 
                                              size_t thread, size_t total_threads );

  template  Image< float > Morphology::Dilate( const Image< float > &image );
  template void Morphology::DilateThreads( const Image< float > &image, const Adjacency &adjacency, 
                                            Image< float > &result, size_t thread, size_t total_threads );
  template Image< float > Morphology::Dilate( const Image< float > &image, const Adjacency &adjacency );
  template Image< float > Morphology::DilateBin( const Image< float > &image, const Adjacency &adjacency );
  template void Morphology::DilateBinThreads( const Image< float > &image, const Adjacency &adjacency, 
                                              const Vector< size_t > &seeds, Image< float > &result, 
                                              size_t thread, size_t total_threads );

  template  Image< double > Morphology::Dilate( const Image< double > &image );
  template void Morphology::DilateThreads( const Image< double > &image, const Adjacency &adjacency, 
                                            Image< double > &result, size_t thread, size_t total_threads );
  template Image< double > Morphology::Dilate( const Image< double > &image, const Adjacency &adjacency );
  template Image< double > Morphology::DilateBin( const Image< double > &image, const Adjacency &adjacency );
  template void Morphology::DilateBinThreads( const Image< double > &image, const Adjacency &adjacency, 
                                              const Vector< size_t > &seeds, Image< double > &result, 
                                              size_t thread, size_t total_threads );

#endif

}

#endif

#endif


