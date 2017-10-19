/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2014/Apr/07
 * @brief Image class declaration. Handles 2D and 3D images.
 */

#ifndef BIALIMAGE_C
#define BIALIMAGE_C

#include "Image.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Image )
#define BIAL_EXPLICIT_Image
#endif

#if defined ( BIAL_EXPLICIT_Image ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"
#include "RealColor.hpp"

namespace Bial {

  template< class D >
  void Image< D >::CreateTables( ) {
    DestroyTables( );
    size_t x_size = _data.dim_size( 0 );
    size_t y_size = _data.dim_size( 1 );
    size_t z_size = _data.dim_size( 2 );
    y_table = new D*[ y_size * z_size ];
    z_table = new D**[ z_size ];
    COMMENT( "Setting y table: ", 4 );
    size_t pos = 0;
    for( size_t z = 0; z < z_size; ++z ) {
      for( size_t y = 0; y < y_size; ++y ) {
        y_table[ y + z * y_size ] = &qk_data[ pos ];
        pos += x_size;
      }
    }
    COMMENT( "Setting z table: ", 4 );
    pos = 0;
    for( size_t z = 0; z < z_size; ++z ) {
      z_table[ z ] = &y_table[ pos ];
      pos += y_size;
    }
  }

  template< class D >
  void Image< D >::DestroyTables( ) {
    COMMENT( "Destroying tables.", 4 );
    if( z_table != nullptr )
      delete[]( z_table );
    z_table = nullptr;
    if( y_table != nullptr )
      delete[]( y_table );
    y_table = nullptr;
  }

  template< class D > Image< D >::Image( ) 
    : _data( 1, 1, 1 ), qk_data( nullptr ), pixel_size( 3, 1.0 ), y_table( nullptr ), z_table( nullptr ) {
    COMMENT( "Setting references.", 2 );
    COMMENT( "Assigning quick access pointers.", 2 );
    qk_data = &_data[ 0 ];
    CreateTables( );
  }

  template< class D > Image< D >::Image( const Vector< size_t > &spc_dim ) try 
    : _data( 1, 1, 1 ), qk_data( nullptr ), pixel_size( 3, 1.0 ), y_table( nullptr ), z_table( nullptr ) {
      size_t dimensions = spc_dim.size( );
      COMMENT( "Checking for number of spatial dimensions.", 2 );
      if( ( dimensions < 2 ) || ( dimensions > 3 ) ) {
        std::string msg( BIAL_ERROR( "Must have 2 or 3 dimensions. Given: " + std::to_string( dimensions ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating data matrix.", 2 );
      Vector< size_t > dims( spc_dim );
      if( dimensions == 2 )
        dims.push_back( 1 );
      COMMENT( "Check for zero size dimensions.", 2 );
      for( size_t dms = 0; dms < 3; ++dms ) {
        if( dims[ dms ] == 0 ) {
          std::string msg( BIAL_ERROR( "Dimension " + std::to_string( dms ) + " with zero elements." ) );
          throw( std::logic_error( msg ) );
        }
      }
      COMMENT( "Assigning data matix and image size.", 2 );
      _data = Matrix< D >( dims );

      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
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

  template< class D > Image< D >::Image( D *new_data, const Vector< size_t > &new_dim ) try
    : _data( new_data, new_dim ), qk_data( new_data ), pixel_size( 3, 1.0 ), y_table( nullptr ), z_table( nullptr ) {
      size_t dimensions = new_dim.size( );
      if( ( dimensions < 2 ) || ( dimensions > 3 ) ) {
        std::string msg( BIAL_ERROR( "Must have 2 or 3 dimensions. Given" + std::to_string( dimensions ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Creating data matrix.", 2 );
      Vector< size_t > dims( new_dim );
      if( dimensions == 2 )
        dims.push_back( 1 );
      COMMENT( "Check for zero size dimensions.", 2 );
      for( size_t dms = 0; dms < 3; ++dms ) {
        if( dims[ dms ] == 0 ) {
          std::string msg( BIAL_ERROR( "Dimension " + std::to_string( dms ) + " with zero elements." ) );
          throw( std::logic_error( msg ) );
        }
      }
      COMMENT( "Assigning data matix and image size.", 2 );
      _data = Matrix< D >( new_data, dims );
      CreateTables( );
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

  template< class D > Image< D >::Image( const Vector< size_t > &spc_dim, const Vector< float > &pixel_size ) try : 
    Image( spc_dim ) {
      COMMENT( "Checking for pixel size dimensions.", 2 );
      size_t pxl_size = pixel_size.size( );
      if( ( pxl_size < 2 ) || ( pxl_size > 3 ) ) {
        std::string msg( BIAL_ERROR( "Pixel size must have 2 or 3 elements. Given: " + std::to_string( pxl_size ) ) );
        throw( std::logic_error( msg ) );
      }
      this->pixel_size = pixel_size;
      if( pxl_size == 2 )
        this->pixel_size.push_back( 1.0f );
      CreateTables( );
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

  template< class D > Image< D >::Image( size_t spc_dim_0, size_t spc_dim_1 ) try 
    : _data( spc_dim_0, spc_dim_1, 1 ), qk_data( nullptr ), pixel_size( Vector< float >( 3, 1.0 ) ), y_table( nullptr ), 
        z_table( nullptr ) {
      COMMENT( "Check for zero size dimensions.", 4 );
      if( ( spc_dim_0 == 0 ) || ( spc_dim_1 == 0 ) ) {
        std::string msg( BIAL_ERROR( "Zero element spatial dimension(s): " + std::to_string( spc_dim_0 ) + ", " +
                                     std::to_string( spc_dim_1 ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Assigning quick access pointers.", 4 );
      qk_data = &_data[ 0 ];
      CreateTables( );
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

  template< class D > Image< D >::Image( size_t spc_dim_0, size_t spc_dim_1, size_t spc_dim_2 ) try 
    : _data( spc_dim_0, spc_dim_1, spc_dim_2 ), qk_data( nullptr ), pixel_size( Vector< float >( 3, 1.0 ) ),
        y_table( nullptr ), z_table( nullptr ) {
      COMMENT( "Check for zero size dimensions.", 2 );
      if( ( spc_dim_0 == 0 ) || ( spc_dim_1 == 0 ) || ( spc_dim_2 == 0 ) ) {
        std::string msg( BIAL_ERROR( "Zero element spatial dimension(s): " + std::to_string( spc_dim_0 ) + ", " +
                                     std::to_string( spc_dim_1 ) + ", " + std::to_string( spc_dim_2 ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
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

  template< class D > Image< D >::Image( const Matrix< D > &mtx ) try 
    : _data( mtx ), qk_data( nullptr ), pixel_size( 3, 1.0 ), y_table( nullptr ), z_table( nullptr ) {
      COMMENT( "Checking number of dimensions of the image.", 2 );
      size_t dimensions = _data.dims;
      if( ( dimensions < 2 ) || ( dimensions > 3 ) ) {
        std::string msg( BIAL_ERROR( "Image must have 2 to 3 dimensions. Given: " + std::to_string( dimensions ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Updating 2D to 3D matrix.", 0 );
      if( mtx.dim_size.size( ) == 2 ) {
        Matrix< D > mat( mtx.size( 0 ), mtx.size( 1 ), 1 );
        for( size_t elm = 0; elm < mat.size( ); ++elm )
          mat[ elm ] = mtx[ elm ];
        _data = mat;
      }
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
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

  template< class D > Image< D >::Image( const Matrix< D > &mtx, const Vector< float > &pixel_size ) try :
    Image( mtx ) {
      COMMENT( "Checking number of dimensions of the pixel.", 2 );
      size_t dims = pixel_size.size( );
      if( ( dims < 2 ) || ( dims > 3 ) ) {
        std::string msg( BIAL_ERROR( "Pixel must have 2 to 3 dimensions. Given: " + std::to_string( dims ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      this->pixel_size = pixel_size;
      if( dims == 2 )
        this->pixel_size.push_back( 1.0f );
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
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

  template< class D > template< class D2 > 
  Image< D >::Image( const Image< D2 > &img ) try 
    : _data( img._data.dim_size ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ), 
        z_table( nullptr ) {
    COMMENT( "Assigning quick access pointers.", 2 );
    qk_data = &_data[ 0 ];
    CreateTables( );
    COMMENT( "Copying image data.", 2 );
    size_t size = _data._size;
    for( size_t pxl = 0; pxl < size; ++pxl )
      QK_DATA( pxl ) = static_cast< D >( img.QK_DATA( pxl ) );
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

  template< > template< > 
  Image< Color >::Image( const Image< RealColor > &img ) try 
    : _data( img._data.dim_size ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ), 
        z_table( nullptr ) {
    COMMENT( "Assigning quick access pointers.", 2 );
    qk_data = &_data[ 0 ];
    CreateTables( );
    size_t size = _data._size;
    COMMENT( "Copying image data.", 2 );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      for( size_t chl = 0; chl < 4; ++chl ) {
        QK_DATA( pxl )( chl ) = static_cast< uchar >( img.QK_DATA( pxl )( chl ) );
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

  template< > template< > 
  Image< RealColor >::Image( const Image< Color > &img ) try 
    : _data( img._data.dim_size ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ), 
        z_table( nullptr ) {
    COMMENT( "Assigning quick access pointers.", 2 );
    qk_data = &_data[ 0 ];
    CreateTables( );
    size_t size = _data._size;
    COMMENT( "Copying image data.", 2 );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      for( size_t chl = 0; chl < 4; ++chl ) {
        QK_DATA( pxl )( chl ) = static_cast< float >( img.QK_DATA( pxl )( chl ) );
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

  template< > template< class D >
  Image< bial_complex >::Image( const Image< D > &img ) try :
      _data( img._data.dim_size ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ),
        z_table( nullptr ) {
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
      size_t size = _data._size;
      COMMENT( "Copying image data.", 2 );
      for( size_t pxl = 0; pxl < size; ++pxl )
        QK_DATA( pxl ) = bial_complex( img.QK_DATA( pxl ), static_cast< D >( 0 ) );
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
  
  template< > template< >
  Image< int >::Image( const Image< bial_complex > &img ) try :
    _data( img._data.dim_size ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ),
        z_table( nullptr ) {
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
      size_t size = _data._size;
      COMMENT( "Copying image data.", 2 );
      for( size_t pxl = 0; pxl < size; ++pxl )
        QK_DATA( pxl ) = static_cast< int >( img.QK_DATA( pxl ).real( ) );
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
  
  template< > template< >
  Image< llint >::Image( const Image< bial_complex > &img ) try :
    _data( img._data.dim_size ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ),
        z_table( nullptr ) {
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
      size_t size = _data._size;
      COMMENT( "Copying image data.", 2 );
      for( size_t pxl = 0; pxl < size; ++pxl )
        QK_DATA( pxl ) = static_cast< llint >( img.QK_DATA( pxl ).real( ) );
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

  template< > template< >
  Image< float >::Image( const Image< bial_complex > &img ) try :
    _data( img._data.dim_size ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ),
        z_table( nullptr ) {
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
      size_t size = _data._size;
      COMMENT( "Copying image data.", 2 );
      for( size_t pxl = 0; pxl < size; ++pxl )
        QK_DATA( pxl ) = static_cast< float >( img.QK_DATA( pxl ).real( ) );
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

  template< > template< >
  Image< double >::Image( const Image< bial_complex > &img ) try :
    _data( img._data.dim_size ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ),
        z_table( nullptr ) {
      COMMENT( "Assigning quick access pointers.", 2 );
      qk_data = &_data[ 0 ];
      CreateTables( );
      size_t size = _data._size;
      COMMENT( "Copying image data.", 2 );
      for( size_t pxl = 0; pxl < size; ++pxl )
        QK_DATA( pxl ) = static_cast< double >( img.QK_DATA( pxl ).real( ) );
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

  template< class D >
  Image< D >::Image( const Image< D > &img ) try :
    _data( img._data ), qk_data( nullptr ), pixel_size( img.pixel_size ), y_table( nullptr ), z_table( nullptr ) {
      COMMENT( "Assigning quick access pointers.", 4 );
      qk_data = &_data[ 0 ];
      CreateTables( );
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

  template< class D >
  Image< D >::Image( Image< D > &&img )  try :
    _data( std::move( img._data ) ), qk_data( img.qk_data ), pixel_size( std::move( img.pixel_size ) ),
      y_table( std::move( img.y_table ) ), z_table( std::move( img.z_table ) ) {
      img.y_table = nullptr;
      img.z_table = nullptr;
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

  template< class D >
  Image< D > &Image< D >::operator=( const Image< D > &other ) {
    COMMENT( "Assigning image", 2 );
    _data = other._data;
    qk_data = &_data[ 0 ];
    CreateTables( );
    pixel_size = other.pixel_size;
    return( *this );
  }

  template< class D >
  Image< D > &Image< D >::operator=( Image< D > &&other ) {
    COMMENT( "Assigning image", 2 );
    _data = std::move( other._data );
    qk_data = other.qk_data;
    pixel_size = std::move( other.pixel_size );
    y_table = std::move( other.y_table );
    z_table = std::move( other.z_table );
    other.y_table = nullptr;
    other.z_table = nullptr;
    return( *this );
  }

  template< class D >
  Image< D > &Image< D >::operator=( D scalar ) {
    size_t size = _data._size;
    for( size_t pxl = 0; pxl < size; ++pxl )
      _data[ pxl ] = scalar;
    return( *this );
  }

  template< class D > template< class D2 >
  Image< D > &Image< D >::operator=( const Image< D2 > &other ) {
    COMMENT( "Assigning image", 2 );
    _data = other._data;
    qk_data = &_data[ 0 ];
    CreateTables( );
    pixel_size = other.pixel_size;
    return( *this );
  }

  template< > template< >
  Image< Color > &Image< Color >::operator=( const Image< RealColor > &other ) {
    COMMENT( "Assigning image", 2 );
    size_t size = _data._size;
    _data = Matrix< Color >( other._data.Dim( ) );
    pixel_size = other.pixel_size;
    qk_data = &_data[ 0 ];
    CreateTables( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      for( size_t chl = 0; chl < 4; ++chl ) {
        QK_DATA( pxl )( chl ) = static_cast< uchar >( other.QK_DATA( pxl )( chl ) );
      }
    }
    return( *this );
  }

  template< > template< >
  Image< RealColor > &Image< RealColor >::operator=( const Image< Color > &other ) {
    COMMENT( "Assigning image", 2 );
    size_t size = _data._size;
    _data = Matrix< RealColor >( other._data.Dim( ) );
    qk_data = &_data[ 0 ];
    CreateTables( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      for( size_t chl = 0; chl < 4; ++chl ) {
        QK_DATA( pxl )( chl ) = static_cast< uchar >( other.QK_DATA( pxl )( chl ) );
      }
    }
    pixel_size = other.pixel_size;
    return( *this );
  }

  template< class D >
  Vector< float > Image< D >::PixelSize( ) const {
    return( pixel_size );
  }

  template< class D >
  void Image< D >::PixelSize( const Vector< float > &val ) {
    COMMENT( "Verifies if new pixel size vector matchs image number of spatial dimension.", 2 );
    size_t dims = val.size( );
    if( ( dims < 2 ) || ( dims > 3 ) ) {
      std::string msg( BIAL_ERROR( "Pixel size must have 2 or 3 dimension. Given: " + std::to_string( dims ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    pixel_size = val;
    if( dims == 2 )
      pixel_size.push_back( 1.0f );
  }

  template< class D >
  float Image< D >::PixelSize( size_t dms ) const {
    COMMENT( "Verifies if requested dimension exists.", 2 );
    if( dms > 2 ) {
      std::string msg( BIAL_ERROR( "Invalid dimension. Requested: " + std::to_string( dms ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    return( pixel_size[ dms ] );
  }

  template< class D >
  void Image< D >::PixelSize( size_t dms, float val ) {
    COMMENT( "Verifies if requested dimension exists.", 2 );
    if( dms > 2 ) {
      std::string msg( BIAL_ERROR( "Invalid dimension. Requested: " + std::to_string( dms ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    pixel_size[ dms ] = val;
  }

  template< class D >
  bool Image< D >::ValidCoordinate( const Vector< size_t > &pxl ) const {
    size_t dims = pxl.size( );
    if( dims > 3 ) {
      std::string msg( BIAL_ERROR( "Invalid vector size. Requested: " + std::to_string( dims ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    for( size_t dms = 0; dms < dims; ++dms ) {
      if( _data.dim_size( dms ) <= pxl[ dms ] )
        return( false );
    }
    return( true );
  }

  template< class D >
  bool Image< D >::ValidCoordinate( size_t x, size_t y ) const {
    if( ( _data.dim_size( 0 ) <= x ) || ( _data.dim_size( 1 ) <= y ) ) {
      return( false );
    }
    return( true );
  }

  template< class D >
  bool Image< D >::ValidCoordinate( size_t x, size_t y, size_t z ) const {
    if( ( _data.dim_size( 0 ) <= x ) || ( _data.dim_size( 1 ) <= y ) || ( _data.dim_size( 2 ) <= z ) ) {
      return( false );
    }
    return( true );
  }

  template< class D > template< class D2 >
  void Image< D >::SetRange( D2 min, D2 max ) {
    try {
      COMMENT( "Verifying if minimum is lower or equal to maximum.", 0 );
      if( min > max ) {
        std::string msg( BIAL_ERROR( "max >= min. Given min: " + std::to_string( min ) + ", max: " + 
                                     std::to_string( max ) ) );
        throw( std::runtime_error( msg ) );
      }
      COMMENT( "Getting data minimum and maximum intensities.", 2 );
      D img_min = Minimum( );
      D img_max = Maximum( );
      if( ( img_max == max ) && ( img_min == min ) ) {
        COMMENT( "If data is already in given range, do not change!", 2 );
        return;
      }
      if( img_max - img_min == static_cast< D >( 0.0 ) ) {
        COMMENT( "If data is flat, set it to min.", 2 );
        Set( min );
        return;
      }
      if( static_cast< D >( 1.1 ) == static_cast< D >( 1.0 ) ) {
        COMMENT( "Setting new range for integer datas.", 2 );
        for( size_t elm = 0; elm < _data._size; ++elm ) {
          QK_DATA( elm ) = static_cast< D >( std::round( min + ( static_cast< double >( QK_DATA( elm ) - img_min ) )
                                                         * ( max - min ) / ( img_max - img_min ) ) );
        }
      }
      else {
        COMMENT( "Setting new range for float point datas.", 2 );
        for( size_t elm = 0; elm < _data._size; ++elm ) {
          QK_DATA( elm ) = static_cast< D >( min + ( static_cast< double >( QK_DATA( elm ) - img_min ) )
                                             * ( max - min ) / ( img_max - img_min ) );
        }
      }
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

  template< > template< class D2 >
  void Image< Color >::SetRange( D2 min, D2 max ) {
    try {
      COMMENT( "Verifying if minimum is lower or equal to maximum.", 0 );
      if( min > max ) {
        std::string msg( BIAL_ERROR( "max >= min. Given min: " + std::to_string( min ) + ", max: " + 
                                     std::to_string( max ) ) );
        throw( std::runtime_error( msg ) );
      }
      COMMENT( "Getting data minimum and maximum intensities.", 2 );
      D2 img_min = static_cast< D2 >( QK_DATA( 0 )( 0 ) );
      D2 img_max = static_cast< D2 >( QK_DATA( 0 )( 0 ) );
      for( size_t pxl = 1; pxl < _data._size; ++pxl ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          if( img_min > QK_DATA( pxl )( chl ) )
            img_min = QK_DATA( pxl )( chl );
          if( img_max < QK_DATA( pxl )( chl ) )
            img_max = QK_DATA( pxl )( chl );
        }
      }
      if( ( img_max == max ) && ( img_min == min ) ) {
        COMMENT( "If data is already in given range, do not change!", 2 );
        return;
      }
      if( img_max == img_min ) {
        COMMENT( "If data is flat, set it to min.", 2 );
        Set( Color( min, min, min, min ) );
        return;
      }
      COMMENT( "Setting new range for integer datas.", 2 );
      for( size_t elm = 0; elm < _data._size; ++elm ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          QK_DATA( elm )( chl ) = 
            static_cast< uchar >( std::round( min + ( static_cast< double >( QK_DATA( elm )( chl ) - img_min ) ) *
                                              ( max - min ) / ( img_max - img_min ) ) );
        }
      }
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

  template< > template< >
  void Image< Color >::SetRange( Color min, Color max ) {
    try {
      COMMENT( "Verifying if minimum is lower or equal to maximum.", 0 );
      for( size_t chl = 0; chl < 4; ++chl ) {
        if( min( chl ) > max( chl ) ) {
          std::string msg( BIAL_ERROR( "max (" + std::to_string( chl ) + ") >= min(" + std::to_string( chl ) + 
                                       "). Given min: " + std::to_string( min( chl ) ) + ", max: " + 
                                       std::to_string( max( chl ) ) ) );
          throw( std::runtime_error( msg ) );
        }
      }
      COMMENT( "Getting data minimum and maximum intensities.", 2 );
      Color img_min = QK_DATA( 0 );
      Color img_max = QK_DATA( 0 );
      for( size_t pxl = 1; pxl < _data._size; ++pxl ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          if( img_min( chl ) > QK_DATA( pxl )( chl ) )
            img_min( chl ) = QK_DATA( pxl )( chl );
          if( img_max( chl ) < QK_DATA( pxl )( chl ) )
            img_max( chl ) = QK_DATA( pxl )( chl );
        }
      }
      if( ( img_max == max ) && ( img_min == min ) ) {
        COMMENT( "If data is already in given range, do not change!", 2 );
        return;
      }
      if( img_max == img_min ) {
        COMMENT( "If data is flat, set it to min.", 2 );
        Set( min );
        return;
      }
      COMMENT( "Setting new range for integer datas.", 2 );
      for( size_t elm = 0; elm < _data._size; ++elm ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          QK_DATA( elm )( chl ) = 
            static_cast< uchar >( std::round( min( chl ) + 
                                              ( static_cast< double >( QK_DATA( elm )( chl ) - img_min( chl ) ) ) *
                                              ( max( chl ) - min( chl ) ) / ( img_max( chl ) - img_min( chl ) ) ) );
        }
      }
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

  template< > template< class D2 >
  void Image< RealColor >::SetRange( D2 min, D2 max ) {
    try {
      COMMENT( "Verifying if minimum is lower or equal to maximum.", 0 );
      if( min > max ) {
        std::string msg( BIAL_ERROR( "max >= min. Given min: " + std::to_string( min ) + ", max: " + 
                                     std::to_string( max ) ) );
        throw( std::runtime_error( msg ) );
      }
      COMMENT( "Getting data minimum and maximum intensities.", 2 );
      D2 img_min = static_cast< D2 >( QK_DATA( 0 )( 0 ) );
      D2 img_max = static_cast< D2 >( QK_DATA( 0 )( 0 ) );
      for( size_t pxl = 1; pxl < _data._size; ++pxl ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          if( img_min > QK_DATA( pxl )( chl ) )
            img_min = QK_DATA( pxl )( chl );
          if( img_max < QK_DATA( pxl )( chl ) )
            img_max = QK_DATA( pxl )( chl );
        }
      }
      if( ( img_max == max ) && ( img_min == min ) ) {
        COMMENT( "If data is already in given range, do not change!", 2 );
        return;
      }
      if( img_max == img_min ) {
        COMMENT( "If data is flat, set it to min.", 2 );
        Set( RealColor( min ) );
        return;
      }
      COMMENT( "Setting new range for integer datas.", 2 );
      for( size_t elm = 0; elm < _data._size; ++elm ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          QK_DATA( elm )( chl ) = 
            static_cast< uchar >( std::round( min + ( static_cast< double >( QK_DATA( elm )( chl ) - img_min ) ) *
                                              ( max - min ) / ( img_max - img_min ) ) );
        }
      }
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

  template< > template< >
  void Image< RealColor >::SetRange( RealColor min, RealColor max ) {
    try {
      COMMENT( "Verifying if minimum is lower or equal to maximum.", 0 );
      for( size_t chl = 0; chl < 4; ++chl ) {
        if( min( chl ) > max( chl ) ) {
          std::string msg( BIAL_ERROR( "max (" + std::to_string( chl ) + ") >= min(" + std::to_string( chl ) + 
                                       "). Given min: " + std::to_string( min( chl ) ) + ", max: " + 
                                       std::to_string( max( chl ) ) ) );
          throw( std::runtime_error( msg ) );
        }
      }
      COMMENT( "Getting data minimum and maximum intensities.", 2 );
      RealColor img_min = QK_DATA( 0 );
      RealColor img_max = QK_DATA( 0 );
      for( size_t pxl = 1; pxl < _data._size; ++pxl ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          if( img_min( chl ) > QK_DATA( pxl )( chl ) )
            img_min( chl ) = QK_DATA( pxl )( chl );
          if( img_max( chl ) < QK_DATA( pxl )( chl ) )
            img_max( chl ) = QK_DATA( pxl )( chl );
        }
      }
      if( ( img_max == max ) && ( img_min == min ) ) {
        COMMENT( "If data is already in given range, do not change!", 2 );
        return;
      }
      if( img_max == img_min ) {
        COMMENT( "If data is flat, set it to min.", 2 );
        Set( min );
        return;
      }
      COMMENT( "Setting new range for integer datas.", 2 );
      for( size_t elm = 0; elm < _data._size; ++elm ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          QK_DATA( elm )( chl ) = 
            static_cast< uchar >( std::round( min( chl ) + 
                                              ( static_cast< double >( QK_DATA( elm )( chl ) - img_min( chl ) ) ) *
                                              ( max( chl ) - min( chl ) ) / ( img_max( chl ) - img_min( chl ) ) ) );
        }
      }
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
  D Image< D >::Minimum( ) const {
    try {
      D min = QK_DATA( 0 );
      for( size_t elm = 1; elm < _data._size; ++elm ) {
        if( min > QK_DATA( elm ) )
          min = QK_DATA( elm );
      }
      return( min );
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

  template< >
  Color Image< Color >::Minimum( ) const {
    try {
      Color min = QK_DATA( 0 );
      for( size_t elm = 1; elm < _data._size; ++elm ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          if( min( chl ) > QK_DATA( elm )( chl ) ) {
            min( chl ) = QK_DATA( elm )( chl );
          }
        }
      }
      return( min );
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

  template< >
  RealColor Image< RealColor >::Minimum( ) const {
    try {
      RealColor min = QK_DATA( 0 );
      for( size_t elm = 1; elm < _data._size; ++elm ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          if( min( chl ) > QK_DATA( elm )( chl ) ) {
            min( chl ) = QK_DATA( elm )( chl );
          }
        }
      }
      return( min );
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

  template< >
  bial_complex Image< bial_complex >::Minimum( ) const {
    try {
      bial_complex min = QK_DATA( 0 );
      for( size_t elm = 1; elm < _data._size; ++elm ) {
        if( std::abs( min ) > std::abs( QK_DATA( elm ) ) )
          min = QK_DATA( elm );
      }
      return( min );
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
  D Image< D >::Maximum( ) const {
    try {
      D max = QK_DATA( 0 );
      for( size_t elm = 1; elm < _data._size; ++elm ) {
        if( max < QK_DATA( elm ) )
          max = QK_DATA( elm );
      }
      return( max );
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

  template< >
  Color Image< Color >::Maximum( ) const {
    try {
      Color max = QK_DATA( 0 );
      for( size_t elm = 1; elm < _data._size; ++elm ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          if( max( chl ) < QK_DATA( elm )( chl ) ) {
            max( chl ) = QK_DATA( elm )( chl );
          }
        }
      }
      return( max );
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

  template< >
  RealColor Image< RealColor >::Maximum( ) const {
    try {
      RealColor max = QK_DATA( 0 );
      for( size_t elm = 1; elm < _data._size; ++elm ) {
        for( size_t chl = 0; chl < 4; ++chl ) {
          if( max( chl ) < QK_DATA( elm )( chl ) ) {
            max( chl ) = QK_DATA( elm )( chl );
          }
        }
      }
      return( max );
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

  template< >
  bial_complex Image< bial_complex >::Maximum( ) const {
    try {
      bial_complex min = QK_DATA( 0 );
      for( size_t elm = 1; elm < _data._size; ++elm ) {
        if( std::abs( min ) < std::abs( QK_DATA( elm ) ) )
          min = QK_DATA( elm );
      }
      return( min );
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


  /* Using macros here to speed up code in run mode and allow debug mode. */

  template< class D >
  const D &Image< D >::operator[]( size_t p ) const {
    return( QK_DATA( p ) );
  }

  template< class D >
  D &Image< D >::operator[]( size_t p ) {
    return( QK_DATA( p ) );
  }

  template< class D >
  const D &Image< D >::operator()( size_t p ) const {
    return( QK_DATA( p ) );
  }

  template< class D >
  D &Image< D >::operator()( size_t p ) {
    return( QK_DATA( p ) );
  }

  template< class D >
  const D &Image< D >::operator()( size_t p_0, size_t p_1 ) const {
#ifdef BIAL_DEBUG
    if( ( p_0 >= _data.size( 0 ) ) || ( p_1 >= _data.size( 1 ) ) ) {
      std::string msg( BIAL_ERROR( "Invalid access index: Given x:" + std::to_string( p_0 ) + 
                                   " y:" + std::to_string( p_1 ) + 
                                   ", expected x < " + std::to_string( _data.size( 0 ) ) +
                                   " y < " + std::to_string( _data.size( 1 ) ) + "." ) );
      throw( std::runtime_error( msg ) );
    }
#endif
    return( y_table[ p_1 ][ p_0 ] );
  }

  template< class D >
  D &Image< D >::operator()( size_t p_0, size_t p_1 ) {
#ifdef BIAL_DEBUG
    if( ( p_0 >= _data.size( 0 ) ) || ( p_1 >= _data.size( 1 ) ) ) {
      std::string msg( BIAL_ERROR( "Invalid access index: Given x:" + std::to_string( p_0 ) + 
                                   " y:" + std::to_string( p_1 ) + 
                                   ", expected x < " + std::to_string( _data.size( 0 ) ) +
                                   " y < " + std::to_string( _data.size( 1 ) ) + "." ) );
      throw( std::runtime_error( msg ) );
    }
#endif
    return( y_table[ p_1 ][ p_0 ] );
  }

  template< class D >
  const D &Image< D >::operator()( size_t p_0, size_t p_1, size_t p_2 ) const {
#ifdef BIAL_DEBUG
    if( ( p_0 >= _data.size( 0 ) ) || ( p_1 >= _data.size( 1 ) ) || ( p_2 >= _data.size( 2 ) ) ) {
      std::string msg( BIAL_ERROR( "Invalid access index: Given x:" + std::to_string( p_0 ) + 
                                   " y:" + std::to_string( p_1 ) + " z:" + std::to_string( p_2 ) + 
                                   ", expected x < " + std::to_string( _data.size( 0 ) ) +
                                   " y < " + std::to_string( _data.size( 1 ) ) +
                                   " z < " + std::to_string( _data.size( 2 ) ) + "." ) );
      throw( std::runtime_error( msg ) );
    }
#endif
    return( z_table[ p_2 ][ p_1 ][ p_0 ] );
  }

  template< class D >
  D &Image< D >::operator()( size_t p_0, size_t p_1, size_t p_2 ) {
#ifdef BIAL_DEBUG
    if( ( p_0 >= _data.size( 0 ) ) || ( p_1 >= _data.size( 1 ) ) || ( p_2 >= _data.size( 2 ) ) ) {
      std::string msg( BIAL_ERROR( "Invalid access index: Given x:" + std::to_string( p_0 ) + 
                                   " y:" + std::to_string( p_1 ) + " z:" + std::to_string( p_2 ) +
                                   ", expected x < " + std::to_string( _data.size( 0 ) ) +
                                   " y < " + std::to_string( _data.size( 1 ) ) +
                                   " z < " + std::to_string( _data.size( 2 ) ) + "." ) );
      throw( std::runtime_error( msg ) );
    }
#endif
    return( z_table[ p_2 ][ p_1 ][ p_0 ] );
  }

  template< class D >
  const D &Image< D >::operator()( const Vector< size_t > &p ) const {
    try {
      size_t dims = p.size( );
#ifdef BIAL_DEBUG
      if( dims > 3 ) {
        std::string msg( BIAL_ERROR( "Invalid vector size. Requested: " + std::to_string( dims ) + "." ) );
        throw( std::logic_error( msg ) );
      }
#endif
      if( dims == 2 ) {
#ifdef BIAL_DEBUG
        if( ( p[ 0 ] >= _data.size( 0 ) ) || ( p[ 1 ] >= _data.size( 1 ) ) ) {
          std::string msg( BIAL_ERROR( "Invalid access index: Given x:" + std::to_string( p[ 0 ] ) + 
                                       " y:" + std::to_string( p[ 1 ] ) + 
                                       ", expected x < " + std::to_string( _data.size( 0 ) ) +
                                       " y < " + std::to_string( _data.size( 1 ) ) + "." ) );
          throw( std::runtime_error( msg ) );
        }
#endif
        return( y_table[ p[ 1 ] ][ p[ 0 ] ] );
      }
#ifdef BIAL_DEBUG
      if( ( p[ 0 ] >= _data.size( 0 ) ) || ( p[ 1 ] >= _data.size( 1 ) ) || ( p[ 2 ] >= _data.size( 2 ) ) ) {
        std::string msg( BIAL_ERROR( "Invalid access index: Given x:" + std::to_string( p[ 0 ] ) + 
                                     " y:" + std::to_string( p[ 1 ] ) + " z:" + std::to_string( p[ 2 ] ) +
                                     ", expected x < " + std::to_string( _data.size( 0 ) ) +
                                     " y < " + std::to_string( _data.size( 1 ) ) +
                                     " z < " + std::to_string( _data.size( 2 ) ) + "." ) );
        throw( std::runtime_error( msg ) );
      }
#endif
      return( z_table[ p[ 2 ] ][ p[ 1 ] ][ p[ 0 ] ] );
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
  D &Image< D >::operator()( const Vector< size_t > &p ) {
    try {
      size_t dims = p.size( );
#ifdef BIAL_DEBUG
      if( dims > 3 ) {
        std::string msg( BIAL_ERROR( "Invalid vector size. Requested: " + std::to_string( dims ) + "." ) );
        throw( std::logic_error( msg ) );
      }
#endif
      if( dims == 2 ) {
#ifdef BIAL_DEBUG
        if( ( p[ 0 ] >= _data.size( 0 ) ) || ( p[ 1 ] >= _data.size( 1 ) ) ) {
          std::string msg( BIAL_ERROR( "Invalid access index: Given x:" + std::to_string( p[ 0 ] ) + 
                                       " y:" + std::to_string( p[ 1 ] ) + 
                                       ", expected x < " + std::to_string( _data.size( 0 ) ) +
                                       " y < " + std::to_string( _data.size( 1 ) ) + "." ) );
          throw( std::runtime_error( msg ) );
        }
#endif
        return( y_table[ p[ 1 ] ][ p[ 0 ] ] );
      }
#ifdef BIAL_DEBUG
      if( ( p[ 0 ] >= _data.size( 0 ) ) || ( p[ 1 ] >= _data.size( 1 ) ) || ( p[ 2 ] >= _data.size( 2 ) ) ) {
        std::string msg( BIAL_ERROR( "Invalid access index: Given x:" + std::to_string( p[ 0 ] ) + 
                                     " y:" + std::to_string( p[ 1 ] ) + " z:" + std::to_string( p[ 2 ] ) +
                                     ", expected x < " + std::to_string( _data.size( 0 ) ) +
                                     " y < " + std::to_string( _data.size( 1 ) ) +
                                     " z < " + std::to_string( _data.size( 2 ) ) + "." ) );
        throw( std::runtime_error( msg ) );
      }
#endif
      return( z_table[ p[ 2 ] ][ p[ 1 ] ][ p[ 0 ] ] );
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
  typename std::vector< D >::iterator Image< D >::begin( ) noexcept {
    COMMENT( "size_of D: " << sizeof( D ), 4 );
    return( _data.begin( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Image< D >::begin( ) const noexcept {
    COMMENT( "size_of D: " << sizeof( D ), 4 );
    return( _data.begin( ) );
  }

  template< class D >
  typename std::vector< D >::iterator Image< D >::end( ) noexcept {
    COMMENT( "size_of D: " << sizeof( D ), 4 );
    return( _data.end( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Image< D >::end( ) const noexcept {
    COMMENT( "size_of D: " << sizeof( D ), 4 );
    return( _data.end( ) );
  }

  template< class D >
  typename std::vector< D >::reverse_iterator Image< D >::rbegin( ) noexcept {
    return( _data.rbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Image< D >::rbegin( ) const noexcept {
    return( _data.rbegin( ) );
  }

  template< class D >
  typename std::vector< D >::reverse_iterator Image< D >::rend( ) noexcept {
    return( _data.rend( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Image< D >::rend( ) const noexcept {
    return( _data.rend( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Image< D >::cbegin( ) const noexcept {
    return( _data.cbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_iterator Image< D >::cend( ) const noexcept {
    return( _data.cend( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Image< D >::crbegin( ) const noexcept {
    return( _data.crbegin( ) );
  }

  template< class D >
  typename std::vector< D >::const_reverse_iterator Image< D >::crend( ) const noexcept {
    return( _data.crend( ) );
  }

  template< class D >
  const D*Image< D >::Data( ) const {
    return( qk_data );
  }

  template< class D >
  D*Image< D >::Data( ) {
    return( qk_data );
  }

  template< class D >
  const D*Image< D >::data( ) const {
    return( qk_data );
  }

  template< class D >
  D*Image< D >::data( ) {
    return( qk_data );
  }

  template< class D >
  const Matrix< D > &Image< D >::DataMatrix( ) const {
    return( _data );
  }

  template< class D >
  Matrix< D > &Image< D >::DataMatrix( ) {
    return( _data );
  }

  template< class D >
  size_t Image< D >::size( ) const {
    return( _data._size );
  }

  template< class D >
  size_t Image< D >::Size( ) const {
    return( _data._size );
  }

  template< class D >
  size_t Image< D >::Coordinate( size_t position, size_t dimension ) const {
    if( dimension == 0 ) {
      return( position % _data.dim_size( 0 ) );
    }
    if( dimension == 1 ) {
      return( ( position / _data.dim_size( 0 ) ) % _data.dim_size( 1 ) );
    }
    return( position / _data.acc_dim_size( 1 ) );
  }

  template< class D >
  Vector< size_t > Image< D >::Coordinates( size_t position ) const {
    // Vector< size_t > index( _data.dim_size.size( ), position / _data.acc_dim_size( 1 ) );
    // index[ 0 ] = position % _data.dim_size( 0 );
    // index[ 1 ] = ( position / _data.dim_size( 0 ) ) % _data.dim_size( 1 );
    std::div_t pos_div_xy_size = std::div( static_cast< int >( position ), _data.acc_dim_size( 1 ) );
    std::div_t xy_size_div_xsize = std::div( pos_div_xy_size.rem, _data.dim_size( 0 ) );
    Vector< size_t > index( 3, pos_div_xy_size.quot );
    index[ 0 ] = xy_size_div_xsize.rem;
    index[ 1 ] = xy_size_div_xsize.quot;
    return( index );
  }

  template< class D >
  Vector< size_t > Image< D >::Coordinates2( size_t position ) const {
    std::div_t pos_div_xsize = std::div( static_cast< int >( position ), _data.acc_dim_size( 0 ) );
    Vector< size_t > index( 3, 0 );
    index[ 0 ] = pos_div_xsize.rem;
    index[ 1 ] = pos_div_xsize.quot;
    return( index );
  }

  template< class D >
  size_t Image< D >::Position( size_t p_0, size_t p_1 ) const {
    return( _data.Position( p_0, p_1 ) );
  }

  template< class D >
  size_t Image< D >::Position( size_t p_0, size_t p_1, size_t p_2 ) const {
    return( _data.Position( p_0, p_1, p_2 ) );
  }

  template< class D >
  size_t Image< D >::Position( const Vector< size_t > &coordinates ) const {
    return( _data.Position( coordinates ) );
  }

  template< class D >
  size_t Image< D >::Position( const Vector< int > &coordinates ) const {
    return( _data.Position( coordinates ) );
  }

  template< class D >
  size_t Image< D >::Position( const Vector< float > &coordinates ) const {
    return( _data.Position( coordinates ) );
  }

  template< class D >
  void Image< D >::Set( D val ) {
    try {
      _data.Set( val );
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
  Vector< size_t > Image< D >::Dim( ) const {
    return( _data.dim_size );
  }

  template< class D >
  size_t Image< D >::MinDim( ) const {
    size_t min_dim = std::min( _data.dim_size( 0 ), _data.dim_size( 1 ) );
    if( _data.dim_size( 2 ) != 1 )
      min_dim = std::min( min_dim, _data.dim_size( 2 ) );
    return( min_dim );
  }

  template< class D >
  size_t Image< D >::MaxDim( ) const {
    size_t max_dim = std::max( std::max( _data.dim_size( 0 ), _data.dim_size( 1 ) ), _data.dim_size( 2 ) );
    return( max_dim );
  }

  template< class D >
  size_t Image< D >::Size( size_t dms ) const {
    return( _data.dim_size( dms ) );
  }

  template< class D >
  size_t Image< D >::size( size_t dms ) const {
    return( _data.dim_size( dms ) );
  }

  template< class D >
  size_t Image< D >::Dims( ) const {
    return( _data.dim_size( 2 ) == 1 ? 2 : 3 );
  }

  template< class D >
  size_t Image< D >::Displacement( size_t dms ) const {
    return( _data.acc_dim_size( dms ) );
  }

  template< class D >
  const D &Image< D >::at( size_t p ) const {
    try {
      return( _data.at( p ) );
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
  D &Image< D >::at( size_t p ) {
    try {
      return( _data.at( p ) );
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
  const D &Image< D >::at( size_t p_0, size_t p_1 ) const {
    try {
      return( _data.at( p_0, p_1 ) );
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
  D &Image< D >::at( size_t p_0, size_t p_1 ) {
    try {
      return( _data.at( p_0, p_1 ) );
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
  const D &Image< D >::at( size_t p_0, size_t p_1, size_t p_2 ) const {
    try {
      return( _data.at( p_0, p_1, p_2 ) );
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
  D &Image< D >::at( size_t p_0, size_t p_1, size_t p_2 ) {
    try {
      return( _data.at( p_0, p_1, p_2 ) );
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
  const D &Image< D >::at( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) const {
    try {
      return( _data.at( p_0, p_1, p_2, p_3 ) );
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
  D &Image< D >::at( size_t p_0, size_t p_1, size_t p_2, size_t p_3 ) {
    try {
      return( _data.at( p_0, p_1, p_2, p_3 ) );
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
  const D &Image< D >::at( const Vector< size_t > &p ) const {
    size_t dims = p.size( );
    if( ( dims < 2 ) || ( dims > 3 ) ) {
      std::string msg( BIAL_ERROR( " Invalid number of dimensions. Given: " + std::to_string( p.size( ) ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    for( size_t dms = 0; dms < dims; ++dms ) {
      if( p[ dms ] >= size( dms ) ) {
        std::string msg( BIAL_ERROR( " Index out of range. Maximum: " + 
                                     std::to_string( size( dms ) - 1 ) + ", given: " + std::to_string( p[ dms ] ) ) );
        throw( std::out_of_range( msg ) );
      }
    }
    return( ( *this )( p ) );
  }

  template< class D >
  D &Image< D >::at( const Vector< size_t > &p ) {
    size_t dims = p.size( );
    if( ( dims < 2 ) || ( dims > 3 ) ) {
      std::string msg( BIAL_ERROR( " Invalid number of dimensions. Given: " + std::to_string( p.size( ) ) + "." ) );
      throw( std::logic_error( msg ) );
    }
    for( size_t dms = 0; dms < dims; ++dms ) {
      if( p[ dms ] >= size( dms ) ) {
        std::string msg( BIAL_ERROR( " Index out of range. Maximum: " + 
                                     std::to_string( size( dms ) - 1 ) + ", given: " + std::to_string( p[ dms ] ) ) );
        throw( std::out_of_range( msg ) );
      }
    }
    return( ( *this )( p ) );
  }

  template< class D >
  Image< D > operator+( const Image< D > &img1, const Image< D > &img2 ) {
    try {
      COMMENT( "Verifying if images have compatible dimensions", 2 );
      if( img1.size( ) != img2.size( ) ) {
        std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img1 );
      COMMENT( "Performing addition.", 2 );
      return( res += img2 );
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

  template< class D >
  Image< D > &operator+=( Image< D > &img1, const Image< D > &img2 ) {
    COMMENT( "Verifying if images have compatible dimensions.", 4 );
    if( img1.size( ) != img2.size( ) ) {
      std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Performing addition.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl )
      img1( pxl ) += img2( pxl );
    return( img1 );
  }

  template< class D >
  Image< D > operator-( const Image< D > &img1, const Image< D > &img2 ) {
    try {
      COMMENT( "Verifying if images have compatible dimensions.", 4 );
      if( img1.size( ) != img2.size( ) ) {
        std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing resultant image.", 4 );
      Image< D > res( img1 );
      COMMENT( "Performing subtraction.", 4 );
      return( res -= img2 );
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

  template< class D >
  Image< D > &operator-=( Image< D > &img1, const Image< D > &img2 ) {
   COMMENT( "Verifying if images have compatible dimensions.", 4 );
    if( img1.size( ) != img2.size( ) ) {
      std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Performing subtraction.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      img1( pxl ) -= img2( pxl );
    }
    return( img1 );
  }

  template< class D >
  Image< D > operator*( const Image< D > &img1, const Image< D > &img2 ) {
    try {
      COMMENT( "Verifying if images have compatible dimensions.", 4 );
      if( img1.size( ) != img2.size( ) ) {
        std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing resultant image.", 4 );
      Image< D > res( img1 );
      COMMENT( "Performing multiplication.", 4 );
      return( res *= img2 );
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

  template< class D >
  Image< D > &operator*=( Image< D > &img1, const Image< D > &img2 ) {
   COMMENT( "Verifying if images have compatible dimensions.", 4 );
    if( img1.size( ) != img2.size( ) ) {
      std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
      throw( std::logic_error( msg ) );
    }
    COMMENT( "Performing multiplication.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      img1( pxl ) *= img2( pxl );
    }
    return( img1 );
  }

  template< class D >
  Image< D > operator/( const Image< D > &img1, const Image< D > &img2 ) {
    try {
      COMMENT( "Verifying if images have compatible dimensions.", 4 );
      if( img1.size( ) != img2.size( ) ) {
        std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing resultant image.", 4 );
      Image< D > res( img1 );
      COMMENT( "Performing division.", 4 );
      return( res /= img2 );
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

  template< class D >
  Image< D > &operator/=( Image< D > &img1, const Image< D > &img2 ) {
    COMMENT( "Verifying if images have compatible dimensions.", 4 );
    if( img1.size( ) != img2.size( ) ) {
      std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
      throw( std::logic_error( msg ) );
    }
#ifdef BIAL_DEBUG
    COMMENT( "Performing division.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      if( img2( pxl ) == 0 ) {
        std::string msg( BIAL_ERROR( "Division by zero at pixel " + std::to_string( pxl ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      img1( pxl ) /= img2( pxl );
    }
#else
    COMMENT( "Performing division.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      img1( pxl ) /= img2( pxl );
    }
#endif
    return( img1 );
  }

  template< >
  Image< Color > &operator/=( Image< Color > &img1, const Image< Color > &img2 ) {
    COMMENT( "Verifying if images have compatible dimensions.", 4 );
    if( img1.size( ) != img2.size( ) ) {
      std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
      throw( std::logic_error( msg ) );
    }
#ifdef BIAL_DEBUG
    COMMENT( "Performing division.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      if( ( img2( pxl )( 0 ) == 0 ) || ( img2( pxl )( 1 ) == 0 ) || 
          ( img2( pxl )( 2 ) == 0 ) || ( img2( pxl )( 3 ) == 0 ) ) {
        std::string msg( BIAL_ERROR( "Division by zero at pixel " + std::to_string( pxl ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      img1( pxl ) /= img2( pxl );
    }
#else
    COMMENT( "Performing division.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      img1( pxl ) /= img2( pxl );
    }
#endif
    return( img1 );
  }

  template< >
  Image< RealColor > &operator/=( Image< RealColor > &img1, const Image< RealColor > &img2 ) {
    COMMENT( "Verifying if images have compatible dimensions.", 4 );
    if( img1.size( ) != img2.size( ) ) {
      std::string msg( BIAL_ERROR( " All images must have the same dimensions." ) );
      throw( std::logic_error( msg ) );
    }
#ifdef BIAL_DEBUG
    COMMENT( "Performing division.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      if( ( img2( pxl )( 0 ) == 0.0f ) || ( img2( pxl )( 1 ) == 0.0f ) || 
          ( img2( pxl )( 2 ) == 0.0f ) || ( img2( pxl )( 3 ) == 0.0f ) ) {
        std::string msg( BIAL_ERROR( "Division by zero at pixel " + std::to_string( pxl ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      img1( pxl ) /= img2( pxl );
    }
#else
    COMMENT( "Performing division.", 4 );
    size_t size = img1.size( );
    for( size_t pxl = 0; pxl < size; ++pxl ) {
      img1( pxl ) /= img2( pxl );
    }
#endif
    return( img1 );
  }

  template< class D >
  Image< D > operator+( const Image< D > &img, double scalar ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img );
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

  template< class D >
  Image< D > operator+( double scalar, const Image< D > &img ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img );
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

  template< class D >
  Image< D > &operator+=( Image< D > &img, double scalar ) {
    try {
      COMMENT( "Performing addition.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        img( pxl ) += scalar;
      }
      return( img );
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

  template< class D >
  Image< D > operator-( const Image< D > &img, double scalar ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img );
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

  template< class D >
  Image< D > operator-( double scalar, const Image< D > &img ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img );
      COMMENT( "Performing subtraction.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        res( pxl ) = scalar - res( pxl );
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

  template< class D >
  Image< D > &operator-=( Image< D > &img, double scalar ) {
    try {
      COMMENT( "Performing subtraction.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        img( pxl ) -= scalar;
      }
      return( img );
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

  template< class D >
  Image< D > operator*( const Image< D > &img, double scalar ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img );
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

  template< class D >
  Image< D > operator*( double scalar, const Image< D > &img ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img );
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

  template< class D >
  Image< D > &operator*=( Image< D > &img, double scalar ) {
    try {
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        img( pxl ) *= scalar;
      }
      return( img );
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

  template< class D >
  Image< D > operator/( const Image< D > &img, double scalar ) {
    try {
      COMMENT( "Verifying if scalar is not zero.", 2 );
      if( scalar == 0.0 ) {
        std::string msg( BIAL_ERROR( "Division by zero." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img );
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

  template< class D >
  Image< D > operator/( double scalar, const Image< D > &img ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< D > res( img );
#ifdef BIAL_DEBUG
      COMMENT( "Performing division.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( res( pxl ) == 0 ) {
          std::string msg( BIAL_ERROR( "Division by zero at pixel " + std::to_string( pxl ) + "." ) );
          throw( std::logic_error( msg ) );
        }
        res( pxl ) = scalar / res( pxl );
      }
#else
      COMMENT( "Performing division.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        res( pxl ) = scalar / res( pxl );
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

  template< >
  Image< Color > operator/( double scalar, const Image< Color > &img ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< Color > res( img );
#ifdef BIAL_DEBUG
      COMMENT( "Performing division.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( ( res( pxl )( 0 ) == 0 ) || ( res( pxl )( 1 ) == 0 ) || 
            ( res( pxl )( 2 ) == 0 ) || ( res( pxl )( 3 ) == 0 ) ) {
          std::string msg( BIAL_ERROR( "Division by zero at pixel " + std::to_string( pxl ) + "." ) );
          throw( std::logic_error( msg ) );
        }
        res( pxl ) = scalar / res( pxl );
      }
#else
      COMMENT( "Performing division.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        res( pxl ) = scalar / res( pxl );
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

  template< >
  Image< RealColor > operator/( double scalar, const Image< RealColor > &img ) {
    try {
      COMMENT( "Initializing resultant image.", 2 );
      Image< RealColor > res( img );
#ifdef BIAL_DEBUG
      COMMENT( "Performing division.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        if( ( res( pxl )( 0 ) == 0.0f ) || ( res( pxl )( 1 ) == 0.0f ) || 
            ( res( pxl )( 2 ) == 0.0f ) || ( res( pxl )( 3 ) == 0.0f ) ) {
          std::string msg( BIAL_ERROR( "Division by zero at pixel " + std::to_string( pxl ) + "." ) );
          throw( std::logic_error( msg ) );
        }
        res( pxl ) = scalar / res( pxl );
      }
#else
      COMMENT( "Performing division.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        res( pxl ) = scalar / res( pxl );
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

  template< class D >
  Image< D > &operator/=( Image< D > &img, double scalar ) {
    try {
      COMMENT( "Verifying if scalar is not zero.", 2 );
      if( scalar == 0.0 ) {
        std::string msg( BIAL_ERROR( "Division by zero." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Performing division.", 2 );
      size_t size = img.size( );
      for( size_t pxl = 0; pxl < size; ++pxl ) {
        img( pxl ) /= scalar;
      }
      return( img );
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

#ifdef BIAL_EXPLICIT_Image

  /* Explicit instantiation of class Image. Insert here if other types are necessary. */
  template class Image< int >;
  template class Image< llint >;
  template class Image< float >;
  template class Image< double >;
  template class Image< Color >;
  template class Image< RealColor >;
  template class Image< bial_complex >;

  /*
   * Explicit instantiation of Image functions. Insert here if other types are necessary.
   * Constructor
   */
  template Image< int >::Image( const Image< llint > &img );
  template Image< int >::Image( const Image< float > &img );
  template Image< int >::Image( const Image< double > &img );

  template Image< llint >::Image( const Image< int > &img );
  template Image< llint >::Image( const Image< float > &img );
  template Image< llint >::Image( const Image< double > &img );

  template Image< float >::Image( const Image< int > &img );
  template Image< float >::Image( const Image< llint > &img );
  template Image< float >::Image( const Image< double > &img );

  template Image< double >::Image( const Image< int > &img );
  template Image< double >::Image( const Image< llint > &img );
  template Image< double >::Image( const Image< float > &img );

  template Image< bial_complex >::Image( const Image< int > &img );
  template Image< bial_complex >::Image( const Image< llint > &img );
  template Image< bial_complex >::Image( const Image< float > &img );
  template Image< bial_complex >::Image( const Image< double > &img );

  /* operator= */
  template Image< int > &Image< int >::operator=( const Image< llint > &img );
  template Image< int > &Image< int >::operator=( const Image< float > &img );
  template Image< int > &Image< int >::operator=( const Image< double > &img );

  template Image< llint > &Image< llint >::operator=( const Image< int > &img );
  template Image< llint > &Image< llint >::operator=( const Image< float > &img );
  template Image< llint > &Image< llint >::operator=( const Image< double > &img );

  template Image< float > &Image< float >::operator=( const Image< int > &img );
  template Image< float > &Image< float >::operator=( const Image< llint > &img );
  template Image< float > &Image< float >::operator=( const Image< double > &img );

  template Image< double > &Image< double >::operator=( const Image< int > &img );
  template Image< double > &Image< double >::operator=( const Image< llint > &img );
  template Image< double > &Image< double >::operator=( const Image< float > &img );

  template Image< int > operator+( const Image< int > &img1, const Image< int > &img2 );
  template Image< int > &operator+=( Image< int > &img1, const Image< int > &img2 );
  template Image< int > operator-( const Image< int > &img1, const Image< int > &img2 );
  template Image< int > &operator-=( Image< int > &img1, const Image< int > &img2 );
  template Image< int > operator*( const Image< int > &img1, const Image< int > &img2 );
  template Image< int > &operator*=( Image< int > &img1, const Image< int > &img2 );
  template Image< int > operator/( const Image< int > &img1, const Image< int > &img2 );
  template Image< int > &operator/=( Image< int > &img1, const Image< int > &img2 );
  template Image< int > operator+( const Image< int > &img, double scalar );
  template Image< int > operator+( double scalar, const Image< int > &img );
  template Image< int > &operator+=( Image< int > &img, double scalar );
  template Image< int > operator-( const Image< int > &img, double scalar );
  template Image< int > operator-( double scalar, const Image< int > &img );
  template Image< int > &operator-=( Image< int > &img, double scalar );
  template Image< int > operator*( const Image< int > &img, double scalar );
  template Image< int > operator*( double scalar, const Image< int > &img );
  template Image< int > &operator*=( Image< int > &img, double scalar );
  template Image< int > operator/( const Image< int > &img, double scalar );
  template Image< int > operator/( double scalar, const Image< int > &img );
  template Image< int > &operator/=( Image< int > &img, double scalar );

  template Image< llint > operator+( const Image< llint > &img1, const Image< llint > &img2 );
  template Image< llint > &operator+=( Image< llint > &img1, const Image< llint > &img2 );
  template Image< llint > operator-( const Image< llint > &img1, const Image< llint > &img2 );
  template Image< llint > &operator-=( Image< llint > &img1, const Image< llint > &img2 );
  template Image< llint > operator*( const Image< llint > &img1, const Image< llint > &img2 );
  template Image< llint > &operator*=( Image< llint > &img1, const Image< llint > &img2 );
  template Image< llint > operator/( const Image< llint > &img1, const Image< llint > &img2 );
  template Image< llint > &operator/=( Image< llint > &img1, const Image< llint > &img2 );
  template Image< llint > operator+( const Image< llint > &img, double scalar );
  template Image< llint > operator+( double scalar, const Image< llint > &img );
  template Image< llint > &operator+=( Image< llint > &img, double scalar );
  template Image< llint > operator-( const Image< llint > &img, double scalar );
  template Image< llint > operator-( double scalar, const Image< llint > &img );
  template Image< llint > &operator-=( Image< llint > &img, double scalar );
  template Image< llint > operator*( const Image< llint > &img, double scalar );
  template Image< llint > operator*( double scalar, const Image< llint > &img );
  template Image< llint > &operator*=( Image< llint > &img, double scalar );
  template Image< llint > operator/( const Image< llint > &img, double scalar );
  template Image< llint > operator/( double scalar, const Image< llint > &img );
  template Image< llint > &operator/=( Image< llint > &img, double scalar );

  template Image< float > operator+( const Image< float > &img1, const Image< float > &img2 );
  template Image< float > &operator+=( Image< float > &img1, const Image< float > &img2 );
  template Image< float > operator-( const Image< float > &img1, const Image< float > &img2 );
  template Image< float > &operator-=( Image< float > &img1, const Image< float > &img2 );
  template Image< float > operator*( const Image< float > &img1, const Image< float > &img2 );
  template Image< float > &operator*=( Image< float > &img1, const Image< float > &img2 );
  template Image< float > operator/( const Image< float > &img1, const Image< float > &img2 );
  template Image< float > &operator/=( Image< float > &img1, const Image< float > &img2 );
  template Image< float > operator+( const Image< float > &img, double scalar );
  template Image< float > operator+( double scalar, const Image< float > &img );
  template Image< float > &operator+=( Image< float > &img, double scalar );
  template Image< float > operator-( const Image< float > &img, double scalar );
  template Image< float > operator-( double scalar, const Image< float > &img );
  template Image< float > &operator-=( Image< float > &img, double scalar );
  template Image< float > operator*( const Image< float > &img, double scalar );
  template Image< float > operator*( double scalar, const Image< float > &img );
  template Image< float > &operator*=( Image< float > &img, double scalar );
  template Image< float > operator/( const Image< float > &img, double scalar );
  template Image< float > operator/( double scalar, const Image< float > &img );
  template Image< float > &operator/=( Image< float > &img, double scalar );

  template Image< double > operator+( const Image< double > &img1, const Image< double > &img2 );
  template Image< double > &operator+=( Image< double > &img1, const Image< double > &img2 );
  template Image< double > operator-( const Image< double > &img1, const Image< double > &img2 );
  template Image< double > &operator-=( Image< double > &img1, const Image< double > &img2 );
  template Image< double > operator*( const Image< double > &img1, const Image< double > &img2 );
  template Image< double > &operator*=( Image< double > &img1, const Image< double > &img2 );
  template Image< double > operator/( const Image< double > &img1, const Image< double > &img2 );
  template Image< double > &operator/=( Image< double > &img1, const Image< double > &img2 );
  template Image< double > operator+( const Image< double > &img, double scalar );
  template Image< double > operator+( double scalar, const Image< double > &img );
  template Image< double > &operator+=( Image< double > &img, double scalar );
  template Image< double > operator-( const Image< double > &img, double scalar );
  template Image< double > operator-( double scalar, const Image< double > &img );
  template Image< double > &operator-=( Image< double > &img, double scalar );
  template Image< double > operator*( const Image< double > &img, double scalar );
  template Image< double > operator*( double scalar, const Image< double > &img );
  template Image< double > &operator*=( Image< double > &img, double scalar );
  template Image< double > operator/( const Image< double > &img, double scalar );
  template Image< double > operator/( double scalar, const Image< double > &img );
  template Image< double > &operator/=( Image< double > &img, double scalar );

  template Image< Color > operator+( const Image< Color > &img1, const Image< Color > &img2 );
  template Image< Color > &operator+=( Image< Color > &img1, const Image< Color > &img2 );
  template Image< Color > operator-( const Image< Color > &img1, const Image< Color > &img2 );
  template Image< Color > &operator-=( Image< Color > &img1, const Image< Color > &img2 );
  template Image< Color > operator*( const Image< Color > &img1, const Image< Color > &img2 );
  template Image< Color > &operator*=( Image< Color > &img1, const Image< Color > &img2 );
  template Image< Color > operator/( const Image< Color > &img1, const Image< Color > &img2 );
  // template Image< Color > &operator/=( Image< Color > &img1, const Image< Color > &img2 );
  template Image< Color > operator+( const Image< Color > &img, double scalar );
  template Image< Color > operator+( double scalar, const Image< Color > &img );
  template Image< Color > &operator+=( Image< Color > &img, double scalar );
  template Image< Color > operator-( const Image< Color > &img, double scalar );
  template Image< Color > operator-( double scalar, const Image< Color > &img );
  template Image< Color > &operator-=( Image< Color > &img, double scalar );
  template Image< Color > operator*( const Image< Color > &img, double scalar );
  template Image< Color > operator*( double scalar, const Image< Color > &img );
  template Image< Color > &operator*=( Image< Color > &img, double scalar );
  template Image< Color > operator/( const Image< Color > &img, double scalar );
  // template Image< Color > operator/( double scalar, const Image< Color > &img );
  template Image< Color > &operator/=( Image< Color > &img, double scalar );

  template Image< RealColor > operator+( const Image< RealColor > &img1, const Image< RealColor > &img2 );
  template Image< RealColor > &operator+=( Image< RealColor > &img1, const Image< RealColor > &img2 );
  template Image< RealColor > operator-( const Image< RealColor > &img1, const Image< RealColor > &img2 );
  template Image< RealColor > &operator-=( Image< RealColor > &img1, const Image< RealColor > &img2 );
  template Image< RealColor > operator*( const Image< RealColor > &img1, const Image< RealColor > &img2 );
  template Image< RealColor > &operator*=( Image< RealColor > &img1, const Image< RealColor > &img2 );
  template Image< RealColor > operator/( const Image< RealColor > &img1, const Image< RealColor > &img2 );
  // template Image< RealColor > &operator/=( Image< RealColor > &img1, const Image< RealColor > &img2 );
  template Image< RealColor > operator+( const Image< RealColor > &img, double scalar );
  template Image< RealColor > operator+( double scalar, const Image< RealColor > &img );
  template Image< RealColor > &operator+=( Image< RealColor > &img, double scalar );
  template Image< RealColor > operator-( const Image< RealColor > &img, double scalar );
  template Image< RealColor > operator-( double scalar, const Image< RealColor > &img );
  template Image< RealColor > &operator-=( Image< RealColor > &img, double scalar );
  template Image< RealColor > operator*( const Image< RealColor > &img, double scalar );
  template Image< RealColor > operator*( double scalar, const Image< RealColor > &img );
  template Image< RealColor > &operator*=( Image< RealColor > &img, double scalar );
  template Image< RealColor > operator/( const Image< RealColor > &img, double scalar );
  // template Image< RealColor > operator/( double scalar, const Image< RealColor > &img );
  template Image< RealColor > &operator/=( Image< RealColor > &img, double scalar );

  template void Image< int >::SetRange( int min, int max );
  template void Image< llint >::SetRange( llint min, llint max );
  template void Image< float >::SetRange( float min, float max );
  template void Image< double >::SetRange( double min, double max );
  template void Image< Color >::SetRange( uchar min, uchar max );
  template void Image< Color >::SetRange( char min, char max );
  template void Image< Color >::SetRange( int min, int max );
  template void Image< Color >::SetRange( size_t min, size_t max );
  template void Image< Color >::SetRange( float min, float max );
  template void Image< Color >::SetRange( double min, double max );
  template void Image< RealColor >::SetRange( int min, int max );
  template void Image< RealColor >::SetRange( size_t min, size_t max );
  template void Image< RealColor >::SetRange( float min, float max );
  template void Image< RealColor >::SetRange( double min, double max );

#endif

}

#endif

#endif
