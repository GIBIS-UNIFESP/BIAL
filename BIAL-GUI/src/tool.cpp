#include "tool.h"

bool Tool::hasLabel( ) const {
  return( m_hasLabel );
}

void Tool::setHasLabel( bool sHasLabel ) {
  m_hasLabel = sHasLabel;
}

Tool::Tool( GuiImage *guiImage, ImageViewer *viewer ) try : QObject( guiImage ), guiImage( guiImage ), viewer( viewer ), m_visible(false) {
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

void Tool::setVisible( bool value ) {
  m_visible = value;
}

QPixmap Tool::getLabel( size_t axis ) {
  Q_UNUSED( axis );
  return( QPixmap( ) );
}

bool Tool::visible( ) const {
  return( m_visible );
}
