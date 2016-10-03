/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2016/Sep/19
 * @brief MultiImage class declaration. Handler for images which data type is not known until run time.
 */

#ifndef BIALMULTIIMAGE_C
#define BIALMULTIIMAGE_C

#include "MultiImage.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_MultiImage )
#define BIAL_EXPLICIT_MultiImage
#endif

#if defined ( BIAL_EXPLICIT_MultiImage ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {
  
  MultiImage::MultiImage( ) try : 
    type( MultiImageType::none ), int_img( nullptr ), flt_img( nullptr ), clr_img( nullptr ), rcl_img( nullptr ) {
      COMMENT( "Creating empty multiimage.", 0 );
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

  MultiImage::MultiImage( const Image< int > &img ) try :
    type( MultiImageType::int_img ), int_img( new Image< int >( img ) ), flt_img( nullptr ), clr_img( nullptr ), 
      rcl_img( nullptr ) {
      COMMENT( "Creating int multiimage.", 0 );
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

  MultiImage::MultiImage( const Image< float > &img ) try :
    type( MultiImageType::flt_img ), int_img( nullptr ), flt_img( new Image< float >( img ) ), clr_img( nullptr ), 
      rcl_img( nullptr ) {
      COMMENT( "Creating float multiimage.", 0 );
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

  MultiImage::MultiImage( const Image< Color > &img )try :
    type( MultiImageType::clr_img ), int_img( nullptr ), flt_img( nullptr ), clr_img( new Image< Color >( img ) ), 
      rcl_img( nullptr ) {
      COMMENT( "Creating color multiimage.", 0 );
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

  MultiImage::MultiImage( const Image< RealColor > &img )try :
    type( MultiImageType::rcl_img ), int_img( nullptr ), flt_img( nullptr ), clr_img( nullptr ), 
      rcl_img( new Image< RealColor >( img ) ) {
      COMMENT( "Creating realcolor multiimage.", 0 );
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

  MultiImage::MultiImage( const MultiImage &mimg ) try : 
    type( mimg.type ), int_img( nullptr ), flt_img( nullptr ), clr_img( nullptr ), rcl_img( nullptr ) {
      switch( type ) {
      case MultiImageType::int_img:
        int_img = new Image< int >( *( mimg.int_img ) );
        break;
      case MultiImageType::flt_img:
        flt_img = new Image< float >( *( mimg.flt_img ) );
        break;
      case MultiImageType::clr_img:
        clr_img = new Image< Color >( *( mimg.clr_img ) );
        break;
      case MultiImageType::rcl_img:
        rcl_img = new Image< RealColor >( *( mimg.rcl_img ) );
        break;
      default:
        break;
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

  MultiImage::MultiImage( MultiImage &&mimg ) try : 
    type( std::move( mimg.type ) ), int_img( mimg.int_img ), flt_img( mimg.flt_img ), clr_img( mimg.clr_img ),
      rcl_img( mimg.rcl_img ) {
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

  MultiImage::~MultiImage( ) {
    COMMENT( "Deteting Multiimage.", 0 );
    switch( type ) {
    case MultiImageType::int_img:
      COMMENT( "liberating int.", 1 );
      free( int_img );
      int_img = nullptr;
      break;
    case MultiImageType::flt_img:
      COMMENT( "liberating flt.", 1 );
      free( flt_img );
      flt_img = nullptr;
      break;
    case MultiImageType::clr_img:
      COMMENT( "liberating clr.", 1 );
      free( clr_img );
      clr_img = nullptr;
      break;
    case MultiImageType::rcl_img:
      COMMENT( "liberating rcl.", 1 );
      free( rcl_img );
      rcl_img = nullptr;
      break;
    default:
      break;
    }
    type = MultiImageType::none;
  }

  MultiImage &MultiImage::operator=( const MultiImage &mimg ) {
    try {
      COMMENT( "Assigning Multiimage.", 0 );
      switch( type ) {
      case MultiImageType::int_img:
        COMMENT( "liberating int.", 1 );
        free( int_img );
        int_img = nullptr;
        break;
      case MultiImageType::flt_img:
        COMMENT( "liberating flt.", 1 );
        free( flt_img );
        flt_img = nullptr;
        break;
      case MultiImageType::clr_img:
        COMMENT( "liberating clr.", 1 );
        free( clr_img );
        clr_img = nullptr;
        break;
      case MultiImageType::rcl_img:
        COMMENT( "liberating rcl.", 1 );
        free( rcl_img );
        rcl_img = nullptr;
        break;
      default:
        break;
      }
      type = mimg.type;
      switch( type ) {
      case MultiImageType::int_img:
        int_img = new Image< int >( *( mimg.int_img ) );
        break;
      case MultiImageType::flt_img:
        flt_img = new Image< float >( *( mimg.flt_img ) );
        break;
      case MultiImageType::clr_img:
        clr_img = new Image< Color >( *( mimg.clr_img ) );
        break;
      case MultiImageType::rcl_img:
        rcl_img = new Image< RealColor >( *( mimg.rcl_img ) );
        break;
      default:
        break;
      }
      return( *this );
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

  MultiImage &MultiImage::operator=( MultiImage &&mimg ) {
    try {
      COMMENT( "Move assigning Multiimage.", 0 );
      switch( type ) {
      case MultiImageType::int_img:
        COMMENT( "liberating int.", 1 );
        free( int_img );
        int_img = nullptr;
        break;
      case MultiImageType::flt_img:
        COMMENT( "liberating flt.", 1 );
        free( flt_img );
        flt_img = nullptr;
        break;
      case MultiImageType::clr_img:
        COMMENT( "liberating clr.", 1 );
        free( clr_img );
        clr_img = nullptr;
        break;
      case MultiImageType::rcl_img:
        COMMENT( "liberating rcl.", 1 );
        free( rcl_img );
        rcl_img = nullptr;
        break;
      default:
        break;
      }
      type = std::move( mimg.type );
      int_img = mimg.int_img;
      flt_img = mimg.flt_img;
      clr_img = mimg.clr_img;
      rcl_img = mimg.rcl_img;
      mimg.type = MultiImageType::none;
      mimg.int_img = nullptr;
      mimg.flt_img = nullptr;
      mimg.clr_img = nullptr;
      mimg.rcl_img = nullptr;
      return( *this );
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

  const MultiImageType &MultiImage::Type( ) const {
    return( type );
  }

  Image< int > &MultiImage::IntImage( ) const {
#ifdef BIAL_DEBUG
    if( int_img == nullptr ) {
      std::string msg( BIAL_ERROR( "Integer image is null." ) );
      throw( std::runtime_error( msg ) );
    }
#endif
    return( *int_img );
  }

  Image< float > &MultiImage::FltImage( ) const {
#ifdef BIAL_DEBUG
    if( flt_img == nullptr ) {
      std::string msg( BIAL_ERROR( "Float image is null." ) );
      throw( std::runtime_error( msg ) );
    }
#endif
    return( *flt_img );
  }

  Image< Color > &MultiImage::ClrImage( ) const {
#ifdef BIAL_DEBUG
    if( clr_img == nullptr ) {
      std::string msg( BIAL_ERROR( "Color image is null." ) );
      throw( std::runtime_error( msg ) );
    }
#endif
    return( *clr_img );
  }

  Image< RealColor > &MultiImage::RclImage( ) const {
#ifdef BIAL_DEBUG
    if( rcl_img == nullptr ) {
      std::string msg( BIAL_ERROR( "RealColor image is null." ) );
      throw( std::runtime_error( msg ) );
    }
#endif
    return( *rcl_img );
  }

#ifdef BIAL_EXPLICIT_MultiImage

#endif

}

#endif

#endif
