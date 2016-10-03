#ifndef FUNCTIONALWIDGET_H
#define FUNCTIONALWIDGET_H

#include <QWidget>

namespace Ui {
  class FunctionalWidget;
}

class FunctionalWidget : public QWidget {
  Q_OBJECT

public:
  explicit FunctionalWidget( QWidget *parent = 0 );
  ~FunctionalWidget( );

signals:
  void invertColors( );
  void brightness( );

private slots:
  void on_pushButtonInvertColors_clicked( );

  void on_pushButtonInvertColors_2_clicked( );

private:
  Ui::FunctionalWidget *ui;
};

#endif /* FUNCTIONALWIDGET_H */
