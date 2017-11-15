#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QObject>
#include <QPushButton>

class PushButton : public QPushButton {
  Q_OBJECT

public:
  explicit PushButton( QWidget *parent = 0 );
  ~PushButton( );

signals:
  void file( const QString &text );
  void pixmap( const QPixmap &pix );
  void item( const QString &text );

  /* QWidget interface */
protected:
  virtual void dropEvent( QDropEvent *event ) override;

  /* QWidget interface */
protected:
  virtual void dragEnterEvent( QDragEnterEvent *event ) override;
  virtual void dragMoveEvent( QDragMoveEvent *event ) override;
  virtual void dragLeaveEvent( QDragLeaveEvent *event ) override;
};

#endif /* PUSHBUTTON_H */
