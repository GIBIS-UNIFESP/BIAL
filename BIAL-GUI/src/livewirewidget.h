#ifndef LIVEWIREWIDGET_H
#define LIVEWIREWIDGET_H

#include "livewiretool.h"

#include <QWidget>

namespace Ui {
  class LiveWireWidget;
}

class LiveWireWidget : public QWidget {
  Q_OBJECT

public:
  explicit LiveWireWidget( QWidget *parent = 0 );
  ~LiveWireWidget( );
  void setTool( Tool *sTool );

private slots:

  void on_pushButtonClear_clicked( );

  void on_pushButtonRobotUser_clicked( );

private:
  Ui::LiveWireWidget *ui;
  LiveWireTool *tool;
};

#endif /* LIVEWIREWIDGET_H */
