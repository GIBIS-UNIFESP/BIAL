/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file PNMHeader.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2013/Aug/03 
 * @version 1.0.00
 * @brief
 * Content: PNM Header class 
 * <br> Description: PNM Header class. 
 * <br> Future add-on's: Read function, or read string constructor. 
 */

#ifndef BIALPNMHEADER_C
#define BIALPNMHEADER_C

#include "PNMHeader.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_PNMHeader )
#define BIAL_EXPLICIT_PNMHeader
#endif
#if defined ( BIAL_EXPLICIT_PNMHeader ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  void PNMHeader::Comment( const std::string &cmt, const size_t line ) {
    try {
      if( comment.size( ) <= line ) {
        comment.insert( comment.end( ), line - comment.size( ) + 1, "" );
      }
      comment( line ) = cmt;
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
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Empty kernel." ) );
      throw( std::logic_error( msg ) );
    }
  }

  std::string PNMHeader::Comment( const size_t line ) const {
    try {
      if( comment.size( ) <= line ) {
        return( "" );
      }
      return( comment( line ) );
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
  }


  int PNMHeader::First( ) const {
    try {
      for( size_t i = 0; i < comment.size( ); ++i ) {
        if( comment( i ).size( ) > 0 ) {
          return( i );
        }
      }
      return( -1 );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
  }


  int PNMHeader::Last( ) const {
    try {
      for( int i = comment.size( ) - 1; i >= 0; --i ) {
        if( comment( i ).size( ) > 0 ) {
          return( i );
        }
      }
      return( -1 );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
  }

}

#endif

#endif
