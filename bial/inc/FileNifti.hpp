/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/08 
 * @brief Reading and writing support for Nifti files. 
 */

#include "Common.hpp"

#ifndef BIALFILENIFTI_H
#define BIALFILENIFTI_H

namespace Bial {

  class NiftiHeader;
  template< class D >
  class Image;

  /**
   * @date 2012/Jul/02
   * @param filename: Source filename to be readed.
   * @return A reference to the created scene.
   * @brief Open a Nifti file for reading and store its content in 'this'.
   * @warning none.
   */
  template< class D >
  static Image< D > ReadNifti( const std::string &filename );

  /**
   * @date 2012/Jul/03
   * @param img: input image.
   * @param filename: Source file to be written.
   * @return A reference to the input image.
   * @brief Open a Nifti file to write the content from 'this'.
   * @warning none.
   */

  template< class D >
  static void WriteNifti( const Image< D > &img, const std::string &filename, const NiftiHeader &hdr );

}

/* Implementation --------------------------------------------------------------------------------------------------- */

#include "Color.hpp"
#include "ColorRGB.hpp"
#include "File.hpp"
#include "NiftiHeader.hpp"

namespace Bial {

  template< >
  Image< Color > ReadNifti( const std::string &filename ) {
    try {
      COMMENT( "Reading nifti file from given name: " << filename, 2 );
      COMMENT( "Reading header for image and pixel dimensions, and for data type.", 2 );
      NiftiHeader hdr( filename );
      Vector< size_t > dim = hdr.Dim( );
      Vector< float > pixdim = hdr.PixelSize( );

      COMMENT( "Seting the parameters for the constructor.", 2 );
      Vector< size_t > spc_dims( { dim[ 0 ], dim[ 1 ] } );
      Vector< float > pixel_size( { pixdim[ 0 ], pixdim[ 1 ] } );
      if( dim.size( ) > 2 ) {
        spc_dims.push_back( dim[ 2 ] );
        pixel_size.push_back( pixdim[ 2 ] );
      }
      size_t channels = 1;
      if( dim.size( ) > 4 ) {
        channels = dim[ 4 ];
      }
      if( channels == 1 ) {
        Image< int > gray_img( ReadNifti< int >( filename ) );
        return( ColorSpace::GraytoARGB( gray_img ) );
      }
      if( channels > 4 ) {
        std::string msg( BIAL_ERROR( "Cannot handle more than 4 channel Nifti image." ) );
        throw( std::runtime_error( msg ) );
      }
      if( dim[ 4 ] > 1 ) {
        std::string msg( BIAL_ERROR( "Cannot handle Nifti time series." ) );
        throw( std::runtime_error( msg ) );
      }

      COMMENT( "Channels: " << channels << ". Creating resultant image.", 2 );
      Image< Color > res( spc_dims, pixel_size );
      size_t img_size = res.size( );
      COMMENT( "Getting filename and opening it.", 2 );
      std::string imgname( NiftiHeader::ExistingDataFileName( filename ) );
      COMMENT( "Image name: " << imgname, 2 );
      std::string
        extension( File::ToLowerExtension( filename,
                                           static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) )
                                                                            - 8 ) ) ) );
      IFile file;
      file.exceptions( std::ios::eofbit | std::ios::failbit | std::ios::badbit | std::ios::goodbit );
      file.open( imgname );

      COMMENT( "Seeking to the appropriate read position.", 2 );
      size_t offset;
      if( extension.rfind( ".nii" ) != std::string::npos ) {
        offset = NiftiHeader::NIFTI_HEADER_SIZE + 4u;
        file.ignore( offset );
      }
      COMMENT( "Reading data.", 2 );
      size_t single_bytes = hdr.BitPix( ) / 8;
      size_t total_bytes = single_bytes * img_size;
      char *dataptr = new char[ total_bytes ];
      file.read( &( dataptr[ 0 ] ), total_bytes ); /* data input */
      if( ( !file.good( ) ) || file.eof( ) || file.fail( ) || file.bad( ) ) {
        file.close( );
        std::string msg( BIAL_ERROR( "Error opening/reading Nifti file." ) );
        throw( std::ios_base::failure( msg ) );
      }
      file.close( );

      COMMENT( "Byte swap array if needed.", 2 );
      bool swap = NiftiHeader::IsSwapped( filename );
      if( ( swap == true ) && ( single_bytes > 1 ) ) {
        NiftiHeader::SwapNBytes( img_size, single_bytes, dataptr );
      }
      COMMENT( "Data type conversion.", 2 );
      switch( hdr.DataType( ) ) {
          case NiftiType::INT32: {
          int *data = reinterpret_cast< int* >( dataptr );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 0; chl < channels; ++chl )
              res( pxl )( chl ) = static_cast< uchar >( data[ pxl + chl * img_size ] );
          }
          break;
        }
          case NiftiType::UINT32: {
          unsigned int *data = reinterpret_cast< unsigned int* >( dataptr );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 0; chl < channels; ++chl )
              res( pxl )( chl ) = static_cast< uchar >( data[ pxl + chl * img_size ] );
          }
          break;
        }
          case NiftiType::INT16: {
          short *data = reinterpret_cast< short* >( dataptr );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 0; chl < channels; ++chl )
              res( pxl )( chl ) = static_cast< uchar >( data[ pxl + chl * img_size ] );
          }
          break;
        }
          case NiftiType::UINT16: {
          unsigned short *data = reinterpret_cast< unsigned short* >( dataptr );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 0; chl < channels; ++chl )
              res( pxl )( chl ) = static_cast< uchar >( data[ pxl + chl * img_size ] );
          }
          break;
        }
          case NiftiType::INT8: {
          char *data = reinterpret_cast< char* >( dataptr );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 0; chl < channels; ++chl )
              res( pxl )( chl ) = static_cast< uchar >( data[ pxl + chl * img_size ] );
          }
          break;
        }
          case NiftiType::UINT8: {
          unsigned char *data = reinterpret_cast< unsigned char* >( dataptr );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 0; chl < channels; ++chl )
              res( pxl )( chl ) = static_cast< uchar >( data[ pxl + chl * img_size ] );
          }
          break;
        }
          case NiftiType::FLOAT32: {
          float *data = reinterpret_cast< float* >( dataptr );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 0; chl < channels; ++chl ) {
              if( !std::isfinite( data[ pxl ] ) ) {
                res( pxl )( chl ) = 0;
              }
              else {
                res( pxl )( chl ) = static_cast< uchar >( data[ pxl + chl * img_size ] );
              }
            }
          }
          break;
        }
          case NiftiType::FLOAT64: {
          double *data = reinterpret_cast< double* >( dataptr );
          for( size_t pxl = 0; pxl < img_size; ++pxl ) {
            for( size_t chl = 0; chl < channels; ++chl ) {
              if( !std::isfinite( data[ pxl ] ) ) {
                res( pxl )( chl ) = 0;
              }
              else {
                res( pxl )( chl ) = static_cast< uchar >( data[ pxl + chl * img_size ] );
              }
            }
          }
          break;
        }
          default: {
          delete[]( reinterpret_cast< unsigned char* >( dataptr ) );
          std::string msg( BIAL_ERROR( "Unsupported nifti data type." ) );
          throw( std::logic_error( msg ) );
        }
      }
      delete[]( reinterpret_cast< unsigned char* >( dataptr ) );
      return( res );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing Nifti file." ) );
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
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  Image< D > ReadNifti( const std::string &filename ) {
    try {
      COMMENT( "Reading nifti file from given name: " << filename, 2 );
      COMMENT( "Reading header for image and pixel dimensions, and for data type.", 2 );
      NiftiHeader hdr( filename );
      COMMENT( "Header: " << hdr, 2 );
      Vector< size_t > dim = hdr.Dim( );
      Vector< float > pixdim = hdr.PixelSize( );

      COMMENT( "Seting the parameters for the constructor.", 2 );
      Vector< size_t > spc_dims( { dim[ 0 ], dim[ 1 ] } );
      Vector< float > pixel_size( { pixdim[ 0 ], pixdim[ 1 ] } );
      if( dim.size( ) > 2 ) {
        spc_dims.push_back( dim[ 2 ] );
        pixel_size.push_back( pixdim[ 2 ] );
      }
      size_t channels = 1;
      if( dim.size( ) > 4 ) {
        channels = dim[ 4 ];
      }
      COMMENT( "Channels: " << channels << ". Creating resultant image.", 2 );
      if( channels > 4 ) {
        std::string msg( BIAL_ERROR( "Cannot handle more than 4 channel Nifti image." ) );
        throw( std::runtime_error( msg ) );
      }
      if( ( dim.size( ) > 3 ) && ( dim[ 3 ] > 1 ) ) {
        std::string msg( BIAL_ERROR( "Cannot handle Nifti time series." ) );
        throw( std::runtime_error( msg ) );
      }
      if( channels > 1 ) {
        Image< Color > color_img( ReadNifti< Color >( filename ) );
        return( ColorSpace::ARGBtoGraybyBrightness< D >( color_img ) );
      }

      Image< D > res( spc_dims, pixel_size );
      COMMENT( "Getting filename and opening it.", 2 );
      std::string imgname( NiftiHeader::ExistingDataFileName( filename ) );
      COMMENT( "Image name: " << imgname, 2 );
      std::string extension( File::ToLowerExtension
                             ( filename, static_cast< size_t >
                               ( std::max( 0, static_cast< int >( filename.size( ) ) - 8 ) ) ) );
      IFile file;
      file.exceptions( std::ios::eofbit | std::ios::failbit | std::ios::badbit | std::ios::goodbit );
      file.open( imgname );

      COMMENT( "Seeking to the appropriate read position.", 2 );
      size_t offset;
      if( extension.rfind( ".nii" ) != std::string::npos ) {
        offset = NiftiHeader::NIFTI_HEADER_SIZE + 4u;
        file.ignore( offset );
      }
      COMMENT( "Reading data.", 2 );
      size_t single_bytes = hdr.BitPix( ) / 8;
      size_t total_bytes = single_bytes * res.size( );
      char *dataptr = new char[ total_bytes ];
      file.read( &( dataptr[ 0 ] ), total_bytes ); /* data input */
      if( ( !file.good( ) ) || file.eof( ) || file.fail( ) || file.bad( ) ) {
        file.close( );
        std::string msg( BIAL_ERROR( "Error opening/reading Nifti file." ) );
        throw( std::ios_base::failure( msg ) );
      }
      file.close( );

      bool swap = NiftiHeader::IsSwapped( filename );
      COMMENT( "Is swapped??????????? " << swap, 2 );
      if( ( swap == true ) && ( single_bytes > 1 ) ) {
        COMMENT( "Byte swap array if needed.", 2 );
        NiftiHeader::SwapNBytes( res.size( ), single_bytes, dataptr );
      }
      COMMENT( "Data type conversion.", 2 );
      switch( hdr.DataType( ) ) {
        case NiftiType::INT32: {
          COMMENT( "Int 32.", 2 );
          int *data = reinterpret_cast< int* >( dataptr );
          for( size_t p = 0; p < res.size( ); ++p ) {
            res( p ) = static_cast< D >( data[ p ] );
          }
          break;
        }
        case NiftiType::UINT32: {
          COMMENT( "UInt 32.", 2 );
          unsigned int *data = reinterpret_cast< unsigned int* >( dataptr );
          for( size_t p = 0; p < res.size( ); ++p ) {
            res( p ) = static_cast< D >( data[ p ] );
          }
          break;
        }
        case NiftiType::INT16: {
          COMMENT( "Int 16.", 2 );
          short *data = reinterpret_cast< short* >( dataptr );
          for( size_t p = 0; p < res.size( ); ++p ) {
            res( p ) = static_cast< D >( data[ p ] );
          }
          break;
        }
        case NiftiType::UINT16: {
          COMMENT( "UInt 16.", 2 );
          unsigned short *data = reinterpret_cast< unsigned short* >( dataptr );
          for( size_t p = 0; p < res.size( ); ++p ) {
            res( p ) = static_cast< D >( data[ p ] );
          }
          break;
        }
        case NiftiType::INT8: {
          COMMENT( "Int 8.", 2 );
          char *data = reinterpret_cast< char* >( dataptr );
          for( size_t p = 0; p < res.size( ); ++p ) {
            res( p ) = static_cast< D >( data[ p ] );
          }
          break;
        }
        case NiftiType::UINT8: {
          COMMENT( "UInt 8.", 2 );
          unsigned char *data = reinterpret_cast< unsigned char* >( dataptr );
          for( size_t p = 0; p < res.size( ); ++p ) {
            res( p ) = static_cast< D >( data[ p ] );
          }
          break;
        }
        case NiftiType::FLOAT32: {
          COMMENT( "Flt 32.", 2 );
          float *data = reinterpret_cast< float* >( dataptr );
          for( size_t p = 0; p < res.size( ); ++p ) {
            if( !std::isfinite( data[ p ] ) ) {
              res( p ) = 0;
            }
            else {
              res( p ) = static_cast< D >( data[ p ] );
            }
          }
          break;
        }
        case NiftiType::FLOAT64: {
          COMMENT( "Flt 64.", 2 );
          double *data = reinterpret_cast< double* >( dataptr );
          for( size_t p = 0; p < res.size( ); ++p ) {
            if( !std::isfinite( data[ p ] ) ) {
              res( p ) = 0;
            }
            else {
              res( p ) = static_cast< D >( data[ p ] );
            }
          }
          break;
        }
        default: {
          delete[]( reinterpret_cast< unsigned char* >( dataptr ) );
          std::string msg( BIAL_ERROR( "Unsupported nifti data type." ) );
          throw( std::logic_error( msg ) );
        }
      }
      delete[] ( reinterpret_cast< unsigned char* >( dataptr ) );
      return( res );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/reading/closing Nifti file." ) );
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
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  void WriteNifti( const Image< D > &img, const std::string &filename, const NiftiHeader &hdr ) {
    try {
      COMMENT( "Defining extension.", 2 );
      bool one_file = false;
      if( filename.rfind( ".nii" ) != std::string::npos ) {
        one_file = true;
      }
      COMMENT( "Opening file.", 2 );
      std::string hdrname( NiftiHeader::HeaderFileName( filename ) );
      COMMENT( "Header name: " << hdrname, 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( hdrname );
      COMMENT( "Writing header containts.", 2 );
      NiftiHeader new_hdr( hdr );
      for( size_t dms = 0; dms < img.Dims( ); ++ dms ) {
        new_hdr.Size( dms, img.size( dms ) );
        new_hdr.PixelSize( dms, img.PixelSize( dms ) );
      }
      new_hdr.Write( file, one_file );

      COMMENT( "Opening data file if using img and hdr extensions.", 2 );
      if( !one_file ) {
        file.close( );
        std::string imgname( NiftiHeader::DataFileName( filename ) );
        file.open( imgname );
      }
      COMMENT( "Getting data description.", 2 );
      switch( new_hdr.DataType( ) ) {
      case NiftiType::INT32: {
        COMMENT( "Converting data to nifti format. _data._size: " << img.size( ) << ".", 2 );
        Vector< int > write_data( img.size( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          write_data[ pxl ] = static_cast< int >( img( pxl ) );
        }
        file.write( reinterpret_cast< char* >( write_data.data( ) ), write_data.size( ) * sizeof( int ) );
        break;
      }
      case NiftiType::UINT32: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< unsigned int > write_data( img.size( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          write_data[ pxl ] = static_cast< int >( img( pxl ) );
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( unsigned int ) );
        break;
      }
      case NiftiType::INT16: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< short > write_data( img.size( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          write_data[ pxl ] = static_cast< int >( img( pxl ) );
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( short ) );
        break;
      }
      case NiftiType::UINT16: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< unsigned short > write_data( img.size( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          write_data[ pxl ] = static_cast< int >( img( pxl ) );
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( unsigned short ) );
        break;
      }
      case NiftiType::INT8: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< char > write_data( img.size( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          write_data[ pxl ] = static_cast< int >( img( pxl ) );
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( char ) );
        break;
      }
      case NiftiType::UINT8: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< unsigned char > write_data( img.size( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          write_data[ pxl ] = static_cast< int >( img( pxl ) );
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( unsigned char ) );
        break;
      }
      case NiftiType::FLOAT32: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< float > write_data( img.size( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          write_data[ pxl ] = static_cast< int >( img( pxl ) );
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( float ) );
        break;
      }
      case NiftiType::FLOAT64: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< double > write_data( img.size( ) );
        for( size_t pxl = 0; pxl < img.size( ); ++pxl ) {
          write_data[ pxl ] = static_cast< int >( img( pxl ) );
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( double ) );
        break;
      }
      default:
        file.close( );
        std::string msg( BIAL_ERROR( "Unsupported nifti data type." ) );
        throw( std::logic_error( msg ) );
      }
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing Nifti file." ) );
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
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< >
  void WriteNifti( const Image< Color > &img, const std::string &filename, const NiftiHeader &hdr ) {
    try {
      COMMENT( "Defining extension.", 2 );
      bool one_file = false;
      if( filename.rfind( ".nii" ) != std::string::npos ) {
        one_file = true;
      }
      COMMENT( "Opening file.", 2 );
      std::string hdrname( NiftiHeader::HeaderFileName( filename ) );
      COMMENT( "Header name: " << hdrname, 2 );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( hdrname );
      COMMENT( "Writing header containts.", 2 );
      NiftiHeader new_hdr( hdr );
      for( size_t dms = 0; dms < img.Dims( ); ++ dms ) {
        new_hdr.Size( dms, img.size( dms ) );
        new_hdr.PixelSize( dms, img.PixelSize( dms ) );
      }
      new_hdr.Write( file, one_file );
      COMMENT( "Opening data file if using img and hdr extensions.", 2 );
      if( !one_file ) {
        file.close( );
        std::string imgname( NiftiHeader::DataFileName( filename ) );
        file.open( imgname );
      }
      COMMENT( "Getting image information.", 2 );
      size_t img_size = img.size( );

      COMMENT( "Getting data description.", 2 );
      switch( new_hdr.DataType( ) ) {
      case NiftiType::INT32: {
        COMMENT( "Converting data to nifti format. _data._size: " << img_size << ".", 2 );
        Vector< int > write_data( img_size );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl ) {
            COMMENT( "Computing coordinates and assigning data.", 4 );
            size_t coord = pxl + chl * img_size;
            write_data[ coord ] = static_cast< int >( img( pxl )( chl ) );
          }
        }
        file.write( reinterpret_cast< char* >( write_data.data( ) ), write_data.size( ) * sizeof( int ) );
        break;
      }
      case NiftiType::UINT32: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< unsigned int > write_data( img_size );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl ) {
            COMMENT( "Computing coordinates and assigning data.", 4 );
            size_t coord = pxl + chl * img_size;
            write_data[ coord ] = static_cast< unsigned int >( img( pxl )( chl ) );
          }
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( unsigned int ) );
        break;
      }
      case NiftiType::INT16: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< short > write_data( img_size );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl ) {
            COMMENT( "Computing coordinates and assigning data.", 4 );
            size_t coord = pxl + chl * img_size;
            write_data[ coord ] = static_cast< short >( img( pxl )( chl ) );
          }
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( short ) );
        break;
      }
      case NiftiType::UINT16: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< unsigned short > write_data( img_size );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl ) {
            COMMENT( "Computing coordinates and assigning data.", 4 );
            size_t coord = pxl + chl * img_size;
            write_data[ coord ] = static_cast< unsigned short >( img( pxl )( chl ) );
          }
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( unsigned short ) );
        break;
      }
      case NiftiType::INT8: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< char > write_data( img_size );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl ) {
            COMMENT( "Computing coordinates and assigning data.", 4 );
            size_t coord = pxl + chl * img_size;
            write_data[ coord ] = static_cast< char >( img( pxl )( chl ) );
          }
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( char ) );
        break;
      }
      case NiftiType::UINT8: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< unsigned char > write_data( img_size );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl ) {
            COMMENT( "Computing coordinates and assigning data.", 4 );
            size_t coord = pxl + chl * img_size;
            write_data[ coord ] = static_cast< unsigned char >( img( pxl )( chl ) );
          }
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( unsigned char ) );
        break;
      }
      case NiftiType::FLOAT32: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< float > write_data( img_size );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl ) {
            COMMENT( "Computing coordinates and assigning data.", 4 );
            size_t coord = pxl + chl * img_size;
            write_data[ coord ] = static_cast< float >( img( pxl )( chl ) );
          }
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( float ) );
        break;
      }
      case NiftiType::FLOAT64: {
        COMMENT( "Converting data to nifti format.", 2 );
        Vector< double > write_data( img_size );
        for( size_t pxl = 0; pxl < img_size; ++pxl ) {
          for( size_t chl = 0; chl < 4; ++chl ) {
            COMMENT( "Computing coordinates and assigning data.", 2 );
            size_t coord = pxl + chl * img_size;
            write_data[ coord ] = static_cast< double >( img( pxl )( chl ) );
          }
        }
        COMMENT( "Writing data.", 2 );
        file.write( reinterpret_cast< char* >( &write_data[ 0 ] ), write_data.size( ) * sizeof( double ) );
        break;
      }
      default:
        file.close( );
        std::string msg( BIAL_ERROR( "Unsupported nifti data type." ) );
        throw( std::logic_error( msg ) );
      }
      file.close( );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error opening/writing/closing Nifti file." ) );
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
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

}

#endif
