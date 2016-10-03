/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Functions for Feature I/O.
 */

#ifndef BIALFILEFEATURE_C
#define BIALFILEFEATURE_C

#include "FileFeature.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FileFeature )
#define BIAL_EXPLICIT_FileFeature
#endif

#if defined ( BIAL_EXPLICIT_FileFeature ) || ( BIAL_IMPLICIT_BIN )

#include "File.hpp"
#include "Feature.hpp"

namespace Bial {

  template< class D >
  Feature< D > FileFeature::Read( const std::string &filename ) {
    try {
      COMMENT( "Opening file.", 1 );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );

      COMMENT( "Reading header information: nodes, classes and feats.", 1 );
      int nnodes;
      file.read( reinterpret_cast< char* >( &nnodes ), sizeof( int ) );
      int nlabels;
      file.read( reinterpret_cast< char* >( &nlabels ), sizeof( int ) );
      int nfeats;
      file.read( reinterpret_cast< char* >( &nfeats ), sizeof( int ) );
      COMMENT( "nnodes: " << nnodes << ", nlabels: " << nlabels << ", nfeats: " << nfeats, 2 );
      Feature< D > feat( nnodes, nfeats );
      feat.Labels( nlabels );
      
      COMMENT( "Reading the features.", 1 );
      int index;
      int label;
      float ffeat;
      for( size_t elm = 0; elm < static_cast< size_t >( nnodes ); ++elm ) {
        file.read( reinterpret_cast< char* >( &index ), sizeof( int ) );
        feat.Index( elm ) = elm; /* Ignoring index. It must be sequential starting at 0. */
        file.read( reinterpret_cast< char* >( &label ), sizeof( int ) );
        feat.Label( elm ) = label;
        for( size_t ftr = 0; ftr < static_cast< size_t >( nfeats ); ++ftr ) {
          file.read( reinterpret_cast< char* >( &ffeat ), sizeof( float ) );
          feat( elm, ftr ) = static_cast< D >( ffeat );
        }
      }
      COMMENT( "Closing and returning.", 1 );
      file.close( );
      return( feat );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing file." ) );
      throw( std::ios_base::failure( msg ) );
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
  void FileFeature::Write( const Feature< D > &feat, const std::string &filename ) {
    try {
      COMMENT( "Opening file.", 1 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( filename );

      size_t nnodes = feat.Elements( );
      size_t nlabels = feat.Labels( );
      size_t feats = feat.Features( );
      COMMENT( "Writing header information: nodes, classes and feats.", 1 );
      file.write( reinterpret_cast< char* >( &nnodes ), sizeof( int ) );
      file.write( reinterpret_cast< char* >( &nlabels ), sizeof( int ) );
      file.write( reinterpret_cast< char* >( &feats ), sizeof( int ) );

      COMMENT( "Writing the features.", 1 );
      for( size_t elm = 0; elm < nnodes; ++elm ) {
        int index = feat.Index( elm );
        file.write( reinterpret_cast< char* >( &index ), sizeof( int ) );
        int label = feat.Label( elm );
        file.write( reinterpret_cast< char* >( &label ), sizeof( int ) );
        for( size_t ftr = 0; ftr < feats; ++ftr ) {
          float feature = feat( elm, ftr );
          file.write( reinterpret_cast< char* >( &feature ), sizeof( float ) );
        }
      }
      COMMENT( "Closing and returning.", 1 );
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing file." ) );
      throw( std::ios_base::failure( msg ) );
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

#ifdef BIAL_EXPLICIT_FileFeature

  template Feature< int > FileFeature::Read( const std::string &filename );
  template Feature< llint > FileFeature::Read( const std::string &filename );
  template Feature< float > FileFeature::Read( const std::string &filename );
  template Feature< double > FileFeature::Read( const std::string &filename );

  template void FileFeature::Write( const Feature< int > &feat, const std::string &filename );
  template void FileFeature::Write( const Feature< llint > &feat, const std::string &filename );
  template void FileFeature::Write( const Feature< float > &feat, const std::string &filename );
  template void FileFeature::Write( const Feature< double > &feat, const std::string &filename );

#endif

}

#endif

#endif
