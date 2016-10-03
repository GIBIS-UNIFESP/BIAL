 /* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief: Segmentation with minimum spanning tree.
 */

#ifndef BIALSEGMENTATIONMST_C
#define BIALSEGMENTATIONMST_C

#include "SegmentationMST.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_SegmentationMST )
#define BIAL_EXPLICIT_SegmentationMST
#endif
#if defined ( BIAL_EXPLICIT_SegmentationMST ) || ( BIAL_IMPLICIT_BIN )

#include "BucketQueue.hpp"
#include "Feature.hpp"
#include "Image.hpp"
#include "MinimumSpanningTree.hpp"
#include "SortingSort.hpp"

namespace Bial {

  template< class D >
  Image< int > Segmentation::MinimumSpanningForest( const Image< D > &img, const Feature< D > feature, 
                                                    size_t regions ) {
    try {
      if( ( regions == 0 ) || ( regions > img.size( ) ) ) {
        std::string msg
          ( BIAL_ERROR( "Number of regions must be greater than zero and lower than the number of elements." ) );
        throw( std::logic_error( msg ) );
      }
      if( regions == 1 ) {
        Image< int > label( img );
        label.Set( 0 );
        return( label );
      }
      COMMENT( "Computing MST.", 0 );
      Image< D > distance( img );
      distance.Set( std::numeric_limits< D >::max( ) );
      Image< int > predecessor( img );
      MinimumSpanningTree::MST( feature, distance, predecessor );
      COMMENT( "Cutting MST into the desired number of regions.", 0 );
      Vector< size_t > order = Sorting::Sort( distance );
      for( size_t rgn = 0; rgn < regions - 1; ++rgn ) {
        predecessor[ order( order.size( ) - rgn - 1 ) ] = -1;
      }
      COMMENT( "Labeling clusters.", 0 );
      Image< int > label( img );
      label.Set( -1 );
      size_t lbl = 0;
      COMMENT( "Setting root labels.", 0 );
      for( size_t elm = 0; elm < label.size( ); ++elm ) {
        if( predecessor[ elm ] == -1 ) {
          label[ elm ] = lbl;
          ++lbl;
        }
      }
      COMMENT( "Setting the other labels.", 0 );
      BucketQueue queue( img.size( ) );
      for( size_t elm = 0; elm < label.size( ); ++elm ) {
        COMMENT( "Looking for root, while inserting all elements in queue.", 3 );
        size_t root = elm;
        while( ( label[ root ] == -1 ) && ( predecessor[ root ] != -1 ) ) {
          queue.Insert( root, 1 );
          root = predecessor[ root ];
        }
        COMMENT( "Getting elements from queue and setting root label to them.", 3 );
        while( !queue.Empty( ) ) {
          size_t pos = queue.Remove( );
          label[ pos ] = label[ root ];
        }
      }
      return( label );
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


#ifdef BIAL_EXPLICIT_SegmentationMST

  template Image< int > Segmentation::MinimumSpanningForest( const Image< int > &img, const Feature< int > feature, 
                                                             size_t regions );
  template Image< int > Segmentation::MinimumSpanningForest( const Image< llint > &img, const Feature< llint > feature,
                                                             size_t regions );
  template Image< int > Segmentation::MinimumSpanningForest( const Image< float > &img, const Feature< float > feature,
                                                             size_t regions );
  template Image< int > Segmentation::MinimumSpanningForest( const Image< double > &img, 
                                                             const Feature< double > feature, size_t regions );

#endif

}

#endif

#endif
