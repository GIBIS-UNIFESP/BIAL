/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Functions to merge feature vectors.
 */

#ifndef BIALFEATUREMERGE_C
#define BIALFEATUREMERGE_C

#include "FeatureResize.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FeatureResize )
#define BIAL_EXPLICIT_FeatureResize
#endif

#if defined ( BIAL_EXPLICIT_FeatureResize ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"

namespace Bial {

  template< class D >
  Feature< D > FeatureResize::MergeElements( const Feature< D > &feat1, const Feature< D > &feat2 ) {
    try {
      COMMENT( "Checking if both feature vectors have the same amount of features.", 1 );
      if( feat1.Features( ) != feat2.Features( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of features in two input merged vectors. Features: " +
                                     std::to_string( feat1.Features( ) ) + " and " + 
                                     std::to_string( feat1.Features( ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating resultant feature vector.", 0 );
      Feature< D > res( feat1.Elements( ) + feat2.Elements( ), feat1.Features( ) );
      COMMENT( "Copying features and indexes from first vector to new vector.", 0 );
      size_t tgt_elm = 0;
      for( ; tgt_elm < feat1.Elements( ); ++tgt_elm ) {
        COMMENT( "Copying indexes and labels.", 4 );
        res.Index( tgt_elm ) = feat1.Index( tgt_elm );
        res.Label( tgt_elm ) = feat1.Label( tgt_elm );
        COMMENT( "Copying features.", 4 );
        for( size_t ftr = 0; ftr < res.Features( ); ++ftr ) {
          res( tgt_elm, ftr ) = feat1( tgt_elm, ftr );
        }
      }
      COMMENT( "Copying features and indexes from second vector to new vector.", 0 );
      for( size_t src_elm = 0; src_elm < feat2.Elements( ); ++tgt_elm, ++src_elm ) {
        COMMENT( "Copying indexes and labels.", 4 );
        res.Index( tgt_elm ) = feat1.Index( src_elm );
        res.Label( tgt_elm ) = feat1.Label( src_elm );
        COMMENT( "Copying features.", 4 );
        for( size_t ftr = 0; ftr < res.Features( ); ++ftr ) {
          res( tgt_elm, ftr ) = feat2( src_elm, ftr );
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
  Feature< D > FeatureResize::MergeFeatures( const Feature< D > &feat1, const Feature< D > &feat2 ) {
    try {
      COMMENT( "Checking if both feature vectors have the same amount of elements.", 1 );
      if( feat1.Elements( ) != feat2.Elements( ) ) {
        std::string msg( BIAL_ERROR( "Incompatible number of elements in two input merged vectors. Elements: " +
                                     std::to_string( feat1.Elements( ) ) + " and " + 
                                     std::to_string( feat1.Elements( ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating resultant feature vector.", 0 );
      Feature< D > res( feat1.Elements( ), feat1.Features( ) + feat2.Features( ) );
      COMMENT( "Copying features and indexes to new vector.", 0 );
      for( size_t elm = 0; elm < res.Elements( ); ++elm ) {
        COMMENT( "Copying indexes and labels.", 4 );
        res.Index( elm ) = feat1.Index( elm );
        res.Label( elm ) = feat1.Label( elm );
        COMMENT( "Copying features from first vector.", 4 );
        size_t tgt_ftr = 0;
        for( ; tgt_ftr < feat1.Features( ); ++tgt_ftr ) {
          res( elm, tgt_ftr ) = feat1( elm, tgt_ftr );
        }
        COMMENT( "Copying features from second vector.", 4 );
        for( size_t src_ftr = 0; src_ftr < feat2.Features( ); ++tgt_ftr, ++src_ftr ) {
          res( elm, tgt_ftr ) = feat2( elm, src_ftr );
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

#ifdef BIAL_EXPLICIT_FeatureResize

  template Feature< int > FeatureResize::MergeElements( const Feature< int > &feat1, const Feature< int > &feat2 );
  template Feature< llint > FeatureResize::MergeElements( const Feature< llint > &feat1, 
                                                          const Feature< llint > &feat2 );
  template Feature< float > FeatureResize::MergeElements( const Feature< float > &feat1, 
                                                          const Feature< float > &feat2 );
  template Feature< double > FeatureResize::MergeElements( const Feature< double > &feat1, 
                                                          const Feature< double > &feat2 );
  template Feature< int > FeatureResize::MergeFeatures( const Feature< int > &feat1, const Feature< int > &feat2 );
  template Feature< llint > FeatureResize::MergeFeatures( const Feature< llint > &feat1, 
                                                          const Feature< llint > &feat2 );
  template Feature< float > FeatureResize::MergeFeatures( const Feature< float > &feat1, 
                                                          const Feature< float > &feat2 );
  template Feature< double > FeatureResize::MergeFeatures( const Feature< double > &feat1, 
                                                          const Feature< double > &feat2 );

#endif

}

#endif

#endif
