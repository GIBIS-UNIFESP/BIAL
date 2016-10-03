#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class Label : public QLabel {
  Q_OBJECT

public:
  explicit Label( QString filePath, QWidget *parent = 0 );
  QString filePath;
  QPixmap m_pixmap;

signals:
  void changeImage( const QString & );
  void Image( const QPixmap &, const QString & );

protected:
  void mousePressEvent( QMouseEvent* );
};

#endif /* LABEL_H */
