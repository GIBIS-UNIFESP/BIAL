#include "tool.h"

bool Tool::hasLabel( ) const {
  return( m_hasLabel );
}

void Tool::setHasLabel( bool sHasLabel ) {
  m_hasLabel = sHasLabel;
}

Tool::Tool( GuiImage *guiImage, ImageViewer *viewer ) : QObject( guiImage ), guiImage( guiImage ), viewer( viewer ), m_visible(false) {
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
