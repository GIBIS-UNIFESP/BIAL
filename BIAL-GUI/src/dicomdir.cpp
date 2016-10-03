#include "dicomdir.h"
#ifdef LIBGDCM
#include <gdcmAttribute.h>
#include <gdcmReader.h>
#include <gdcmStreamImageReader.h>
#include <gdcmStringFilter.h>
#endif

/* ImageInfo */
QDir ImageInfo::getDir( ) const {
  return( dir );
}

void ImageInfo::setDir( const QDir &value ) {
  dir = value;
}

QString ImageInfo::getFileName( ) {
  QString imgpath = QDir::fromNativeSeparators( referencedFileID );
  for( int pos = 0; pos < imgpath.size( ); ++pos ) {
    if( ( ( imgpath[ pos ] == '\\' ) || ( imgpath[ pos ] == '/' ) ) && ( imgpath[ pos ] != DIR_SEPARATOR ) ) {
      imgpath[ pos ] = DIR_SEPARATOR;
    }
  }
  QStringList list = imgpath.split( DIR_SEPARATOR );
  QFileInfo info;
  for( QString str : list ) {
    info.setFile( dir.absoluteFilePath( str ) );
    for( int i = 0; i < 2 && !info.exists( ); ++i ) {
      if( i == 0 ) {
        str = str.toUpper( );
      }
      else {
        str = str.toLower( );
      }
      info.setFile( dir.absoluteFilePath( str ) );
    }
    dir.cd( str );
  }
  return( info.absoluteFilePath( ) );
}

ImageInfo::ImageInfo( ) {
}

QString ImageInfo::getReferencedFileID( ) const {
  return( referencedFileID );
}

void ImageInfo::setReferencedFileID( const QString &value ) {
  referencedFileID = value;
}

QString ImageInfo::getUid( ) const {
  return( uid );
}

void ImageInfo::setUid( const QString &value ) {
  uid = value;
}

QString Series::getUid( ) const {
  return( uid );
}

void Series::setUid( const QString &value ) {
  uid = value;
}

QString Series::getDescr( ) const {
  return( descr );
}

void Series::setDescr( const QString &value ) {
  descr = value;
}

QStringList Series::getImages( ) {
  QStringList files;
  for( auto itr = images.begin( ); itr != images.end( ); ++itr ) {
    files.push_back( itr->getFileName( ) );
  }
  return( files );
}

void Series::addImage( const ImageInfo &imageInfo ) {
  images.push_back( imageInfo );
}

QString Series::getModality( ) const {
  return( modality );
}

void Series::setModality( const QString &value ) {
  modality = value;
}
/* Study */
QString Study::getUid( ) const {
  return( uid );
}

void Study::setUid( const QString &value ) {
  uid = value;
}

QString Study::getDescr( ) const {
  return( descr );
}

void Study::setDescr( const QString &value ) {
  descr = value;
}

void Study::addSeries( const Series &s ) {
  series.push_back( s );
}

QString Study::getDate( ) const {
  return( date );
}

void Study::setDate( const QString &value ) {
  date = value;
}

QStringList Study::getImages( ) {
  QStringList files;
  for( auto itr = series.begin( ); itr != series.end( ); ++itr ) {
    QStringList sFiles = itr->getImages( );
    files << sFiles;
  }
  return( files );
}
/* Patient */
QString Patient::getName( ) const {
  return( name );
}

void Patient::setName( const QString &value ) {
  name = value;
}

QString Patient::getId( ) const {
  return( id );
}

void Patient::setId( const QString &value ) {
  id = value;
}

void Patient::addStudy( const Study &s ) {
  study.push_back( s );
}

QStringList Patient::getImages( ) {
  QStringList files;
  for( auto itr = study.begin( ); itr != study.end( ); ++itr ) {
    QStringList sFiles = itr->getImages( );
    files << sFiles;
  }
  return( files );
}
/* Dicomdir */
#ifdef LIBGDCM
QString str( const gdcm::Value &val ) {
  std::stringstream strm;
  strm << val;
  return( QString::fromStdString( strm.str( ) ) );
}

QString str( gdcm::SmartPointer< gdcm::Value > val ) {
  std::stringstream strm;
  strm << *val;
  return( QString::fromStdString( strm.str( ) ) );
}
#endif
DicomDir::DicomDir( ) {
}

bool DicomDir::open( const QString &filename ) {
  this->filename = filename;
  QFileInfo fileInfo( filename );
  title = fileInfo.baseName( );
  /* TODO Verify if file exists ( The function Bial::File::Exists do not exists yes ) */
  QDir dir = fileInfo.dir( );
#ifndef LIBGDCM
  BIAL_WARNING( "GDCM library not found and/or reprocessor variable LIBGDCM not set!" );
  return( false );
#else
  gdcm::Reader reader;
  COMMENT( "Opening dicomdir file: " << filename.toStdString( ), 1 );
  reader.SetFileName( filename.toUtf8( ) );
  if( !reader.Read( ) ) {
    BIAL_WARNING( "Cannot read file!" );
    return( false );
  }
  const gdcm::File &file = reader.GetFile( );
  const gdcm::FileMetaInformation header = file.GetHeader( );
  const gdcm::DataSet &ds = file.GetDataSet( );
  gdcm::MediaStorage ms;
  ms.SetFromFile( file );
  if( gdcm::MediaStorage::GetMSType( ms.GetString( ) ) != gdcm::MediaStorage::MediaStorageDirectoryStorage ) {
    BIAL_WARNING( "This file is not a DICOMDIR ( Media storage type: " << ms << " )" )
    return( false );
  }
  if( header.FindDataElement( gdcm::Tag( 0x0002, 0x0002 ) ) ) {
    if( str( header.GetDataElement( gdcm::Tag( 0x0002, 0x0002 ) ).GetValue( ) ) != "1.2.840.10008.1.3.10" ) {
      BIAL_WARNING( "This file is not a DICOMDIR" );
    }
  }
  else {
    BIAL_WARNING( "Media Storage SOP Class not present" );
    return( false );
  }
  for( auto it = ds.GetDES( ).begin( ); it != ds.GetDES( ).end( ); ++it ) {
    const gdcm::DataElement &de = *it;
    if( de.GetTag( ) == gdcm::Tag( 0x004, 0x1220 ) ) { /* verifies if it is a valid ... */
      gdcm::SmartPointer< gdcm::SequenceOfItems > sq = de.GetValueAsSQ( );
      COMMENT( "Iterating through DicomDir itens", 1 );
      for( size_t itemNr = 1; itemNr < sq->GetNumberOfItems( ); ++itemNr ) {
        gdcm::Item &item = sq->GetItem( itemNr );
        if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
          gdcm::SmartPointer< gdcm::Value > val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
          while( ( str( val ) == "PATIENT " ) || ( str( val ) == "PATIENT" ) ) {
            COMMENT( "Creating Patient", 2 );
            Patient p;
            if( item.FindDataElement( gdcm::Tag( 0x0010, 0x0010 ) ) ) {
              val = item.GetDataElement( gdcm::Tag( 0x0010, 0x0010 ) ).GetValue( );
              p.setName( str( val ) );
            }
            if( item.FindDataElement( gdcm::Tag( 0x0010, 0x0020 ) ) ) {
              val = item.GetDataElement( gdcm::Tag( 0x0010, 0x0020 ) ).GetValue( );
              p.setId( str( val ) );
            }
            /* Next */
            item = sq->GetItem( ++itemNr );
            if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
              val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
            }
            while( ( str( val ) == "STUDY " ) || ( str( val ) == "STUDY" ) ) {
              COMMENT( "Creating Study", 2 )
              Study study;
              /*              std::cout << str( val ) << std::endl; */
              if( item.FindDataElement( gdcm::Tag( 0x0020, 0x000d ) ) ) {
                val = item.GetDataElement( gdcm::Tag( 0x0020, 0x000d ) ).GetValue( );
                /*                std::cout << "  UID = " << *val << std::endl; */
                study.setUid( str( val ) );
              }
              if( item.FindDataElement( gdcm::Tag( 0x0008, 0x0020 ) ) ) {
                val = item.GetDataElement( gdcm::Tag( 0x0008, 0x0020 ) ).GetValue( );
                /*                std::cout << "  Date = " << *val << std::endl; */
                study.setDate( str( val ) );
              }
              if( item.FindDataElement( gdcm::Tag( 0x0008, 0x1030 ) ) ) {
                val = item.GetDataElement( gdcm::Tag( 0x0008, 0x1030 ) ).GetValue( );
                /*                std::cout << "  Description = " << *val << std::endl; */
                study.setDescr( str( val ) );
              }
              item = sq->GetItem( ++itemNr );
              if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
                val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
              }
              while( str( val ) == "SERIES " || str( val ) == "SERIES" ) {
                COMMENT( "Creating Series", 2 );
                Series series;
                /*                std::cout << "  " << str( val ) << std::endl; */
                if( item.FindDataElement( gdcm::Tag( 0x0020, 0x000e ) ) ) {
                  val = item.GetDataElement( gdcm::Tag( 0x0020, 0x000e ) ).GetValue( );
                  /*                  std::cout << "    UID = " << *val << std::endl; */
                  series.setUid( str( val ) );
                }
                if( item.FindDataElement( gdcm::Tag( 0x0008, 0x0060 ) ) ) {
                  val = item.GetDataElement( gdcm::Tag( 0x0008, 0x0060 ) ).GetValue( );
                  /*                  std::cout << "    Modality = " << *val << std::endl; */
                  series.setModality( str( val ) );
                }
                if( item.FindDataElement( gdcm::Tag( 0x0008, 0x103e ) ) ) {
                  val = item.GetDataElement( gdcm::Tag( 0x0008, 0x103e ) ).GetValue( );
                  /*                  std::cout << "    Description = " << *val << std::endl; */
                  series.setDescr( str( val ) );
                }
                item = sq->GetItem( ++itemNr );
                if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
                  val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
                }
                while( str( val ) == "IMAGE " || str( val ) == "IMAGE" ) {
                  COMMENT( "Creating ImageInfo", 2 );
                  ImageInfo imageInfo;
                  imageInfo.setDir( dir );
                  /*
                   *                  std::cout << "    " << item << std::endl;
                   *                  std::cout << "    " << *val << std::endl;
                   */
                  if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1511 ) ) ) {
                    val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1511 ) ).GetValue( );
                    /*                    std::cout << "      UID = " << *val << std::endl; */
                    imageInfo.setUid( str( val ) );
                  }
                  if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1500 ) ) ) {
                    val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1500 ) ).GetValue( );
                    /*                    std::cout << "      ReferencedFileID = " << *val << std::endl; */
                    imageInfo.setReferencedFileID( str( val ) );
                  }
                  if( itemNr < sq->GetNumberOfItems( ) ) {
                    item = sq->GetItem( ++itemNr );
                    if( item.FindDataElement( gdcm::Tag( 0x0004, 0x1430 ) ) ) {
                      val = item.GetDataElement( gdcm::Tag( 0x0004, 0x1430 ) ).GetValue( );
                    }
                  }
                  else {
                    break;
                  }
                  series.addImage( imageInfo );
                }
                study.addSeries( series );
              }
              p.addStudy( study );
            }
            patient.push_back( p );
          }
        }
      }
    }
  }
#endif
  return( true );
}

QStringList DicomDir::getImages( ) {
  QStringList files;
  for( auto itr = patient.begin( ); itr != patient.end( ); ++itr ) {
    QStringList pFiles = itr->getImages( );
    files << pFiles;
  }
  return( files );
}
