/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/27 
 * @brief Methods for sampling elements. 
 */

#ifndef BIALSAMPLE_C
#define BIALSAMPLE_C

#include "Sample.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Sample )
#define BIAL_EXPLICIT_Sample
#endif
#if defined ( BIAL_EXPLICIT_Sample ) || ( BIAL_IMPLICIT_BIN )

#include "Feature.hpp"
namespace Bial {

  Sample::Sample( size_t elements ) try : sample( elements, Vector< size_t >( 1 ) ) {
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

  const size_t &Sample::operator[]( size_t elm ) const {
    try {
      return( sample( elm ) ( 0 ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  size_t &Sample::operator[]( size_t elm ) {
    try {
      return( sample( elm ) ( 0 ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  const size_t &Sample::operator()( size_t elm ) const {
    try {
      return( sample( elm ) ( 0 ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  size_t &Sample::operator()( size_t elm ) {
    try {
      return( sample( elm ) ( 0 ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  const size_t &Sample::operator()( size_t main_elm, size_t repeated_elm ) const {
    try {
      return( sample( main_elm ) ( repeated_elm ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  size_t &Sample::operator()( size_t main_elm, size_t repeated_elm ) {
    try {
      return( sample( main_elm ) ( repeated_elm ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  size_t Sample::Size( ) const {
    return( sample.size( ) );
  }

  size_t Sample::size( ) const {
    return( sample.size( ) );
  }

  size_t Sample::Size( size_t elm ) const {
    try {
      return( sample( elm ).size( ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  size_t Sample::size( size_t elm ) const {
    try {
      return( sample( elm ).size( ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
  }

  void Sample::push_back( ) {
    try {
      sample.push_back( Vector< size_t >( ) );
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

  void Sample::push_back( size_t pos, size_t new_elm  ) {
    try {
      sample( pos ).push_back( new_elm );
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


#ifdef BIAL_EXPLICIT_Sample

#endif

}

#endif

#endif
