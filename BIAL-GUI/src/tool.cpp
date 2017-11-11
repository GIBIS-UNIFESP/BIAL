#include "activeContourTool.h"
#include "defaulttool.h"
#include "segmentationtool.h"
#include "tool.h"

bool Tool::hasLabel( ) const {
  return( m_hasLabel );
}

void Tool::setHasLabel( bool sHasLabel ) {
  m_hasLabel = sHasLabel;
}

GuiImage* Tool::getGuiImage( ) const {
  return( guiImage );
}

Tool* Tool::buildTool( int toolType, GuiImage *img, ImageViewer *viewer ) {
  switch( toolType ) {
      case DefaultTool::Type:
      return( new DefaultTool( img, viewer ) );
      case ActiveContourTool::Type:
      return( new ActiveContourTool( img, viewer ) );
      case SegmentationTool::Type:
      return( new SegmentationTool( img, viewer ) );
      default:
      throw std::logic_error( "Invalid tool" );
  }
}

Tool* Tool::setImageTool( int tool_type, GuiImage *img, ImageViewer *viewer ) {
  for( int toolPos = 0; toolPos < img->tools.size( ); ++toolPos ) {
    if( img->tools.at( toolPos )->type( ) == tool_type ) {
      img->setCurrentToolPos( toolPos );
    }
  }
  if( img->currentTool( )->type( ) != tool_type ) {
    img->tools.append( Tool::buildTool( tool_type, img, viewer ) );
    img->setCurrentToolPos( img->tools.size( ) - 1 );
  }
}


Tool::Tool( GuiImage *guiImage, ImageViewer *viewer ) try : QObject( guiImage ), guiImage( guiImage ), viewer( viewer ),
  m_visible( false ) {
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

void Tool::leave( ) {

}

void Tool::enter( ) {

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
