#include <QFileInfo>

#include "label.h"

Label::Label( QString filePath, QWidget *parent ) : QLabel( parent ), filePath( filePath ), m_pixmap( QPixmap(
                                                                                                        filePath ) ) {
}

void Label::mousePressEvent( QMouseEvent* ) {
  QFileInfo info( filePath );

  emit Image( m_pixmap, info.fileName( ) );
}
