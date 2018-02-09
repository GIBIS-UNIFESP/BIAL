/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2015/Oct/06
 * @brief Colorspace convertion methods. For real value color spaces.
 */

#ifndef BIALREALCOLOR_C
#define BIALREALCOLOR_C

#include "RealColor.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_RealColor )
#define BIAL_EXPLICIT_RealColor
#endif

#if defined ( BIAL_EXPLICIT_RealColor ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"

namespace Bial {

  RealColor::RealColor( float value ) : channel( value ) {
  }

  RealColor::RealColor( float chl_0, float chl_1, float chl_2, float chl_3 ) : channel( chl_0 ) {
    channel[ 1 ] = chl_1;
    channel[ 2 ] = chl_2;
    channel[ 3 ] = chl_3;
  }

  RealColor::RealColor( const RealColor &other ) : channel( other.channel ) {
  }

  RealColor::RealColor( RealColor &&other ) : channel( std::move( other.channel ) ) {
  }

  RealColor::RealColor( const Color &other ) 
    : RealColor( static_cast< float >( other( 0 ) ), static_cast< float >( other( 1 ) ), 
                 static_cast< float >( other( 2 ) ), static_cast< float >( other( 3 ) ) ) {
  }

  RealColor &RealColor::operator=( const RealColor &other ) {
    channel = other.channel;
    return( *this );
  }

  RealColor &RealColor::operator=( RealColor &&other ) {
    channel = std::move( other.channel );
    return( *this );
  }

  const float &RealColor::operator[]( size_t chl ) const {
#ifdef BIAL_DEBUG
    if( chl > 3 ) {
      std::string msg( BIAL_ERROR( "Cannot acces channel " + std::to_string( chl ) + "." ) );
      throw( std::runtime_error( msg ) );
  }
#endif
    return( channel[ chl ] );
  }

  float &RealColor::operator[]( size_t chl ) {
#ifdef BIAL_DEBUG
    if( chl > 3 ) {
      std::string msg( BIAL_ERROR( "Cannot acces channel " + std::to_string( chl ) + "." ) );
      throw( std::runtime_error( msg ) );
  }
#endif
    return( channel[ chl ] );
  }

  const float &RealColor::operator()( size_t chl ) const {
#ifdef BIAL_DEBUG
    if( chl > 3 ) {
      std::string msg( BIAL_ERROR( "Cannot acces channel " + std::to_string( chl ) + "." ) );
      throw( std::runtime_error( msg ) );
  }
#endif
    return( channel[ chl ] );
  }

  float &RealColor::operator()( size_t chl ) {
#ifdef BIAL_DEBUG
    if( chl > 3 ) {
      std::string msg( BIAL_ERROR( "Cannot acces channel " + std::to_string( chl ) + "." ) );
      throw( std::runtime_error( msg ) );
  }
#endif
    return( channel[ chl ] );
  }

  RealColor operator+( const RealColor &clr1, const RealColor &clr2 ) {
    RealColor res( clr1 );
    return( res += clr2 );
  }

  RealColor &operator+=( RealColor &clr1, const RealColor &clr2 ) {
    for( size_t chl = 0; chl < 4; ++chl )
      clr1[ chl ] += clr2[ chl ];
    return( clr1 );
  }

  RealColor operator-( const RealColor &clr1, const RealColor &clr2 ) {
    RealColor res( clr1 );
    return( res -= clr2 );
  }

  RealColor &operator-=( RealColor &clr1, const RealColor &clr2 ) {
    for( size_t chl = 0; chl < 4; ++chl )
      clr1[ chl ] -= clr2[ chl ];
    return( clr1 );
  }

  RealColor operator*( const RealColor &clr1, const RealColor &clr2 ) {
    RealColor res( clr1 );
    return( res *= clr2 );
  }

  RealColor &operator*=( RealColor &clr1, const RealColor &clr2 ) {
    for( size_t chl = 0; chl < 4; ++chl )
      clr1[ chl ] *= clr2[ chl ];
    return( clr1 );
  }

  RealColor operator/( const RealColor &clr1, const RealColor &clr2 ) {
    try {
      RealColor res( clr1 );
      return( res /= clr2 );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
  }

  RealColor &operator/=( RealColor &clr1, const RealColor &clr2 ) {
    for( size_t chl = 0; chl < 4; ++chl ) {
#ifdef BIAL_DEBUG
      if( clr2( chl ) == 0 ) {
        std::string msg( BIAL_ERROR( "Division by zero at channel " + std::to_string( chl ) + "." ) );
        throw( std::runtime_error( msg ) );
      }
#endif
      clr1[ chl ] /= clr2[ chl ];
    }
    return( clr1 );
  }

  bool operator==( const RealColor &clr1, const RealColor &clr2 ) {
    for( size_t clr = 0; clr < 4; ++clr ) {
      if( clr1[ clr ] != clr2[ clr ] )
        return( false );
    }
    return( true );
  }

  bool operator!=( const RealColor &clr1, const RealColor &clr2 ) {
    for( size_t clr = 0; clr < 4; ++clr ) {
      if( clr1[ clr ] != clr2[ clr ] )
        return( true );
    }
    return( false );
  }

  RealColor operator+( const RealColor &clr, double scalar ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      RealColor res( clr );
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

  RealColor operator+( double scalar, const RealColor &clr ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      RealColor res( clr );
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

  RealColor &operator+=( RealColor &clr, double scalar ) {
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

  RealColor operator-( const RealColor &clr, double scalar ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      RealColor res( clr );
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

  RealColor operator-( double scalar, const RealColor &clr ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      RealColor res( clr );
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

  RealColor &operator-=( RealColor &clr, double scalar ) {
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

  RealColor operator*( const RealColor &clr, double scalar ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      RealColor res( clr );
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

  RealColor operator*( double scalar, const RealColor &clr ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      RealColor res( clr );
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

  RealColor &operator*=( RealColor &clr, double scalar ) {
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

  RealColor operator/( const RealColor &clr, double scalar ) {
    try {
      COMMENT( "Verifying if scalar is not zero.", 2 );
      if( scalar == 0.0 ) {
        std::string msg( BIAL_ERROR( "Division by zero." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing resultant color.", 2 );
      RealColor res( clr );
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

  RealColor operator/( double scalar, const RealColor &clr ) {
    try {
      COMMENT( "Initializing resultant color.", 2 );
      RealColor res( clr );
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

  RealColor &operator/=( RealColor &clr, double scalar ) {
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
