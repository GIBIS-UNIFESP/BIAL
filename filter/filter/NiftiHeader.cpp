/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Jul/02
 * @brief NiftiHeader class to deal with magnetic resonance images#include "Color.hpp".
 * <br> Future add-on's: Complete nifti header extension support and analyze 7.5 support.
 */

/* This implementation was based on Robert W Cox, SSCC/DIRP/NIMH/NIH/DHHS/USA/EARTH c code.
 * Source codes: nifti1_io.h, nifti1.h, and nifti1_io.c
 */

#ifndef BIALNIFTIHEADER_C
#define BIALNIFTIHEADER_C

#include "NiftiHeader.hpp"
#include "Color.hpp"
#include "File.hpp"
#include "Image.hpp"
#include "MatrixDeterminant.hpp"
#include "MatrixPolar.hpp"
#include "RealColor.hpp"

/* Implementation --------------------------------------------------------------------------------------------------- */

namespace Bial {

  NiftiHeader::NiftiHeader( ) try 
    : data_type( 10, '\0' ), db_name( 18, '\0' ),extents( NiftiHeader::ANALYZE_EXTENT ), session_error( 0 ), 
        regular( 'r' ), freq_dim( 0 ), phase_dim( 0 ), slice_dim( 0 ), dim( 8, 0 ), intent_p1( 0.0f ), intent_p2( 0.0f ),
        intent_p3( 0.0f ), intent_code( NiftiIntent::NONE ), datatype( NiftiType::INT8 ), bitpix( 8 ), slice_start( 0 ),
        pixdim( 8, 0.0f ), vox_offset( NiftiHeader::NIFTI_HEADER_SIZE + 4.0 ), scl_slope( 0.0f ), scl_inter( 0.0f ), 
        slice_end( 0 ), slice_code( NiftiSlice::UNKNOWN ), xyz_units( NiftiUnit::UNKNOWN ), 
        time_units( NiftiUnit::UNKNOWN ), cal_max( 0.0f ), cal_min( 0.0f ), slice_duration( 0.0f ), toffset( 0.0f ), 
        glmax( 255 ), glmin( 0 ), descrip( 80, '\0' ), aux_file( 24, '\0' ), qform_code( NiftiXForm::UNKNOWN ), 
        sform_code( NiftiXForm::UNKNOWN ), quatern_b( 0.0f ), quatern_c( 0.0f ), quatern_d( 0.0f ), qoffset_x( 0.0f ), 
        qoffset_y( 0.0f ), qoffset_z( 0.0f ), qfac( 1.0f ), qtm( 4, 4 ), stm( 4, 4 ), intent_name( 16, '\0' ) {
      qtm.Set( 0.0f );
      stm.Set( 0.0f );
      qtm[ 0 ] = qtm[ 5 ] = qtm[ 10 ] = qtm[ 15 ] = 1.0f;
      stm[ 0 ] = stm[ 5 ] = stm[ 10 ] = stm[ 15 ] = 1.0f;
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

  NiftiHeader::NiftiHeader( const Vector< size_t > &new_dim, const Vector< float > &new_pixdim, NiftiType new_datatype,
                            bool one_file ) try 
    : data_type( 10, '\0' ), db_name( 18, '\0' ), extents( NiftiHeader::ANALYZE_EXTENT ), session_error( 0 ), 
        regular( 'r' ), freq_dim( 0 ), phase_dim( 0 ), slice_dim( 0 ), dim( new_dim ), intent_p1( 0.0f ), 
        intent_p2( 0.0f ), intent_p3( 0.0f ), intent_code( NiftiIntent::NONE ), datatype( new_datatype ), bitpix( 8 ),
        slice_start( 0 ), pixdim( new_pixdim ), vox_offset( NiftiHeader::NIFTI_HEADER_SIZE + 4.0 ), scl_slope( 0.0f ),
        scl_inter( 0.0f ), slice_end( 0 ), slice_code( NiftiSlice::UNKNOWN ), xyz_units( NiftiUnit::UNKNOWN ), 
        time_units( NiftiUnit::UNKNOWN ), cal_max( 0.0f ), cal_min( 0.0f ), slice_duration( 0.0f ), toffset( 0.0f ), 
        glmax( 255 ), glmin( 0 ), descrip( 80, '\0' ), aux_file( 24, '\0' ), qform_code( NiftiXForm::UNKNOWN ), 
        sform_code( NiftiXForm::UNKNOWN ), quatern_b( 0.0f ), quatern_c( 0.0f ), quatern_d( 0.0f ), qoffset_x( 0.0f ), 
        qoffset_y( 0.0f ), qoffset_z( 0.0f ), qfac( 1.0f ), qtm( 4, 4 ), stm( 4, 4 ), intent_name( 16, '\0' ) {
      qtm.Set( 0.0f );
      stm.Set( 0.0f );
      qtm[ 0 ] = qtm[ 5 ] = qtm[ 10 ] = qtm[ 15 ] = 1.0f;
      stm[ 0 ] = stm[ 5 ] = stm[ 10 ] = stm[ 15 ] = 1.0f;
      SetBitPix( );
      if( !one_file ) {
        vox_offset = 0.0f;
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

  template< class D >
  NiftiHeader::NiftiHeader( const Image< D > &img, bool one_file ) try 
    : data_type( 10, '\0' ), db_name( 18, '\0' ), extents( NiftiHeader::ANALYZE_EXTENT ), session_error( 0 ), 
        regular( 'r' ), freq_dim( 0 ), phase_dim( 0 ), slice_dim( 0 ), dim( img.Dim( ) ), intent_p1( 0.0f ), 
        intent_p2( 0.0f ), intent_p3( 0.0f ), intent_code( NiftiIntent::NONE ), datatype( DataTypeDecode( img[ 0 ] ) ), 
        bitpix( 8 ), slice_start( 0 ), pixdim( img.PixelSize( ) ), vox_offset( NiftiHeader::NIFTI_HEADER_SIZE + 4.0 ),
        scl_slope( 0.0f ), scl_inter( 0.0f ), slice_end( 0 ), slice_code( NiftiSlice::UNKNOWN ),
        xyz_units( NiftiUnit::UNKNOWN ), time_units( NiftiUnit::UNKNOWN ), cal_max( 0.0f ), cal_min( 0.0f ),
        slice_duration( 0.0f ), toffset( 0.0f ), glmax( 255 ), glmin( 0 ), descrip( 80, '\0' ), aux_file( 24, '\0' ),
        qform_code( NiftiXForm::UNKNOWN ), sform_code( NiftiXForm::UNKNOWN ), quatern_b( 0.0f ), quatern_c( 0.0f ),
        quatern_d( 0.0f ), qoffset_x( 0.0f ), qoffset_y( 0.0f ), qoffset_z( 0.0f ), qfac( 1.0f ), qtm( 4, 4 ), stm( 4, 4 ),
        intent_name( 16, '\0' ) {
      if( img.Dims( ) == 2 )
        dim.pop_back( );
      qtm.Set( 0.0f );
      stm.Set( 0.0f );
      qtm[ 0 ] = qtm[ 5 ] = qtm[ 10 ] = qtm[ 15 ] = 1.0f;
      stm[ 0 ] = stm[ 5 ] = stm[ 10 ] = stm[ 15 ] = 1.0f;
      SetBitPix( );
      if( !one_file ) {
        vox_offset = 0.0f;
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


  NiftiHeader::NiftiHeader( const std::string &filename ) try 
    : data_type( 10, '\0' ), db_name( 18, '\0' ), extents( NiftiHeader::ANALYZE_EXTENT ), session_error( 0 ),
        regular( 'r' ), freq_dim( 0 ), phase_dim( 0 ), slice_dim( 0 ), dim( 8, 0 ), intent_p1( 0.0f ), intent_p2( 0.0f ),
        intent_p3( 0.0f ), intent_code( NiftiIntent::NONE ), datatype( NiftiType::INT8 ), bitpix( 8 ), slice_start( 0 ),
        pixdim( 8, 0.0f ), vox_offset( NiftiHeader::NIFTI_HEADER_SIZE + 4.0 ), scl_slope( 0.0f ), scl_inter( 0.0f ), 
        slice_end( 0 ), slice_code( NiftiSlice::UNKNOWN ), xyz_units( NiftiUnit::UNKNOWN ), 
        time_units( NiftiUnit::UNKNOWN ), cal_max( 0.0f ), cal_min( 0.0f ), slice_duration( 0.0f ), toffset( 0.0f ), 
        glmax( 255 ), glmin( 0 ), descrip( 80, '\0' ), aux_file( 24, '\0' ), qform_code( NiftiXForm::UNKNOWN ), 
        sform_code( NiftiXForm::UNKNOWN ), quatern_b( 0.0f ), quatern_c( 0.0f ), quatern_d( 0.0f ), qoffset_x( 0.0f ),
        qoffset_y( 0.0f ), qoffset_z( 0.0f ), qfac( 1.0f ), qtm( 4, 4 ), stm( 4, 4 ), intent_name( 16, '\0' ) {
      
      /* Opening file. */
      std::string hdrname( NiftiHeader::ExistingHeaderFileName( filename ) );
      IFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( hdrname );
      COMMENT( "Reading and translating header.", 2 );
      int hdr_size;
      file.read( reinterpret_cast< char* >( &hdr_size ), sizeof( int ) ); /* hdr_size */
      bool swap = false;
      if( hdr_size != NIFTI_HEADER_SIZE ) {
        NiftiHeader::Swap4Bytes( 1, reinterpret_cast< void* >( &hdr_size ) );
        if( hdr_size == NIFTI_HEADER_SIZE ) {
          swap = true;
        }
        else {
          std::string msg( BIAL_ERROR( "Corrupted nifti header." ) );
          throw( std::logic_error( msg ) );
        }
      }
      file.read( reinterpret_cast< char* >( &data_type[ 0 ] ), 10 * sizeof( char ) ); /* data_type */
      COMMENT( "data_type: " << data_type, 2 );
      file.read( reinterpret_cast< char* >( &db_name[ 0 ] ), 18 * sizeof( char ) ); /* db_name */
      COMMENT( "db_name: " << db_name, 2 );
      file.read( reinterpret_cast< char* >( &extents ), 7 * sizeof( char ) ); /* extents, session_error, regular */
      COMMENT( "extents: " << extents, 2 );
      char dim_info;
      file.read( reinterpret_cast< char* >( &dim_info ), sizeof( char ) ); /* dim_info */
      freq_dim = dim_info & 0x03;
      phase_dim = ( dim_info >> 2 ) & 0x03;
      slice_dim = ( dim_info >> 4 ) & 0x03;
      COMMENT( "freq_dim: " << freq_dim, 2 );
      COMMENT( "phase_dim: " << phase_dim, 2 );
      COMMENT( "slice_dim: " << slice_dim, 2 );
      Array< short, 8 > hdr_dim;
      file.read( reinterpret_cast< char* >( &hdr_dim ), 8 * sizeof( short ) ); /* hdr_dim */
      if( swap ) {
        NiftiHeader::Swap2Bytes( 1, reinterpret_cast< void* >( &( hdr_dim[ 0 ] ) ) );
      }
      if( ( hdr_dim[ 0 ] <= 1 ) || ( hdr_dim[ 0 ] > 5 ) ) {
        std::string msg( BIAL_ERROR( "Invalid number of dimensions. Expected: 2 to 5. Given: " +
                                     std::to_string( hdr_dim[ 0 ] ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "fixing bad hdr_dim values.", 2 );
      int dms;
      for( dms = hdr_dim[ 0 ]; dms > 1; --dms ) {
        if( swap ) {
          NiftiHeader::Swap2Bytes( 1, reinterpret_cast< void* >( &( hdr_dim[ dms ] ) ) );
        }
        if( ( hdr_dim[ dms ] != 0 ) && ( hdr_dim[ dms ] != 1 ) ) {
          break;
        }
      }
      dim = Vector< size_t >( dms );
      for( ; dms > 0; dms-- ) {
        if( swap ) {
          NiftiHeader::Swap2Bytes( 1, reinterpret_cast< void* >( &( hdr_dim[ dms ] ) ) );
        }
        if( hdr_dim[ dms ] <= 0 ) {
          dim[ dms - 1 ] = 1;
        }
        else {
          dim[ dms - 1 ] = hdr_dim[ dms ];
        }
      }
      file.read( reinterpret_cast< char* >( &intent_p1 ), 10 * sizeof( short ) );
      COMMENT( "intent_p1: " << intent_p1, 2 );
      COMMENT( "intent_p2: " << intent_p2, 2 );
      COMMENT( "intent_p3: " << intent_p3, 2 );
      COMMENT( "intent_code: " << static_cast< short >( intent_code ), 2 );
      COMMENT( "datatype: " << static_cast< short >( datatype ), 2 );
      COMMENT( "bitpix: " << bitpix, 2 );
      COMMENT( "slice_start: " << slice_start, 2 );

      Array< float, 8 > hdr_pixdim;
      file.read( reinterpret_cast< char* >( &hdr_pixdim ), 8 * sizeof( float ) ); /* hdr_pixdim */
      if( swap ) {
        NiftiHeader::Swap4Bytes( 1, reinterpret_cast< void* >( &( hdr_pixdim[ 0 ] ) ) );
      }
      if( hdr_pixdim[ 0 ] < 0.0f ) {
        qfac = -1.0f;
      }
      else {
        qfac = 1.0f;
      }
      pixdim = Vector< float >( dim.size( ) );
      for( dms = 0; dms < static_cast< int >( pixdim.size( ) ); ++dms ) {
        if( swap ) {
          NiftiHeader::Swap4Bytes( 1, reinterpret_cast< void* >( &( hdr_pixdim[ dms ] ) ) );
        }
        pixdim[ dms ] = hdr_pixdim[ dms + 1 ];
      }
      COMMENT( "vox_offset, scl_slope, scl_inter, slice_end, slice_code.", 2 );
      file.read( reinterpret_cast< char* >( &vox_offset ), 15 * sizeof( char ) );
      char xyzt_units;
      file.read( reinterpret_cast< char* >( &xyzt_units ), sizeof( char ) ); /* xyzt_units */
      xyz_units = static_cast< NiftiUnit >( xyzt_units & 0x07 );
      time_units = static_cast< NiftiUnit >( xyzt_units & 0x38 );
      COMMENT( "cal_max, cal_min, slice_duration, toffset, glmax, glmin", 2 );
      file.read( reinterpret_cast< char* >( &cal_max ), 6 * sizeof( float ) );
      file.read( reinterpret_cast< char* >( &descrip[ 0 ] ), 80 * sizeof( char ) ); /* descrip */
      file.read( reinterpret_cast< char* >( &aux_file[ 0 ] ), 24 * sizeof( char ) ); /* aux_file */
      COMMENT( "qform_code, sform_code, quatern_b, quatern_c, quatern_d, qoffset_x, qoffset_y, qoffset_z", 2 );
      qtm.Set( 0.0f );
      stm.Set( 0.0f );
      file.read( reinterpret_cast< char* >( &qform_code ), 14 * sizeof( short ) );
      file.read( reinterpret_cast< char* >( &stm[ 0 ] ), 12 * sizeof( float ) ); /* stm */
      file.read( reinterpret_cast< char* >( &intent_name[ 0 ] ), 16 * sizeof( char ) ); /* intent_name */
      std::string magic( 4, '\0' );
      file.read( reinterpret_cast< char* >( &magic[ 0 ] ), 4 * sizeof( char ) ); /* magic */
      file.close( );

      COMMENT( "Checking for header correct read.", 2 );
      if( ( tolower( magic[ 0 ] ) != 'n' ) || ( magic[ 3 ] != '\0' ) ||
          ( ( tolower( magic[ 1 ] ) != 'i' ) && ( magic[ 1 ] != '+' ) )
          || ( magic[ 2 ] < '1' ) || ( magic[ 2 ] > '9' ) ) {
        std::string msg( BIAL_ERROR( "Corrupted nifti image. Magic string is incorrect." ) );
        throw( std::logic_error( msg ) );
      }
      if( swap ) {
        SwapHeader( );
      }
      if( ( datatype != NiftiType::UINT8 ) && ( datatype != NiftiType::INT16 ) && ( datatype != NiftiType::INT32 ) &&
          ( datatype != NiftiType::INT64 ) && ( datatype != NiftiType::FLOAT32 ) && ( datatype != NiftiType::FLOAT64 ) &&
          ( datatype != NiftiType::INT8 ) && ( datatype != NiftiType::UINT16 ) && ( datatype != NiftiType::UINT32 ) &&
          ( datatype != NiftiType::UINT64 ) ) {
        std::string msg( BIAL_ERROR( "Unsupported nifti header." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "compute qtm transformation from pixel indexes (i,j,k) to (x,y,z)", 2 );
      if( qform_code <= NiftiXForm::UNKNOWN ) {
        COMMENT( "Use grid spacing for qtm.", 2 );
        qtm( 0, 0 ) = pixdim[ 0 ]; /* nim->dx; grid spacings */
        qtm( 1, 1 ) = pixdim[ 1 ]; /* nim->dy; along diagonal */
	if( pixdim.size( ) > 2 )
	  qtm( 2, 2 ) = pixdim[ 2 ]; /* nim->dz; */
	else
	  qtm( 2, 2 ) = 1;
        qtm( 0, 1 ) = qtm( 0, 2 ) = qtm( 0, 3 ) = 0.0f; /* off diagonal is zero */
        qtm( 1, 0 ) = qtm( 1, 2 ) = qtm( 1, 3 ) = 0.0f;
        qtm( 2, 0 ) = qtm( 2, 1 ) = qtm( 2, 3 ) = 0.0f;
        qtm( 3, 0 ) = qtm( 3, 1 ) = qtm( 3, 2 ) = 0.0f; /* last row is always [ 0 0 0 1 ] */
        qtm( 3, 3 ) = 1.0f;
        qform_code = NiftiXForm::UNKNOWN;
      }
      else {
        COMMENT( "Use the quaternion-specified transformation.", 2 );
        quatern_b = std::isfinite( quatern_b ) ? quatern_b : 0.0f;
        quatern_c = std::isfinite( quatern_c ) ? quatern_c : 0.0f;
        quatern_d = std::isfinite( quatern_d ) ? quatern_d : 0.0f;
        qoffset_x = std::isfinite( qoffset_x ) ? qoffset_x : 0.0f;
        qoffset_y = std::isfinite( qoffset_y ) ? qoffset_y : 0.0f;
        qoffset_z = std::isfinite( qoffset_z ) ? qoffset_z : 0.0f;
        QuaternToMat( pixdim );
      }
      if( sform_code <= NiftiXForm::UNKNOWN ) {
        COMMENT( "sform_code <= 0, then no sto transformation.", 2 );
        sform_code = NiftiXForm::UNKNOWN;
      }
      scl_slope = std::isfinite( scl_slope ) ? scl_slope : 0.0f;
      scl_inter = std::isfinite( scl_inter ) ? scl_inter : 0.0f;
      intent_p1 = std::isfinite( intent_p1 ) ? intent_p1 : 0.0f;
      intent_p2 = std::isfinite( intent_p2 ) ? intent_p2 : 0.0f;
      intent_p3 = std::isfinite( intent_p3 ) ? intent_p3 : 0.0f;
      toffset = std::isfinite( toffset ) ? toffset : 0.0f;
      slice_duration = std::isfinite( slice_duration ) ? slice_duration : 0.0f;
      cal_min = std::isfinite( cal_min ) ? cal_min : 0.0f;
      cal_max = std::isfinite( cal_max ) ? cal_max : 0.0f;
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
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  const Vector< size_t > &NiftiHeader::Dim( ) const {
    return( dim );
  }

  size_t NiftiHeader::Size( size_t dms ) const {
    return( dim[ dms ] );
  }

  void NiftiHeader::Size( size_t dms, size_t val ) {
    dim[ dms ] = val;
  }

  const Vector< float > &NiftiHeader::PixelSize( ) const {
    return( pixdim );
  }

  float NiftiHeader::PixelSize( size_t dms ) const {
    return( pixdim[ dms ] );
  }

  void NiftiHeader::PixelSize( size_t dms, float val ) {
    pixdim[ dms ] = val;
  }

  short NiftiHeader::BitPix( ) const {
    return( bitpix );
  }

  NiftiType NiftiHeader::DataType( ) const {
    return( datatype );
  }

  NiftiHeader NiftiHeader::Read( const std::string &filename ) {
    try {
      return( NiftiHeader( filename ) );
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
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  void NiftiHeader::Write( OFile &file, bool one_file ) const {
    try {
      COMMENT( "Writing header.", 2 );
      int hdr_size = NIFTI_HEADER_SIZE;
      file.write( reinterpret_cast< const char* >( &hdr_size ), sizeof( int ) ); /* hdr_size */
      file.write( data_type.c_str( ), 10 * sizeof( char ) ); /* data_type */
      file.write( db_name.c_str( ), 18 * sizeof( char ) ); /* db_name */
      file.write( reinterpret_cast< const char* >( &extents ), 7 * sizeof( char ) ); /*extents, session_error, regular*/
      char dim_info = ( static_cast< char >( freq_dim ) & 0x03 ) | ( ( static_cast< char >( phase_dim ) & 0x03 ) << 2 )
        | ( ( static_cast< char >( slice_dim ) & 0x03 ) << 4 );
      file.write( reinterpret_cast< const char* >( &dim_info ), sizeof( char ) ); /* dim_info */
      Vector< short > hdr_dim( 8, 1 );
      hdr_dim[ 0 ] = dim.size( );
      for( size_t dms = 0; dms < dim.size( ); ++dms )
        hdr_dim( dms + 1 ) = dim( dms );
      file.write( reinterpret_cast< const char* >( hdr_dim.data( ) ), 8 * sizeof( short ) ); /* hdr_dim */
      COMMENT( "intent_p1, float intent_p2, intent_p3, intent_code, datatype, bitpix, slice_start", 2 );
      file.write( reinterpret_cast< const char* >( &intent_p1 ), 10 * sizeof( short ) );
      Vector< float > hdr_pixdim( 8, 1.0f );
      hdr_pixdim( 0 ) = qfac;
      for( size_t dms = 0; dms < dim.size( ); ++dms )
        hdr_pixdim( dms + 1 ) = pixdim( dms );
      file.write( reinterpret_cast< const char* >( hdr_pixdim.data( ) ), 8 * sizeof( float ) ); /* hdr_pixdim */
      float vox_offset = ( one_file ? NiftiHeader::NIFTI_HEADER_SIZE + 4.0f : 0.0f );
      COMMENT( "vox_offset.", 2 );
      file.write( reinterpret_cast< const char* >( &vox_offset ), 4 * sizeof( char ) );
      COMMENT( "scl_slope, scl_inter, slice_end, slice_code.", 2 );
      file.write( reinterpret_cast< const char* >( &scl_slope ), 11 * sizeof( char ) );
      char xyzt_units = ( static_cast< char >( xyz_units ) & 0x07 ) | ( static_cast< char >( time_units ) & 0x38 );
      file.write( reinterpret_cast< const char* >( &xyzt_units ), sizeof( char ) ); /* xyzt_units */
      COMMENT( "cal_max, cal_min, slice_duration, toffset, glmax, glmin.", 2 );
      file.write( reinterpret_cast< const char* >( &cal_max ), 6 * sizeof( float ) );
      file.write( descrip.c_str( ), 80 * sizeof( char ) ); /* descrip */
      file.write( aux_file.c_str( ), 24 * sizeof( char ) ); /* aux_file */
      COMMENT( "qform_code,sform_code,quatern_b,quatern_c,quatern_d,qoffset_x,qoffset_y,qoffset_z.", 2 );
      file.write( reinterpret_cast< const char* >( &qform_code ), 14 * sizeof( short ) );
      file.write( reinterpret_cast< const char* >( &stm[ 0 ] ), 12 * sizeof( float ) ); /* stm */
      file.write( intent_name.c_str( ), 16 * sizeof( char ) ); /* intent_name */
      Vector< char > magic( 8, 0 );
      magic( 0 ) = 'n'; magic( 2 ) = '1';
      if( one_file ) {
        magic( 1 ) = '+';
      }
      else {
        magic( 1 ) = 'i';
      }
      file.write( magic.data( ), 8 * sizeof( char ) ); /* magic */
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
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  void NiftiHeader::Write( const std::string &filename ) const {
    try {
      /* Opening file */
      std::string hdrname( NiftiHeader::HeaderFileName( filename ) );
      OFile file;
      file.exceptions( std::fstream::failbit | std::fstream::badbit );
      file.open( hdrname );

      /* Writing header containts. */
      bool one_file = false;
      if( filename.rfind( ".nii" ) != std::string::npos ) {
        one_file = true;
      }
      Write( file, one_file );

      /* closing file. */
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
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  bool NiftiHeader::IsNiftiFile( const std::string &filename ) {
    try {
      std::string extension( File::ToLowerExtension( filename ) );
      if( ( extension.compare( ".nii" ) == 0 ) || ( extension.compare( ".nii.gz" ) == 0 ) ||
          ( extension.compare( ".hdr" ) == 0 ) || ( extension.compare( ".hdr.gz" ) == 0 ) ||
          ( extension.compare( ".img" ) == 0 ) || ( extension.compare( ".img.gz" ) == 0 ) ) {
        return( true );
      }
      return( false );
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

  std::string NiftiHeader::HeaderFileName( const std::string &filename ) {
    try {
      return( filename );
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

  std::string NiftiHeader::DataFileName( const std::string &filename ) {
    try {
      COMMENT( "Defining extension.", 2 );
      std::string extension
        ( File::ToLowerExtension( filename, static_cast< size_t >
                                  ( std::max( 0, static_cast< int >( filename.size( ) ) - 8 ) ) ) );
      std::string basename = File::Basename( filename, true );
      if( ( extension.compare( ".nii" ) == 0 ) || ( extension.compare( ".nii.gz" ) == 0 ) ||
          ( extension.compare( ".img" ) == 0 ) || ( extension.compare( ".img.gz" ) == 0 ) ) {
        return( filename );
      }
      else { /* ".hdr" or ".hdr.gz" */
        std::string imgname = basename + ".img";
        if( extension.compare( ".gz" ) == 0 ) {
          imgname = basename + ".gz";
        }
        return( imgname );
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
  }

  std::string NiftiHeader::ExistingHeaderFileName( const std::string &filename ) {
    try {
      COMMENT( "Defining extension.", 2 );
      std::string 
        extension( File::ToLowerExtension( filename,
                                           static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) -
                                                                            8 ) ) ) );
      COMMENT( "Defining basename.", 2 );
      std::string basename = File::Basename( filename, true );
      if( ( extension.compare( ".nii" ) == 0 ) || ( extension.compare( ".nii.gz" ) == 0 ) ||
          ( extension.compare( ".hdr" ) == 0 ) || ( extension.compare( ".hdr.gz" ) == 0 ) ) {
        return( filename );
      }
      else { /* ".img" or ".img.gz" Using multiple options to avoid operational system dependent calls and libraries. */
        std::string hdrname = basename + ".hdr";
        std::string truename = File::LocateCaseSensitiveFileName( hdrname );
        if( truename.compare( "" ) ) {
          return( truename );
        }
        hdrname = basename + ".hdr.gz";
        truename = File::LocateCaseSensitiveFileName( hdrname );
        if( truename.compare( "" ) ) {
          return( truename );
        }
        throw( std::exception( ) );
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
  }

  std::string NiftiHeader::ExistingDataFileName( const std::string &filename ) {
    try {
      COMMENT( "Defining extension.", 2 );
      std::string
  extension( File::ToLowerExtension( filename,
             static_cast< size_t >( std::max( 0, static_cast< int >( filename.size( ) ) -
                      8 ) ) ) );
      std::string basename = File::Basename( filename, true );
      if( ( extension.compare( ".nii" ) == 0 ) || ( extension.compare( ".nii.gz" ) == 0 ) ||
          ( extension.compare( ".img" ) == 0 ) || ( extension.compare( ".img.gz" ) == 0 ) ) {
        return( filename );
      }
      else { /* ".hdr" or ".hdr.gz" */
        std::string imgname = basename + ".img";
        std::string truename = File::LocateCaseSensitiveFileName( imgname );
        if( truename.compare( "" ) ) {
          return( truename );
        }
        imgname = basename + ".img.gz";
        truename = File::LocateCaseSensitiveFileName( imgname );
        if( truename.compare( "" ) ) {
          return( truename );
        }
        throw( std::exception( ) );
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
  }

  void NiftiHeader::SetBitPix( ) {
    switch( datatype ) {
    case NiftiType::INT8:
    case NiftiType::UINT8: {
      bitpix = 8;
      break;
    }
    case NiftiType::INT16:
    case NiftiType::UINT16: {
      bitpix = 16;
      break;
    }
    case NiftiType::INT32:
    case NiftiType::UINT32:
    case NiftiType::FLOAT32: {
      bitpix = 32;
      break;
    }
    case NiftiType::INT64:
    case NiftiType::UINT64:
    case NiftiType::FLOAT64: {
      bitpix = 64;
      break;
    }
    default: { /* All other data types. */
      std::cout << "void NiftiHeader::SetBitPix( ): Warning: Invalid data type. Setting to 8 bits signed integer."
                << std::endl;
      datatype = NiftiType::INT8;
      bitpix = 8;
      break;
    }
    }
  }

  NiftiType NiftiHeader::DataTypeDecode( const int ) {
    return( NiftiType::INT32 );
  }
  NiftiType NiftiHeader::DataTypeDecode( const long long ) {
    return( NiftiType::INT64 );
  }
  NiftiType NiftiHeader::DataTypeDecode( const float ) {
    return( NiftiType::FLOAT32 );
  }
  NiftiType NiftiHeader::DataTypeDecode( const double ) {
    return( NiftiType::FLOAT64 );
  }
  NiftiType NiftiHeader::DataTypeDecode( const Color ) {
    return( NiftiType::UINT8 );
  }
  NiftiType NiftiHeader::DataTypeDecode( const RealColor ) {
    return( NiftiType::FLOAT32 );
  }

  size_t NiftiHeader::AxialDimension( const std::string &orientation ) {
    COMMENT( "First dimension.", 2 );
    if( ( orientation.find_first_of( "S" ) == 0 ) || ( orientation.find_first_of( "I" ) == 0 ) ) {
      return( 0 );
    }
    COMMENT( "Second dimension.", 2 );
    if( ( orientation.find_first_of( "S" ) == 3 ) || ( orientation.find_first_of( "I" ) == 3 ) ) {
      return( 1 );
    }
    if( ( orientation.find_first_of( "S" ) == 6 ) || ( orientation.find_first_of( "I" ) == 6 ) ) {
      return( 2 );
    }
    std::string msg( BIAL_ERROR( "Invalid orientation string." ) );
    throw( std::logic_error( msg ) );
  }

  size_t NiftiHeader::CoronalDimension( const std::string &orientation ) {
    COMMENT( "First dimension.", 2 );
    if( ( orientation.find_first_of( "A" ) == 0 ) || ( orientation.find_first_of( "P" ) == 0 ) ) {
      return( 0 );
    }
    COMMENT( "Second dimension.", 2 );
    if( ( orientation.find_first_of( "A" ) == 3 ) || ( orientation.find_first_of( "P" ) == 3 ) ) {
      return( 1 );
    }
    if( ( orientation.find_first_of( "A" ) == 6 ) || ( orientation.find_first_of( "P" ) == 6 ) ) {
      return( 2 );
    }
    std::string msg( BIAL_ERROR( "Invalid orientation string." ) );
    throw( std::logic_error( msg ) );
  }

  size_t NiftiHeader::SagittalDimension( const std::string &orientation ) {
    COMMENT( "First dimension.", 2 );
    if( ( orientation.find_first_of( "R" ) == 0 ) || ( orientation.find_first_of( "L" ) == 0 ) ) {
      return( 0 );
    }
    COMMENT( "Second dimension.", 2 );
    if( ( orientation.find_first_of( "R" ) == 3 ) || ( orientation.find_first_of( "L" ) == 3 ) ) {
      return( 1 );
    }
    if( ( orientation.find_first_of( "R" ) == 6 ) || ( orientation.find_first_of( "L" ) == 6 ) ) {
      return( 2 );
    }
    std::string msg( BIAL_ERROR( "Invalid orientation string." ) );
    throw( std::logic_error( msg ) );
  }

  bool NiftiHeader::SuperiorInferior( const std::string &orientation ) {
    COMMENT( "First dimension.", 2 );
    if( ( orientation.find_first_of( "S" ) == 0 ) ||
        ( orientation.find_first_of( "S" ) == 3 ) ||
        ( orientation.find_first_of( "S" ) == 6 ) ) {
      return( true );
    }
    COMMENT( "Second dimension.", 2 );
    if( ( orientation.find_first_of( "I" ) == 0 ) ||
        ( orientation.find_first_of( "I" ) == 3 ) ||
        ( orientation.find_first_of( "I" ) == 6 ) ) {
      return( false );
    }
    std::string msg( BIAL_ERROR( "Invalid orientation string." ) );
    throw( std::logic_error( msg ) );
  }

  bool NiftiHeader::AnteriorPosterior( const std::string &orientation ) {
    COMMENT( "First dimension.", 2 );
    if( ( orientation.find_first_of( "A" ) == 0 ) ||
        ( orientation.find_first_of( "A" ) == 3 ) ||
        ( orientation.find_first_of( "A" ) == 6 ) ) {
      return( true );
    }
    COMMENT( "Second dimension.", 2 );
    if( ( orientation.find_first_of( "P" ) == 0 ) ||
        ( orientation.find_first_of( "P" ) == 3 ) ||
        ( orientation.find_first_of( "P" ) == 6 ) ) {
      return( false );
    }
    std::string msg( BIAL_ERROR( "Invalid orientation string." ) );
    throw( std::logic_error( msg ) );
  }

  void NiftiHeader::Swap2Bytes( size_t size, void *data ) {
    try {
      unsigned char *base_element = reinterpret_cast< unsigned char* >( data );
      unsigned char *next_element;
      for( size_t ii = 0; ii < size; ++ii ) {
        next_element = base_element + 1;
        std::swap( *base_element, *next_element );
        base_element += 2;
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
  }

  void NiftiHeader::Swap4Bytes( size_t size, void *data ) {
    try {
      unsigned char *base_element = reinterpret_cast< unsigned char* >( data );
      unsigned char *low_element;
      unsigned char *high_element;
      for( size_t ii = 0; ii < size; ++ii ) {
        low_element = base_element;
        high_element = base_element + 3;
        std::swap( *low_element, *high_element );
        low_element++;
        high_element--;
        std::swap( *low_element, *high_element );
        base_element += 4;
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
  }

  void NiftiHeader::Swap8Bytes( size_t size, void *data ) {
    try {
      unsigned char *base_element = reinterpret_cast< unsigned char* >( data );
      unsigned char *low_element;
      unsigned char *high_element;
      for( size_t ii = 0; ii < size; ++ii ) {
        low_element = base_element;
        high_element = base_element + 7;
        while( high_element > low_element ) {
          std::swap( *low_element, *high_element );
          low_element++;
          high_element--;
        }
        base_element += 8;
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
  }

  void NiftiHeader::Swap16Bytes( size_t size, void *data ) {
    try {
      unsigned char *base_element = reinterpret_cast< unsigned char* >( data );
      unsigned char *low_element;
      unsigned char *high_element;
      for( size_t ii = 0; ii < size; ii++ ) {
        low_element = base_element;
        high_element = base_element + 15;
        while( high_element > low_element ) {
          std::swap( *low_element, *high_element );
          low_element++;
          high_element--;
        }
        base_element += 16;
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
  }

  void NiftiHeader::SwapHeader( ) {
    try {
      COMMENT( "Header must be swapped.", 0 );
      NiftiHeader::Swap4Bytes( 1, reinterpret_cast< void* >( &extents ) );
      NiftiHeader::Swap2Bytes( 1, reinterpret_cast< void* >( &session_error ) );
      NiftiHeader::Swap4Bytes( 3, reinterpret_cast< void* >( &intent_p1 ) ); /* intent_p1, intent_p2, intent_p3 */
      COMMENT( "intent_code, datatype, bitpix, slice_start.", 2 );
      NiftiHeader::Swap2Bytes( 4, reinterpret_cast< void* >( &intent_code ) );
      NiftiHeader::Swap4Bytes( 3, reinterpret_cast< void* >( &vox_offset ) ); /* vox_offset, scl_slope, slc_inter */
      NiftiHeader::Swap2Bytes( 1, reinterpret_cast< void* >( &slice_end ) );
      COMMENT( "cal_max, cal_min, slice_duration, toffset, glmax, glmin.", 2 );
      NiftiHeader::Swap4Bytes( 6, reinterpret_cast< void* >( &cal_max ) );
      NiftiHeader::Swap2Bytes( 2, reinterpret_cast< void* >( &qform_code ) ); /* qform_code, sform_code */
      COMMENT( "quatern_b, quatern_c, quatern_d, qoffset_x, qoffset_y, qoffset_z.", 2 );
      NiftiHeader::Swap4Bytes( 6, reinterpret_cast< void* >( &quatern_b ) );
      NiftiHeader::Swap4Bytes( 12, reinterpret_cast< void* >( &stm[ 0 ] ) ); /* stm[ 0 ] to stm[ 11 ] */
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

  bool NiftiHeader::IsSwapped( const std::string filename ) {
    try {
      std::string hdrname( NiftiHeader::ExistingHeaderFileName( filename ) );
      IFile file( hdrname );
      int hdr_size;
      file.read( reinterpret_cast< char* >( &hdr_size ), sizeof( int ) ); /* hdr_size */
      file.close( );
      if( hdr_size != NIFTI_HEADER_SIZE ) {
        NiftiHeader::Swap4Bytes( 1, reinterpret_cast< void* >( &hdr_size ) );
        if( hdr_size == NIFTI_HEADER_SIZE ) {
          return( true );
        }
        else {
          std::string msg( BIAL_ERROR( "Corrupted nifti header." ) );
          throw( std::logic_error( msg ) );
        }
      }
      return( false );
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
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  void NiftiHeader::SwapNBytes( size_t size, size_t bytes, void *data ) {
    try {
      switch( bytes ) {
      case 2: {
        NiftiHeader::Swap2Bytes( size, data );
        break;
      }
      case 4: {
        NiftiHeader::Swap4Bytes( size, data );
        break;
      }
      case 8: {
        NiftiHeader::Swap8Bytes( size, data );
        break;
      }
      case 16: {
        NiftiHeader::Swap16Bytes( size, data );
        break;
      }
      default: {
        COMMENT( "nifti_swap_bytes( size , bytes, data )", 2 );
        std::string msg( BIAL_ERROR( "Invalid number of bytes to be swapped." ) );
        throw( std::logic_error( msg ) );
        break;
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
  }

  void NiftiHeader::DataType( NiftiType type ) {
    try {
      datatype = type;
      SetBitPix( );
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

  NiftiType NiftiHeader::DataType( const std::string &filename ) {
    try {
      NiftiHeader hdr( filename );
      return( hdr.datatype );
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
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  void NiftiHeader::QuaternToMat( const Vector< float > &pixel_dim ) {
    try {
      double b = quatern_b;
      double c = quatern_c;
      double d = quatern_d;
      COMMENT( "last row is always [ 0 0 0 1 ]", 2 );
      qtm( 3, 0 ) = qtm( 3, 1 ) = qtm( 3, 2 ) = 0.0f;
      qtm( 3, 3 ) = 1.0f;
      COMMENT( "Compute a parameter from b,c,d.", 2 );
      double a = 1.0l - ( b * b + c * c + d * d );
      if( a < 1.e-7l ) {
        COMMENT( "special case.", 2 );
        a = 1.0l / sqrt( b * b + c * c + d * d );
        b *= a; /* normalize (b,c,d) Vector */
        c *= a;
        d *= a;
        a = 0.0l; /* a = 0 ==> 180 degree rotation */
      }
      else {
        a = sqrt( a ); /* angle = 2*arccos(a) */
      }
      COMMENT( "Load rotation matrix, including scaling factors for voxel sizes.", 2 );
      /*
       * These are relative to the native space. It is not pixdim, as
       * pixdim refers to the image space is it? Maybe in this function
       * is, but not in MatToQuatern.
       */
      double xd = ( pixel_dim[ 0 ] > 0.0f ) ? pixel_dim[ 0 ] : 1.0; /* make sure are positive */
      double yd = ( pixel_dim[ 1 ] > 0.0f ) ? pixel_dim[ 1 ] : 1.0; /* make sure are positive */
      double zd = ( pixel_dim[ 2 ] > 0.0f ) ? pixel_dim[ 2 ] : 1.0; /* make sure are positive */
      if( qfac < 0.0 ) {
        zd = -zd; /* left handedness? */
      }
      qtm( 0, 0 ) = ( a * a + b * b - c * c - d * d ) * xd;
      qtm( 0, 1 ) = 2.0l * ( b * c - a * d ) * yd;
      qtm( 0, 2 ) = 2.0l * ( b * d + a * c ) * zd;
      qtm( 1, 0 ) = 2.0l * ( b * c + a * d ) * xd;
      qtm( 1, 1 ) = ( a * a + c * c - b * b - d * d ) * yd;
      qtm( 1, 2 ) = 2.0l * ( c * d - a * b ) * zd;
      qtm( 2, 0 ) = 2.0l * ( b * d - a * c ) * xd;
      qtm( 2, 1 ) = 2.0l * ( c * d + a * b ) * yd;
      qtm( 2, 2 ) = ( a * a + d * d - c * c - b * b ) * zd;
      qtm( 0, 3 ) = qoffset_x; /* load offsets */
      qtm( 1, 3 ) = qoffset_y; /* load offsets */
      qtm( 2, 3 ) = qoffset_z; /* load offsets */
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

  void NiftiHeader::MatToQuatern( ) {
    try {
      Matrix< double > Q( 3, 3 );
      COMMENT( "offset outputs are read write out of input matrix.", 2 );
      qoffset_x = qtm( 0, 3 );
      qoffset_y = qtm( 1, 3 );
      qoffset_z = qtm( 2, 3 );
      COMMENT( "load 3x3 matrix into local variables.", 2 );
      double r11 = qtm( 0, 0 );
      double r12 = qtm( 0, 1 );
      double r13 = qtm( 0, 2 );
      double r21 = qtm( 1, 0 );
      double r22 = qtm( 1, 1 );
      double r23 = qtm( 1, 2 );
      double r31 = qtm( 2, 0 );
      double r32 = qtm( 2, 1 );
      double r33 = qtm( 2, 2 );
      COMMENT( "compute lengths of each column; these determine grid spacings.", 2 );
      double xd = sqrt( r11 * r11 + r21 * r21 + r31 * r31 );
      double yd = sqrt( r12 * r12 + r22 * r22 + r32 * r32 );
      double zd = sqrt( r13 * r13 + r23 * r23 + r33 * r33 );
      COMMENT( "if a column length is zero, patch the trouble.", 2 );
      if( xd == 0.0 ) {
        r11 = 1.0;
        r21 = r31 = 0.0;
        xd = 1.0;
      }
      if( yd == 0.0 ) {
        r22 = 1.0;
        r12 = r32 = 0.0;
        yd = 1.0;
      }
      if( zd == 0.0 ) {
        r33 = 1.0;
        r13 = r23 = 0.0;
        zd = 1.0;
      }
      COMMENT( "These are relative to the native space. It is not pixdim, as pixdim refers to the image space.", 2 );
      /*
       * pixdim[ 0 ] = xd;
       * pixdim[ 1 ] = yd;
       * pixdim[ 2 ] = zd;
       */
      COMMENT( "normalize the columns.", 2 );
      r11 /= xd; r21 /= xd; r31 /= xd;
      r12 /= yd; r22 /= yd; r32 /= yd;
      r13 /= zd; r23 /= zd; r33 /= zd;
      /* At this point, the matrix has normal columns, but we have to allow
       * for the fact that the hideous user may not have given us a matrix
       * with orthogonal columns.
       * So, now find the orthogonal matrix closest to the current matrix.
       * One reason for using the polar decomposition to get this
       * orthogonal matrix, rather than just directly orthogonalizing
       * the columns, is so that inputting the inverse matrix to R
       * will result in the inverse orthogonal matrix at this point.
       * If we just orthogonalized the columns, this wouldn't necessarily hold.
       */
      Q( 0, 0 ) = r11; Q( 0, 1 ) = r12; Q( 0, 2 ) = r13;
      Q( 1, 0 ) = r21; Q( 1, 1 ) = r22; Q( 1, 2 ) = r23;
      Q( 2, 0 ) = r31; Q( 2, 1 ) = r32; Q( 2, 2 ) = r33;

      /* P is orthog matrix closest to Q */
      Matrix< double > P( MatrixOp::Polar( const_cast< const Matrix< double > & >( Q ) ) );
      r11 = P( 0, 0 ); r12 = P( 0, 1 ); r13 = P( 0, 2 ); /* unload */
      r21 = P( 1, 0 ); r22 = P( 1, 1 ); r23 = P( 1, 2 );
      r31 = P( 2, 0 ); r32 = P( 2, 1 ); r33 = P( 2, 2 );
      /* @param                [ r11 r12 r13 ]
       * at this point, the matrix  [ r21 r22 r23 ] is orthogonal
       * @param                [ r31 r32 r33 ]
       */
      COMMENT( "Compute the determinant to determine if it is proper.", 2 );
      zd = MatrixOp::Determinant( P ); /* should be -1 or 1 */
      if( zd > 0 ) { /* proper */
        qfac = 1.0f;
      }
      else {                  /* improper ==> flip 3rd column */
        qfac = -1.0f;
        r13 = -r13; r23 = -r23; r33 = -r33;
      }
      COMMENT( "Compute quaternion parameters.", 2 );
      double a = r11 + r22 + r33 + 1.0;
      double b, c, d;
      if( a > 0.5 ) { /* simplest case */
        a = 0.5 * sqrt( a );
        b = 0.25 * ( r32 - r23 ) / a;
        c = 0.25 * ( r13 - r31 ) / a;
        d = 0.25 * ( r21 - r12 ) / a;
      }
      else {                        /* trickier case */
        xd = 1.0 + r11 - ( r22 + r33 ); /* 4*b*b */
        yd = 1.0 + r22 - ( r11 + r33 ); /* 4*c*c */
        zd = 1.0 + r33 - ( r11 + r22 ); /* 4*d*d */
        if( xd > 1.0 ) {
          b = 0.5 * sqrt( xd );
          c = 0.25 * ( r12 + r21 ) / b;
          d = 0.25 * ( r13 + r31 ) / b;
          a = 0.25 * ( r32 - r23 ) / b;
        }
        else if( yd > 1.0 ) {
          c = 0.5 * sqrt( yd );
          b = 0.25 * ( r12 + r21 ) / c;
          d = 0.25 * ( r23 + r32 ) / c;
          a = 0.25 * ( r13 - r31 ) / c;
        }
        else {
          d = 0.5 * sqrt( zd );
          b = 0.25 * ( r13 + r31 ) / d;
          c = 0.25 * ( r23 + r32 ) / d;
          a = 0.25 * ( r21 - r12 ) / d;
        }
        if( a < 0.0 ) {
          b = -b;
          c = -c;
          d = -d;
          a = -a;
        }
      }
      quatern_b = b;
      quatern_c = c;
      quatern_d = d;
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

  std::string NiftiHeader::Orientation( size_t row ) const {
    try {
      if( ( qform_code <= NiftiXForm::UNKNOWN ) && ( sform_code <= NiftiXForm::UNKNOWN ) ) {
        return( "Undefined" );
      }
      const Matrix< float > &mat = qform_code > NiftiXForm::UNKNOWN ? qtm : stm;
      if( ( std::abs( mat( 0, row ) ) > std::abs( mat( 1, row ) ) ) &&
          ( std::abs( mat( 0, row ) ) > std::abs( mat( 2, row ) ) ) ) {
        if( mat( 0, row ) > 0 ) {
          return( "LR" );
        }
        else {
          return( "RL" );
        }
      }
      else if( std::abs( mat( 1, row ) ) > std::abs( mat( 2, row ) ) ) {
        if( mat( 1, row ) > 0 ) {
          return( "PA" );
        }
        else {
          return( "AP" );
        }
      }
      else {
        if( mat( 2, row ) > 0 ) {
          return( "IS" );
        }
        else {
          return( "SI" );
        }
      }
    }
    /*
     *   else if( sform_code > NiftiXForm::UNKNOWN ) {
     *     if( ( std::abs( stm( 0, row ) ) > std::abs( stm( 1, row ) ) ) &&
     *       ( std::abs( stm( 0, row ) ) > std::abs( stm( 2, row ) ) ) ) {
     *    if( stm( 0, row ) > 0 )
     *      return( "LR" );
     *    else
     *      return( "RL" );
     *     }
     *     else if( ( std::abs( stm( 1, row ) ) > std::abs( stm( 0, row ) ) ) &&
     *            ( std::abs( stm( 1, row ) ) > std::abs( stm( 2, row ) ) ) ) {
     *    if( stm( 1, row ) > 0 )
     *      return( "PA" );
     *    else
     *      return( "AP" );
     *     }
     *     else if( ( std::abs( stm( 2, row ) ) > std::abs( stm( 0, row ) ) ) &&
     *            ( std::abs( stm( 2, row ) ) > std::abs( stm( 1, row ) ) ) ) {
     *    if( stm( 2, row ) > 0 )
     *      return( "IS" );
     *    else
     *      return( "SI" );
     *     }
     *   }
     * }
     */
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

  std::string NiftiHeader::Orientation( ) const {
    try {
      return( Orientation( 0 ) + " " + Orientation( 1 ) + " " + Orientation( 2 ) );
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

  float NiftiHeader::SclSlope( ) const {
    return( scl_slope );
  }

  float NiftiHeader::SclInter( ) const {
    return( scl_inter );
  }

  bool NiftiHeader::UseQuatern( ) const {
    if( qform_code > NiftiXForm::UNKNOWN ) {
      return( true );
    }
    return( false );
  }

  bool NiftiHeader::UseAffine( ) const {
    if( sform_code > NiftiXForm::UNKNOWN ) {
      return( true );
    }
    return( false );
  }

  void NiftiHeader::QuaternMatrix( const Matrix< float > mat ) {
    try {
      qtm = mat;
      qform_code = NiftiXForm::SCANNER_ANAT;
      MatToQuatern( );
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

  void NiftiHeader::QuaternOffset( float x, float y, float z ) {
    qoffset_x = x;
    qoffset_y = y;
    qoffset_z = z;
  }

  Matrix< float > NiftiHeader::QuaternMatrix( ) const {
    return( qtm );
  }

  void NiftiHeader::AffineMatrix( const Matrix< float > mat ) {
    stm = mat;
    sform_code = NiftiXForm::SCANNER_ANAT;
  }

  Matrix< float > NiftiHeader::AffineMatrix( ) const {
    return( stm );
  }

  void NiftiHeader::DataTypeSizes( int datatype, size_t &nbyper, size_t &swapsize ) {
    switch( datatype ) {
    case static_cast< int >( NiftiType::INT8 ):
    case static_cast< int >( NiftiType::UINT8 ): {
      nbyper = 1;
      swapsize = 0;
      break;
    }
    case static_cast< int >( NiftiType::INT16 ):
    case static_cast< int >( NiftiType::UINT16 ): {
      nbyper = 2;
      swapsize = 2;
      break;
    }
    case static_cast< int >( NiftiType::INT32 ):
    case static_cast< int >( NiftiType::UINT32 ):
    case static_cast< int >( NiftiType::FLOAT32 ): {
      nbyper = 4;
      swapsize = 4;
      break;
    }
    case static_cast< int >( NiftiType::FLOAT64 ):
    case static_cast< int >( NiftiType::INT64 ):
    case static_cast< int >( NiftiType::UINT64 ): {
      nbyper = 8;
      swapsize = 8;
      break;
    }
    default: {
      std::string msg( BIAL_ERROR( "Data type not supported." ) );
      throw( std::logic_error( msg ) );
      break;
    }
      /*
       * case NiftiType::FLOAT128:   nbyper = 16; swapsize = 16; break;
       * case NiftiType::COMPLEX128: nbyper = 16; swapsize = 8;  break;
       * case NiftiType::COMPLEX256: nbyper = 32; swapsize = 16; break;
       * case NiftiType::RGB24:      nbyper = 3;  swapsize = 0;  break;
       * case NiftiType::RGBA32:     nbyper = 4;  swapsize = 0;  break;
       * case NiftiType::COMPLEX64:  nbyper = 8;  swapsize = 4;  break;
       */
    }
  }

  template< class O >
  O &NiftiHeader::PrintOrientation( O &os ) const {
    try {
      os << "qtm: " << std::endl;
      qtm.Print( os );
      os << std::endl << "stm: " << std::endl;
      qtm.Print( os );
      return( os );
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

  template< class O >
  O &NiftiHeader::Print( O &os ) const {
    try {
      os << "data_type: " << data_type.c_str( ) << "." << std::endl;
      os << "db_name: " << db_name.c_str( ) << "." << std::endl;
      os << "extents: " << extents << "." << std::endl;
      os << "session_error: " << session_error << "." << std::endl;
      os << "regular: " << regular << "." << std::endl;
      os << "freq_dim: " << freq_dim << "." << std::endl;
      os << "phase_dim: " << phase_dim << "." << std::endl;
      os << "slice_dim: " << slice_dim << "." << std::endl;
      for( size_t idx = 0; idx < dim.size( ); ++idx )
        os << "dim[ " << idx << " ]: " << dim[ idx ] << "." << std::endl;
      os << "intent_p1: " << intent_p1 << "." << std::endl;
      os << "intent_p2: " << intent_p2 << "." << std::endl;
      os << "intent_p3: " << intent_p3 << "." << std::endl;
      os << "intent_code: " << static_cast< int >( intent_code ) << "." << std::endl;
      os << "datatype: " << static_cast< int >( datatype ) << "." << std::endl;
      os << "bitpix: " << bitpix << "." << std::endl;
      os << "slice_start: " << slice_start << "." << std::endl;
      for( size_t idx = 0; idx < pixdim.size( ); ++idx )
        os << "pixdim[ " << idx << " ]: " << pixdim[ idx ] << "." << std::endl;
      os << "vox_offset: " << vox_offset << "." << std::endl;
      os << "scl_slope: " << scl_slope << "." << std::endl;
      os << "scl_inter: " << scl_inter << "." << std::endl;
      os << "slice_end: " << slice_end << "." << std::endl;
      os << "slice_code: " << static_cast< int >( slice_code ) << "." << std::endl;
      os << "xyz_units: " << static_cast< int >( xyz_units ) << "." << std::endl;
      os << "time_units: " << static_cast< int >( time_units ) << "." << std::endl;
      os << "cal_max: " << cal_max << "." << std::endl;
      os << "cal_min: " << cal_min << "." << std::endl;
      os << "slice_duration: " << slice_duration << "." << std::endl;
      os << "toffset: " << toffset << "." << std::endl;
      os << "glmax: " << glmax << "." << std::endl;
      os << "glmin: " << glmin << "." << std::endl;
      os << "descrip: " << descrip.c_str( ) << "." << std::endl;
      os << "aux_file: " << aux_file.c_str( ) << "." << std::endl;
      os << "qform_code: " << static_cast< int >( qform_code ) << "." << std::endl;
      os << "sform_code: " << static_cast< int >( sform_code ) << "." << std::endl;
      os << "quatern_b: " << quatern_b << "." << std::endl;
      os << "quatern_c: " << quatern_c << "." << std::endl;
      os << "quatern_d: " << quatern_d << "." << std::endl;
      os << "qoffset_x: " << qoffset_x << "." << std::endl;
      os << "qoffset_y: " << qoffset_y << "." << std::endl;
      os << "qoffset_z: " << qoffset_z << "." << std::endl;
      os << "qfac: " << qfac << "." << std::endl;
      PrintOrientation( os );
      os << std::endl << "intent_name: " << intent_name.c_str( ) << "." << std::endl;
      return( os );
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

  template NiftiHeader::NiftiHeader( const Image< int > &img, bool one_file );
  template NiftiHeader::NiftiHeader( const Image< llint > &img, bool one_file );
  template NiftiHeader::NiftiHeader( const Image< float > &img, bool one_file );
  template NiftiHeader::NiftiHeader( const Image< double > &img, bool one_file );
  template NiftiHeader::NiftiHeader( const Image< Color > &img, bool one_file );

  template std::ostream &NiftiHeader::PrintOrientation( std::ostream &os ) const;
  template std::ostream &NiftiHeader::Print( std::ostream &os ) const;
  template OFile &NiftiHeader::PrintOrientation( OFile &os ) const;
  template OFile &NiftiHeader::Print( OFile &os ) const;

}

#endif
