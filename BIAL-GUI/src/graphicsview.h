#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView {
  Q_OBJECT

public:
  explicit GraphicsView( QWidget *parent = 0 );

  bool getFit( ) const;
  void setFit( bool value );

signals:
  void dropImage( const QString &filePath );
  void dropFolder( const QString &folderPath );
  void saveFile( const QString &item );

  /* QWidget interface */
protected:
  virtual void dragEnterEvent( QDragEnterEvent *event ) override;
  virtual void dragMoveEvent( QDragMoveEvent *event ) override;
  virtual void dragLeaveEvent( QDragLeaveEvent *event ) override;
  virtual void dropEvent( QDropEvent *event ) override;

  /* QWidget interface */
protected:
  virtual void resizeEvent( QResizeEvent *event ) override;
  bool fit = false;
};

#endif /* GRAPHICSVIEW_H */
