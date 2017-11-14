/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2012/Jun/21 */
/* Content: Dicom header reader */
/* Description: Reads Dicom file and header containts. Based on Fabio Kawaoka Takase implementation. */
/* Future add-on's: - Translate all comments to English. Reformat code to Bial standard. */
/*                  - Implement data writting. */
/*                  - Implement Nifti Header convertion. */

/*****************************************************************************
*  File Name        :  TDicom.h
*  1st Version date :  Aug 2007
*  Author           :  Fabio Kawaoka Takase
*  ----------------------------------------------------------------------------
*  Description :
*  ^^^^^^^^^^^
*  TDicom class definition. This class extends the TDynMatrix class, providing
*  the support to open DICOM files.
*  ----------------------------------------------------------------------------
*  Updates:
*  ^^^^^^^^
*  2008-03-13:    Adicionadas classes TElementId e TGroupId
*  ----------------------------------------------------------------------------
*****************************************************************************/

#ifndef BIALDICOMHEADER_C
#define BIALDICOMHEADER_C

#include "DicomHeader.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_DicomHeader )
#define BIAL_EXPLICIT_DicomHeader
#endif

#if defined ( BIAL_EXPLICIT_DicomHeader ) || ( BIAL_IMPLICIT_BIN )

#include "Color.hpp"

namespace Bial {

  DicomHeader::DicomHeader( ) : lista( std::deque< TGroupId >( ) ) {
  }

  void DicomHeader::push( TGroupId group ) {
    lista.push_back( group );
  }

  void DicomHeader::readZeroPadding( ) {
    try {
      COMMENT( "Reading zero padding.", 4 );
      for( size_t i = 0; i < 128; ++i ) {
        file_pointer.ignore( );
      }
    }
    catch( std::ios_base::failure e ) {
      std::string msg = e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error reading Dicom file." );
      throw( std::ios_base::failure( msg ) );
    }
  }

  void DicomHeader::readDICM( ) {
    try {
      COMMENT( "Read DICM.", 4 );
      char c[ 5 ];
      for( size_t i = 0; i < 4; i++ ) {
        file_pointer >> c[ i ];
      }
      c[ 4 ] = '\0';
      std::string str( "DICM" );
      if( str.compare( c ) != 0 ) {
        std::string msg( BIAL_ERROR( "Corrupted Dicom image. Magic string is incorrect." ) );
        throw( std::logic_error( msg ) );
      }
    }
    catch( std::ios_base::failure e ) {
      std::string msg = e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Error reading Dicom file." );
      throw( std::ios_base::failure( msg ) );
    }
  }

  template< class D >
  Image< D > DicomHeader::readData( ) {
    try {
      int ishort;
      int numberOfBytes;
      char c;

      int lastGroup = 0;
      TGroupId group;
      bool first = true;

      int blockCounter = 0;
      int blockSize = 100; /* still unknown */
      while( 1 ) {
        int igroup = readInt16( );
        int isub = readInt16( );
        std::string s = readID( );
        blockCounter += 6;
        std::string dataValue( "" );

        TElementId element;

        COMMENT( "Reading groups.", 4 );
        if( first ) {
          first = false;
          lastGroup = igroup;
          group = TGroupId( igroup );
        }
        if( lastGroup != igroup ) {
          push( group ); /* adiciona o Group a lista do DICOM */
          group = TGroupId( igroup ); /* cria novo group com novo Group ID */
          lastGroup = igroup; /* lastGroup deve ser atualizado */
        }
        COMMENT( "Reading groups.", 4 );
        if( first ) {
          first = false;
          lastGroup = igroup;
          group = TGroupId( igroup );
        }
        if( lastGroup != igroup ) {
          push( group ); /* adiciona o Group a lista do DICOM */
          group = TGroupId( igroup ); /* cria novo group com novo Group ID */
          lastGroup = igroup; /* lastGroup deve ser atualizado */
        }
        if( ( s.compare( "OB" ) == 0 ) || ( s.compare( "OW" ) == 0 ) ||
            ( s.compare( "SQ" ) == 0 ) || ( s.compare( "UN" ) == 0 ) ) {
          ishort = readInt16( ); /* read 2 0 padded bytes */
          numberOfBytes = readInt32( );
          blockCounter += 6;
        }
        else {
          ishort = readInt16( );
          blockCounter += 2;
          numberOfBytes = ishort;
        }
        if( s.compare( "UL" ) == 0 ) {
          blockSize = readInt32( );
          blockCounter = -4;
          element.set( isub, blockSize );
        }
        else if( s.compare( "OW" ) == 0 ) {
          return( readImageData< D >( ) );
        }
        else if( s.compare( "US" ) == 0 ) { /* unsigned short */
          ishort = readInt16( );
          element.set( isub, ishort );
        }
        else {
          for( int j = 0; j < numberOfBytes; ++j ) {
            file_pointer.get( c );
            dataValue += c;
          }
          element.set( isub, dataValue );
        }
        blockCounter = blockCounter + numberOfBytes;
        COMMENT( "block: " << blockCounter, 4 );
        /* element.set( isub, dataValue ); // acerta o valor de Element e seu ID */
        group.push( igroup, element ); /* adiciona ao Group */
      }
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  TElementId DicomHeader::getElement( int grupo, int elemento ) {
    iterator it = lista.begin( );
    for( ; it != lista.end( ); ++it ) {
      if( it->getId( ) == grupo ) {
        break;
      }
    }
    if( it == lista.end( ) ) {
      std::string msg( BIAL_ERROR( "Could not find required Dicom group." ) );
      throw( std::logic_error( msg ) );
    }
    TGroupId::iterator ig = it->begin( );
    for( ; ig != it->end( ); ig++ ) {
      if( ig->getId( ) == elemento ) {
        return( *ig );
      }
    }
    std::string msg( BIAL_ERROR( "Could not find required Dicom element." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D >
  Image< D > DicomHeader::readImageData( ) {
    try {
      TElementId row = getElement( 40, 16 );
      TElementId col = getElement( 40, 17 );
      if( ( row.getValueType( ) != 3 ) || ( col.getValueType( ) != 3 ) ) {
        throw( std::logic_error( "" ) );
      }
      Image< D > res( col.getIntValue( ), row.getIntValue( ) );
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        res[ pxl ] = static_cast< D >( readInt16( ) );
      }
      return( res );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  template< >
  Image< Color > DicomHeader::readImageData( ) {
    try {
      TElementId row = getElement( 40, 16 );
      TElementId col = getElement( 40, 17 );
      if( ( row.getValueType( ) != 3 ) || ( col.getValueType( ) != 3 ) ) {
        throw( std::logic_error( "" ) );
      }
      Image< Color > res( col.getIntValue( ), row.getIntValue( ) );
      for( size_t pxl = 0; pxl < res.size( ); ++pxl ) {
        res[ pxl ]( 1 ) = static_cast< uchar >( readInt16( ) );
        res[ pxl ]( 2 ) = res[ pxl ]( 1 );
        res[ pxl ]( 3 ) = res[ pxl ]( 1 );
      }
      return( res );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  int DicomHeader::readInt16( ) {
    try {
      unsigned char c;
      file_pointer.get( reinterpret_cast< char & >( c ) );
      int ishort = static_cast< int >( c );
      file_pointer.get( reinterpret_cast< char & >( c ) );
      ishort = ishort + ( static_cast< int >( c ) * 256 );
      return( ishort );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  long DicomHeader::readInt32( ) {
    try {
      long ilong = 0;
      unsigned char c;
      for( size_t i = 0; i < 4; ++i ) {
        file_pointer.get( reinterpret_cast< char & >( c ) );
        long ilaux = static_cast< int >( c );
        for( size_t j = 0; j < i; ++j ) {
          ilaux = ilaux * 256;
        }
        ilong = ilong + ilaux;
      }
      return( ilong );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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

  std::string DicomHeader::readID( ) { /* read VR 2 + 2 bytes, the second is 0x00 */
    try {
      std::string s;
      char c;
      file_pointer.get( c );
      s.append( 1, static_cast< unsigned char >( c ) );
      file_pointer.get( c );
      s.append( 1, static_cast< unsigned char >( c ) );
      return( s );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while reading file." ) );
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
  Image< D > DicomHeader::readFile( std::string filename ) {
    try {
      file_pointer.exceptions( std::fstream::failbit | std::fstream::badbit );
      file_pointer.open( filename );
      COMMENT( "Reading Header.", 4 );
      readZeroPadding( );
      readDICM( );
      COMMENT( "Reading Data.", 4 );
      Image< D > res( readData< D >( ) );
      file_pointer.close( );
      return( res );
    }
    catch( std::ios_base::failure &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "I/O error while opening/reading/closing file." ) );
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

#ifdef BIAL_EXPLICIT_DicomHeader

  template Image< int > DicomHeader::readImageData( );
  template Image< int > DicomHeader::readFile( std::string filename );

  template Image< llint > DicomHeader::readImageData( );
  template Image< llint > DicomHeader::readFile( std::string filename );

  template Image< float > DicomHeader::readImageData( );
  template Image< float > DicomHeader::readFile( std::string filename );

  template Image< double > DicomHeader::readImageData( );
  template Image< double > DicomHeader::readFile( std::string filename );

  template Image< Color > DicomHeader::readFile( std::string filename );

#endif

}


#endif

#endif
