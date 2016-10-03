/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Generates a feature vector composed by red, green, and blue median colors in the adjacency of pixels 
 * as the features. A vector is generated for each valid pixel.
 */

#ifndef BIALCOLORMEDIANFEATURE_C
#define BIALCOLORMEDIANFEATURE_C

#include "ColorMedianFeature.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_ColorMedianFeature )
#define BIAL_EXPLICIT_ColorMedianFeature
#endif

#if defined ( BIAL_EXPLICIT_ColorMedianFeature ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "Color.hpp"
#include "Feature.hpp"
#include "Image.hpp"
#include "SortingSort.hpp"

namespace Bial {

  template< class D >
  Feature< D > ColorMedianFeature( const Image< Color > &src, const Adjacency &adj_rel, float proportion ) {
    try {
      if( ( proportion > 1.0 ) || ( proportion <= 0.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid proportion. Expected: 0 < proportion <= 1." ) );
        throw( std::logic_error( msg ) );
      }
      if( src.Dims( ) != adj_rel.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and adjacency relation dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating feature vector.", 0 );
      size_t features = adj_rel.size( ) * proportion * 3;
      if( features == 0 ) {
        features = 3;
      }
      size_t elements = src.size( );
      Feature< D > res( elements, features );
      COMMENT( "Computing median features.", 0 );
      try {
        size_t total_threads = 12;
        Vector< std::thread > threads;
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads.push_back( std::thread( &ColorMedianFeatureThread< D >, std::ref( src ), std::ref( adj_rel ),
                                          std::ref( res ), thd, total_threads ) );
        }
        for( size_t thd = 0; thd < total_threads; ++thd ) {
          threads( thd ).join( );
        }
      }
      catch( std::exception &e ) {
        std::cout << "Warning: Failed to run in multi-thread. Exception: " << e.what( ) << std::endl;
        ColorMedianFeatureThread( src, adj_rel, res, 0, 1 );
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
  Feature< D > ColorMedianFeature( const Image< Color > &src, const Image< D > &msk, const Adjacency &adj_rel, 
                                   float proportion ) {
    try {
      if( ( proportion > 1.0 ) || ( proportion <= 0.0 ) ) {
        std::string msg( BIAL_ERROR( "Invalid proportion. Expected: 0 < proportion <= 1." ) );
        throw( std::logic_error( msg ) );
      }
      if( src.Dims( ) != adj_rel.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Image and adjacency relation dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      if( src.Dims( ) != msk.Dims( ) ) {
        std::string msg( BIAL_ERROR( "Source image and mask image dimensions do not match." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating feature vector.", 0 );
      size_t features = adj_rel.size( ) * proportion * 3;
      if( features == 0 ) {
        features = 3;
      }
      size_t elements = 0;
      for( size_t elm = 0; elm < msk.size( ); ++elm ) {
        if( msk( elm ) != 0 ) 
          ++elements;
      }
      //Bial::Elements( msk, static_cast< D >( 1 ), msk.Maximum( ) );
      Feature< D > res( elements, features );
      COMMENT( "Computing median pixels.", 0 );
      Vector< double > median( adj_rel.size( ) );
      Vector< size_t > index( adj_rel.size( ) );
      size_t adj_base = ( adj_rel.size( ) / 2 ) - ( features / ( 2 * 3 ) );
      size_t tgt_elm = 0;
      for( size_t src_elm = 0; src_elm < src.size( ); ++src_elm ) {
        if( msk[ src_elm ] != 0 ) {
          COMMENT( "Computing the sum of the distances of all channels from adjacent to source pixels.", 4 );
          for( size_t adj = 0; adj < adj_rel.size( ); ++adj ) {
            size_t elm_adj = adj_rel( src, src_elm, adj );
            index( adj ) = elm_adj;
            if( ( elm_adj < src.size( ) ) && ( msk[ elm_adj ] != 0 ) ) {
              for( size_t chn = 1; chn < 4; ++chn ) {
                D adj_value = src( elm_adj )( chn );
                D src_value = src( src_elm )( chn );
                median( adj ) += ( adj_value > src_value ) ? adj_value - src_value : src_value - adj_value;
              }
            }
            else {
              median( adj ) = 0;
            }
          }
          COMMENT( "Sorting the distances.", 4 );
          Sorting::Sort( index, Sorting::Sort( median ) );
          COMMENT( "Getting the features from the median distances.", 4 );
          for( size_t ftr = 0; ftr < features / 3; ++ftr ) {
            for( size_t chn = 1; chn < 4; ++chn ) {
              res( tgt_elm, ftr * 3 + ( chn - 1 ) ) = src( index( adj_base + ftr ) )( chn );
            }
            res.Index( tgt_elm ) = src_elm;
            ++tgt_elm;
          }
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
  void ColorMedianFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< D > &res, size_t thread,
                                 size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 2 );
      size_t features = res.Features( );
      size_t min_index = thread * src.size( ) / total_threads;
      size_t max_index = ( thread + 1 ) * src.size( ) / total_threads;
      COMMENT( "Computing median features.", 2 );
      Vector< double > median( adj_rel.size( ) );
      Vector< size_t > index( adj_rel.size( ) );
      size_t adj_base = ( adj_rel.size( ) / 2 ) - ( features / ( 2 * 3 ) );
      for( size_t src_elm = min_index; src_elm < max_index; ++src_elm ) {
        COMMENT( "Setting index.", 4 );
        res.Index( src_elm ) = src_elm;
        COMMENT( "Computing the sum of the distances of all channels from adjacent to source pixels.", 4 );
        for( size_t adj = 0; adj < adj_rel.size( ); ++adj ) {
          COMMENT( "Computing index of elements.", 4 );
          size_t elm_adj = adj_rel( src, src_elm, adj );
          if( elm_adj < src.size( ) ) {
            index( adj ) = elm_adj;
          }
          else {
            index( adj ) = src_elm;
          }
          COMMENT( "Computing the median vector fo the distances from the source element to target elements.", 4 );
          if( elm_adj < src.size( ) ) {
            for( size_t chn = 1; chn < 4; ++chn ) {
              D adj_value = src( elm_adj )( chn );
              D src_value = src( src_elm )( chn );
              median( adj ) += ( adj_value > src_value ) ? adj_value - src_value : src_value - adj_value;
            }
          }
          else {
            median( adj ) = 0;
          }
        }
        COMMENT( "Sorting the distances.", 4 );
        Sorting::Sort( index, Sorting::Sort( median ) );
        COMMENT( "Getting the features from the median distances.", 4 );
        for( size_t ftr = 0; ftr < features / 3; ++ftr ) {
          for( size_t chn = 1; chn < 4; ++chn ) {
            res( src_elm, ftr * 3 + ( chn - 1 ) ) = src( index( adj_base + ftr ) )( chn );
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

#ifdef BIAL_EXPLICIT_ColorMedianFeature

  template Feature< int > ColorMedianFeature( const Image< Color > &src, const Adjacency &adj_rel, float proportion );
  template Feature< llint > ColorMedianFeature( const Image< Color > &src, const Adjacency &adj_rel, float proportion );
  template Feature< float > ColorMedianFeature( const Image< Color > &src, const Adjacency &adj_rel, float proportion );
  template Feature< double > ColorMedianFeature( const Image< Color > &src, const Adjacency &adj_rel, 
                                                 float proportion );

  template Feature< int > ColorMedianFeature( const Image< Color > &src, const Image< int > &msk, 
                                              const Adjacency &adj_rel, float proportion );
  template Feature< llint > ColorMedianFeature( const Image< Color > &src, const Image< llint > &msk, 
                                                const Adjacency &adj_rel, float proportion );
  template Feature< float > ColorMedianFeature( const Image< Color > &src, const Image< float > &msk, 
                                                const Adjacency &adj_rel, float proportion );
  template Feature< double > ColorMedianFeature( const Image< Color > &src, const Image< double > &msk, 
                                                 const Adjacency &adj_rel, float proportion );

  template void ColorMedianFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< int > &res,
                                          size_t thread, size_t total_threads );
  template void ColorMedianFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< llint > &res,
                                          size_t thread, size_t total_threads );
  template void ColorMedianFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< float > &res,
                                          size_t thread, size_t total_threads );
  template void ColorMedianFeatureThread( const Image< Color > &src, const Adjacency &adj_rel, Feature< double > &res,
                                          size_t thread, size_t total_threads );


#endif

}

#endif

#endif
