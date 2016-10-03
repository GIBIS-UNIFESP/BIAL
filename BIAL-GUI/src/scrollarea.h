#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QObject>
#include <QScrollArea>

class ScrollArea : public QScrollArea {
  Q_OBJECT

public:
  explicit ScrollArea( QWidget *parent = 0 );

signals:
  void item( const QString &text );

protected:
  virtual void dragEnterEvent( QDragEnterEvent *event ) override;
  virtual void dragMoveEvent( QDragMoveEvent *event ) override;
  virtual void dragLeaveEvent( QDragLeaveEvent *event ) override;
  virtual void dropEvent( QDropEvent *event ) override;
};

#endif /* SCROLLAREA_H */
