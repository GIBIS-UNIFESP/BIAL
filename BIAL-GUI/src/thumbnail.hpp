#ifndef LABEL_HPP
#define LABEL_HPP

#include "guiimage.h"

#include <QFrame>

class Thumbnail : public QFrame {
  Q_OBJECT
public:
  explicit Thumbnail( GuiImage *image, int number, int size, QWidget *parent = 0 );

  int imageNumber( ) const;
  void setImageNumber( int imageNumber );

signals:
  void changeImage( int index );

public slots:

protected:
  void mousePressEvent( QMouseEvent* );
  int m_imageNumber;
};

#endif /* LABEL_HPP */
