#ifndef LIVEWIREWIDGET_H
#define LIVEWIREWIDGET_H

#include <QWidget>

namespace Ui {
class LiveWireWidget;
}

class LiveWireWidget : public QWidget
{
  Q_OBJECT

public:
  explicit LiveWireWidget(QWidget *parent = 0);
  ~LiveWireWidget();

private:
  Ui::LiveWireWidget *ui;
};

#endif // LIVEWIREWIDGET_H
