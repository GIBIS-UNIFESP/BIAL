/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/26 
 * @brief OPF cluster matching verification algorithms. 
 */

#ifndef BIALOPFCLUSTERMATCHING_C
#define BIALOPFCLUSTERMATCHING_C

#include "OPFClusterMatching.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_OPFClusterMatching )
#define BIAL_EXPLICIT_OPFClusterMatching
#endif
#if defined ( BIAL_EXPLICIT_OPFClusterMatching ) || ( BIAL_IMPLICIT_BIN )

#include "Matrix.hpp"

namespace Bial {

  template< template< class D > class C >
  float OPF::ClusterMatchingError( const Vector< C< int > > &sim_label ) {
    try {
      float max_change = 0.0;
      COMMENT( "Checking for maximum similarity between pairs of labels.", 0 );
      size_t best_match = 0;
      for( size_t src_lbl = 0; src_lbl < sim_label.size( ); ++src_lbl ) {
        for( size_t tgt_lbl = src_lbl + 1; tgt_lbl < sim_label.size( ); ++tgt_lbl ) {
          COMMENT( "Initializing best match for this pair of labels.", 3 );
          best_match = 0;
          COMMENT( "Creating similarity matrix.", 3 );
          Matrix< size_t > lbl_match( sim_label( src_lbl ).Maximum( ) + 1, sim_label( tgt_lbl ).Maximum( ) + 1 );
          Vector< Vector< size_t > > avb_match( sim_label( src_lbl ).Maximum( ) + 1 );
          lbl_match.Set( 0 );
          COMMENT( "Matching pixels for similarity.", 3 );
          for( size_t pxl = 0; pxl < sim_label( src_lbl ).size( ); ++pxl ) {
            ++lbl_match( sim_label( src_lbl )( pxl ), sim_label( tgt_lbl )( pxl ) );
          }
          COMMENT( "Setting available matches and computing the total number of matches.", 3 );
          size_t total_matches = 1;
          for( size_t src_idx = 0; src_idx < lbl_match.size( 0 ); ++src_idx ) {
            for( size_t tgt_idx = 0; tgt_idx < lbl_match.size( 1 ); ++tgt_idx ) {
              if( lbl_match( src_idx, tgt_idx ) != 0 ) {
                avb_match( src_idx ).push_back( tgt_idx );
              }
            }
            avb_match( src_idx ).push_back( lbl_match.size( 1 ) );
            total_matches *= avb_match( src_idx ).size( );
          }
          COMMENT( "Selecting the best match.", 3 );
          Vector< size_t > index( lbl_match.size( 0 ), 0 );
          for( size_t elm = 0; elm < total_matches; ++elm ) {
            COMMENT( "Position zero of valid_index is used for global validation. Other indexes are at index + 1.", 4 );
            Vector< bool > valid_index( lbl_match.size( 0 ) + 1, true );
            COMMENT( "Checking if indexes are all different and computing the matching value.", 4 );
            size_t match = 0;
            for( size_t src_idx = 0; src_idx < index.size( ); ++src_idx ) {
              size_t tgt_idx = avb_match( src_idx )( index( src_idx ) );
              COMMENT( "Skipping non-matched indexes.", 4 );
              if( tgt_idx == lbl_match.size( 1 ) ) {
                continue;
              }
              COMMENT( "Updating matching.", 4 );
              match += lbl_match( src_idx, tgt_idx );
              COMMENT( "Verifying indexes.", 4 );
              if( valid_index( tgt_idx + 1 ) ) {
                valid_index( tgt_idx + 1 ) = false;
              }
              else {
                valid_index( 0 ) = false;
                break;
              }
            }
            COMMENT( "Updating best match.", 3 );
            if( valid_index( 0 ) ) {
              COMMENT( "Updating: index: " << index << ": best_match: " << best_match << ", match: " << match, 3 );
              if( best_match < match ) {
                best_match = match;
              }
            }
            COMMENT( "Stepping to the next index.", 3 );
            size_t src_idx = 0;
            while( ( src_idx < index.size( ) ) && ( avb_match( src_idx )( index( src_idx ) ) == lbl_match.size( 1 ) ) )
              index( src_idx++ ) = 0;
            if( src_idx < index.size( ) ) {
              ++index( src_idx );
            }
          }
        }
        COMMENT( "Updating maximum change for the three containers.", 0 );
        float pair_change = 1.0 - static_cast< float >( best_match ) / sim_label( 0 ).size( );
        COMMENT( "best_match: " << best_match << ", sim_label[ 0 ].size( ): " << sim_label( 0 ).size( ), 1 );
        COMMENT( "pair_change: " << pair_change << ", max_change: " << max_change, 1 );
        if( max_change < pair_change ) {
          max_change = pair_change;
        }
      }
      return( max_change );
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

#ifdef BIAL_EXPLICIT_OPFClusterMatching

  template float OPF::ClusterMatchingError( const Vector< Image< int > > &sim_label );

#endif

}

#endif

#endif
