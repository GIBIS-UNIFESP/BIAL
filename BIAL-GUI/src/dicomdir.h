/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @file Dicomdir.h
 * @author Lucas Santana Lellis
 * @date 2015/Jul/21
 * @version 2.0.00
 * @brief
 * Content: Dicomdir related classes
 * <br> Description: Dicomdir related classes and GDCM library integration.
 * <br> Future add-on's: unknown.
 */

#include "Common.hpp"

#include <QDir>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#ifndef DICOMDIR_H
#define DICOMDIR_H
class ImageInfo {
  QString uid;
  QString referencedFileID;
  QDir dir;

public:
  ImageInfo( );
  QString getUid( ) const;
  void setUid( const QString &value );
  QString getReferencedFileID( ) const;
  void setReferencedFileID( const QString &value );
  QDir getDir( ) const;
  void setDir( const QDir &value );
  QString getFileName( );
};
class Series {
  std::vector< ImageInfo > images;
  QString uid;
  QString modality;
  QString descr;

public:
  void addImage( const ImageInfo &imageInfo );
  QString getUid( ) const;
  void setUid( const QString &value );
  QString getModality( ) const;
  void setModality( const QString &value );
  QString getDescr( ) const;
  void setDescr( const QString &value );
  QStringList getImages( );
};
class Study {
  std::vector< Series > series;
  QString uid;
  QString date;
  QString descr;

public:
  void addSeries( const Series &s );
  QString getUid( ) const;
  void setUid( const QString &value );
  QString getDate( ) const;
  void setDate( const QString &value );
  QString getDescr( ) const;
  void setDescr( const QString &value );
  QStringList getImages( );
};
class Patient {
  std::vector< Study > study;
  QString name;
  QString id;

public:
  void addStudy( const Study &s );
  QString getName( ) const;
  void setName( const QString &value );
  QString getId( ) const;
  void setId( const QString &value );
  QStringList getImages( );
};
class DicomDir {
  std::vector< Patient > patient;
  QString filename;
  QString title;
  QString dir;

public:
  DicomDir( );
  bool open( const QString &filename );
  QString getTitle( ) const;
  void setTitle( const QString &value );
  QString getFilename( ) const;
  QStringList getImages( );
};

#endif /* DICOMDIR_H */
