/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date: 2015/Oct/06
 * @brief Colorspace convertion methods. For integer colors.
 */

#ifndef BIALCOLOR_C
#define BIALCOLOR_C

#include "Color.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Color )
#define BIAL_EXPLICIT_Color
#endif

#if defined ( BIAL_EXPLICIT_Color ) || ( BIAL_IMPLICIT_BIN )

#include "RealColor.hpp"

namespace Bial {

  Color::Color( ) : value( 0u ) {
  }

  Color::Color( uint val ) : value( val ) {
  }

  Color::Color( uchar alpha, uchar red, uchar green, uchar blue ) : value( 0 ) {
    channel[ 0 ] = alpha;
    channel[ 1 ] = red;
    channel[ 2 ] = green;
    channel[ 3 ] = blue;
  }

  Color::Color( const Color &other ) : value( other.value ) {
  }

  Color::Color( Color &&other ) : value( std::move( other.value ) ) {
  }

  Color::Color( const RealColor &other ) 
    : Color( static_cast< uchar >( other( 0 ) ), static_cast< uchar >( other( 1 ) ), 
             static_cast< uchar >( other( 2 ) ), static_cast< uchar >( other( 3 ) ) ) {
  }

  Color &Color::operator=( const Color &other ) {
    value = other.value;
    return( *this );
  }

  Color &Color::operator=( Color &&other ) {
    value = std::move( other.value );
    return( *this );
  }

  const uchar &Color::operator[]( size_t chl ) const {
#ifdef BIAL_DEBUG
    if( chl > 3 ) {
      std::string msg( BIAL_ERROR( "Cannot acces channel " + std::to_string( chl ) + "." ) );
      throw( std::runtime_error( msg ) );
  }
#endif
    return( channel[ chl ] );
  }

  uchar &Color::operator[]( size_t chl ) {
#ifdef BIAL_DEBUG
    if( chl > 3 ) {
      std::string msg( BIAL_ERROR( "Cannot acces channel " + std::to_string( chl ) + "." ) );
      throw( std::runtime_error( msg ) );
  }
#endif
    return( channel[ chl ] );
  }

  const uchar &Color::operator()( size_t chl ) const {
#ifdef BIAL_DEBUG
    if( chl > 3 ) {
      std::string msg( BIAL_ERROR( "Cannot acces channel " + std::to_string( chl ) + "." ) );
      throw( std::runtime_error( msg ) );
  }
#endif
    return( channel[ chl ] );
  }

  uchar &Color::operator()( size_t chl ) {
#ifdef BIAL_DEBUG
    if( chl > 3 ) {
      std::string msg( BIAL_ERROR( "Cannot acces channel " + std::to_string( chl ) + "." ) );
      throw( std::runtime_error( msg ) );
  }
#endif
    return( channel[ chl ] );
  }

  void Color::Value( uint val ) {
    value = val;
  }

  uint Color::Value( ) const {
    return( value );
  }

  Color operator+( const Color &clr1, const Color &clr2 ) {
    Color res( clr1 );
    return( res += clr2 );
  }

  Color &operator+=( Color &clr1, const Color &clr2 ) {
    for( size_t chl = 0; chl < 4; ++chl )
      clr1.channel[ chl ] += clr2.channel[ chl ];
    return( clr1 );
  }

  Color operator-( const Color &clr1, const Color &clr2 ) {
    Color res( clr1 );
    return( res -= clr2 );
  }

  Color &operator-=( Color &clr1, const Color &clr2 ) {
    for( size_t chl = 0; chl < 4; ++chl )
      clr1.channel[ chl ] -= clr2.channel[ chl ];
    return( clr1 );
  }

  Color operator*( const Color &clr1, const Color &clr2 ) {
    Color res( clr1 );
    return( res *= clr2 );
  }

  Color &operator*=( Color &clr1, const Color &clr2 ) {
    for( size_t chl = 0; chl < 4; ++chl )
      clr1.channel[ chl ] *= clr2.channel[ chl ];
    return( clr1 );
  }

  Color operator/( const Color &clr1, const Color &clr2 ) {
    try {
      Color res( clr1 );
      return( res /= clr2 );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
  }

  Color &operator/=( Color &clr1, const Color &clr2 ) {
    for( size_t chl = 0; chl < 4; ++chl ) {
#ifdef BIAL_DEBUG
      if( clr2( chl ) == 0 ) {
        std::string msg( BIAL_ERROR( "Division by zero at channel " + std::to_string( chl ) + "." ) );
        throw( std::runtime_error( msg ) );
      }
#endif
      clr1.channel[ chl ] /= clr2.channel[ chl ];
    }
    return( clr1 );
  }

  bool operator==( const Color &clr1, const Color &clr2 ) {
    for( size_t clr = 0; clr < 4; ++clr ) {
      if( clr1[ clr ] != clr2[ clr ] )
        return( false );
    }
    return( true );
  }

  bool operator!=( const Color &clr1, const Color &clr2 ) {
    for( size_t clr = 0; clr < 4; ++clr ) {
      if( clr1[ clr ] != clr2[ clr ] )
        return( true );
    }
    return( false );
  }

  Color operator+( const Color &clr, double scalar ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      Color res( clr );
      return( res += scalar );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color operator+( double scalar, const Color &clr ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      Color res( clr );
      return( res += scalar );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color &operator+=( Color &clr, double scalar ) {
    try {
      COMMENT( "Performing addition.", 2 );
      for( size_t chl = 0; chl < 4; ++chl ) {
        clr( chl ) += scalar;
      }
      return( clr );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color operator-( const Color &clr, double scalar ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      Color res( clr );
      return( res -= scalar );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color operator-( double scalar, const Color &clr ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      Color res( clr );
      COMMENT( "Performing subtraction.", 2 );
      for( size_t chl = 0; chl < 4; ++chl ) {
        res( chl ) = scalar - res( chl );
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
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color &operator-=( Color &clr, double scalar ) {
    try {
      COMMENT( "Performing subtraction.", 2 );
      for( size_t chl = 0; chl < 4; ++chl ) {
        clr( chl ) -= scalar;
      }
      return( clr );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color operator*( const Color &clr, double scalar ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      Color res( clr );
      COMMENT( "Performing multiplication.", 2 );
      return( res *= scalar );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color operator*( double scalar, const Color &clr ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      Color res( clr );
      COMMENT( "Performing multiplication.", 2 );
      return( res *= scalar );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color &operator*=( Color &clr, double scalar ) {
    try {
      for( size_t chl = 0; chl < 4; ++chl ) {
        clr( chl ) *= scalar;
      }
      return( clr );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color operator/( const Color &clr, double scalar ) {
    try {
      COMMENT( "Verifying if scalar is not zero.", 2 );
      if( scalar == 0.0 ) {
        std::string msg( BIAL_ERROR( "Division by zero." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing resultant color.", 2 );
      Color res( clr );
      COMMENT( "Performing division.", 2 );
      return( res /= scalar );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color operator/( double scalar, const Color &clr ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      Color res( clr );
#ifdef BIAL_DEBUG
      COMMENT( "Performing division.", 2 );
      for( size_t chl = 0; chl < 4; ++chl ) {
        if( res( chl ) == 0 ) {
          std::string msg( BIAL_ERROR( "Division by zero at pixel " + std::to_string( chl ) + "." ) );
          throw( std::logic_error( msg ) );
        }
        res( chl ) = scalar / res( chl );
      }
#else
      COMMENT( "Performing division.", 2 );
      for( size_t chl = 0; chl < 4; ++chl ) {
        res( chl ) = scalar / res( chl );
      }
#endif
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
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  Color &operator/=( Color &clr, double scalar ) {
    try {
      COMMENT( "Verifying if scalar is not zero.", 2 );
      if( scalar == 0.0 ) {
        std::string msg( BIAL_ERROR( "Division by zero." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Performing division.", 2 );
      for( size_t chl = 0; chl < 4; ++chl ) {
        clr( chl ) /= scalar;
      }
      return( clr );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

}

#endif

#endif
