#include "livewirewidget.h"
#include "ui_livewirewidget.h"

LiveWireWidget::LiveWireWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LiveWireWidget)
{
  ui->setupUi(this);
}

LiveWireWidget::~LiveWireWidget()
{
  delete ui;
}
