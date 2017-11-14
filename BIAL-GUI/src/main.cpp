#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

int main( int argc, char *argv[] ) {
  QApplication app( argc, argv );
  app.setOrganizationName( "UNIFESP" );
  app.setApplicationName( "BIAL" );

  QCommandLineParser parser;
  parser.setApplicationDescription( "Biomedical Image Analysis Library" );
  parser.addHelpOption( );
  parser.addVersionOption( );
  parser.addPositionalArgument( "image", QCoreApplication::translate( "main", "Source image to be opened." ) );

  QCommandLineOption dicomdir( QStringList( ) << "dicomdir",
                               QCoreApplication::translate( "main", "DICOM dataset directory" ),
                               QCoreApplication::translate( "main", "dicomdir" ) );
  parser.addOption( dicomdir );

  QCommandLineOption folder( QStringList( ) << "f" << "dir" << "folder" << "directory",
                             QCoreApplication::translate( "main", "Input folder with images." ),
                             QCoreApplication::translate( "main", "folder" ) );
  parser.addOption( folder );

  QCommandLineOption label( QStringList( ) << "l" << "label",
                            QCoreApplication::translate( "main", "Input label." ),
                            QCoreApplication::translate( "main", "label" ) );

  parser.addOption( label );

  QCommandLineOption liveWire( "ac", QCoreApplication::translate( "main", "Open active contour tool." ) );
  parser.addOption( liveWire );

  parser.process( app );

  MainWindow window;

  window.commandLineOpen( parser, dicomdir, folder, label, liveWire );
  window.showMaximized( );
  return( app.exec( ) );
}
