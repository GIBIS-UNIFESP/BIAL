#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "viewerinterface.h"

#include <QWidget>

namespace Ui {
  class ImageWidget;
}

class GraphicsScene;
class QGraphicsView;

class ImageWidget : public QWidget, public ViewerInterface {
  Q_OBJECT

public:
  explicit ImageWidget( QWidget *parent = 0 );
  ~ImageWidget( );
  void hideControls( );
  void showControls( );
  virtual void setViewBgColor( const QColor &color );
  GraphicsScene* scene( ) const;
  QGraphicsView* graphicsView( );
  size_t viewNumber( ) const;
  void setViewNumber( const size_t &viewNumber );
  void setRange( int start, int end );
  void setSlice( int slice );
  void show( );

signals:
  void sliceChanged( size_t viewNbr, size_t slice );
  void rotate( size_t viewNbr );
  void fliph( size_t viewNbr );
  void flipv( size_t viewNbr );
  void dropImage( const QString &filePath );
  void dropFolder( const QString &folderPath );

private slots:
  void on_spinBox_valueChanged( int position );
  void on_rotateButton_clicked( );
  void on_horizontalSlider_valueChanged( int position );
  void on_flip_h_button_clicked( );
  void on_flip_v_button_clicked( );

private:
  Ui::ImageWidget *ui;
  GraphicsScene *m_scene;
  size_t m_viewNumber;
};

#endif /* IMAGEWIDGET_H */
