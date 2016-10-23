#include <QDebug>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGridLayout>
#include <iostream>

#include "Common.hpp"
#include "controller.h"
#include "defaulttool.h"
#include "graphicsscene.h"
#include "imageviewer.h"
#include "imagewidget.h"

ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent) {
  m_controller = nullptr;
  for (size_t i = 0; i < views.size(); ++i) {
    views[i] = new ImageWidget(this);
    views[i]->hideControls();
    getScene(i)->installEventFilter(this);
    views[i]->setViewNumber(i);
    connect(views.at(i), &ImageWidget::dropImage, this, &ImageViewer::dropImage);
    connect(views.at(i), &ImageWidget::dropFolder, this, &ImageViewer::dropFolder);
  }
  layout = new QGridLayout(this);
  layout->setVerticalSpacing(0);
  layout->setHorizontalSpacing(0);
  layout->setMargin(0);
  setGridLayout();
  QPalette p(palette());
  p.setColor(QPalette::Background, Qt::black);
  setAutoFillBackground(true);
  setPalette(p);
  dragging = false;
}

ImageViewer::~ImageViewer() {}

void ImageViewer::setViewBgColor(const QColor &color) {
  for (ImageWidget *view : views) {
    view->setViewBgColor(color);
  }
}

void ImageViewer::setController(Controller *value) {
  m_controller = value;
  connect(m_controller, &Controller::currentImageChanged, this, &ImageViewer::changeImage);
  connect(m_controller, &Controller::imageUpdated, this, &ImageViewer::updateViews);
  /*  connect( this, &ImageViewer::mouseClicked, controller, &Controller::changeOthersSlices ); */
  for (ImageWidget *view : views) {
    connect(view, &ImageWidget::sliceChanged, m_controller, &Controller::setCurrentSlice);
    connect(view, &ImageWidget::sliceChanged, this, &ImageViewer::sliceChanged);
    connect(view, &ImageWidget::rotate, m_controller, &Controller::rotate90);
    connect(view, &ImageWidget::fliph, m_controller, &Controller::flipH);
    connect(view, &ImageWidget::flipv, m_controller, &Controller::flipV);
  }
  for (size_t axis = 0; axis < 4; ++axis) {
    getScene(axis)->addItem(m_controller->getPixmapItem(axis));
    getScene(axis)->addItem(m_controller->getLabelItem(axis));
  }
}

Controller *ImageViewer::controller() const { return (m_controller); }

void ImageViewer::updateViews() {
  COMMENT("ImageViewer::updateViews", 2);
  if (!m_controller) {
    return;
  }
  GuiImage *img = m_controller->currentImage();
  if (!img) {
    return;
  }
  for (size_t axis = 0; axis < 4; ++axis) {
    views[axis]->setSlice(img->currentSlice(axis));
    getScene(axis)->setOverlay(m_controller->currentFormat()->overlay());
  }
}

void ImageViewer::changeImage() {
  COMMENT("ImageViewer::changeImage", 2);
  std::cout  << "ChangeImage" << std::endl;
  GuiImage *img = m_controller->currentImage();
  if (img) {
    if (img->tools.empty()) {
      img->tools.append(new DefaultTool(img, this));
    }
    img->getIntersection(0);
    DisplayFormat *format = m_controller->currentFormat();
    for (size_t axis = 0; axis < 4; ++axis) {
      getScene(axis)->setOverlay(false);
      getScene(axis)->setOverlayPen(format->overlayColor());
//      getScene(axis)->setOverlayPos(QPointF(100,100));

      if (format->hasViewerControls()) {
        views[axis]->setRange(0, img->depth(axis) - 1);
        views[axis]->setSlice(img->currentSlice(axis));
        views[axis]->showControls();
      } else {
        views[axis]->hideControls();
      }
    }
    if (format->modality() == Modality::RGB2D) {
      getScene(1)->setOverlayPen(QPen(Qt::green));
      getScene(2)->setOverlayPen(QPen(Qt::red));
      getScene(3)->setOverlayPen(QPen(Qt::yellow));
    }
    setLayoutType(format->currentLayout());
    setViewMode(format->currentViews());
    controller()->update();
    for (size_t axis = 0; axis < 4; ++axis) {
//      DefaultTool *tool = dynamic_cast<DefaultTool *>(img->tools[0]);
//      tool->updateOverlay( overlaypos, axis );
      if (m_controller) {
        QRectF r = m_controller->getPixmapItem(axis)->boundingRect();
        getScene(axis)->setSceneRect(r);
        QGraphicsView *view = views[axis]->graphicsView();
        view->fitInView(m_controller->getPixmapItem(axis), Qt::KeepAspectRatio);
      }
    }
    updateViews();
  }
}

void ImageViewer::setLayoutType(Layout layout) {
  switch (layout) {
  case Layout::GRID: {
    setGridLayout();
    break;
  }
  case Layout::HORIZONTAL: {
    setHorizontalLayout();
    break;
  }
  case Layout::VERTICAL: {
    setVerticalLayout();
    break;
  }
  }
}

void ImageViewer::getNewLayout() {
  for (ImageWidget *view : views) {
    layout->removeWidget(view);
  }
  updateViews();
}

void ImageViewer::setGridLayout() {
  COMMENT("Set grid layout.", 0)
  getNewLayout();
  layout->addWidget(views[0], 0, 0);
  layout->addWidget(views[1], 0, 1);
  layout->addWidget(views[2], 1, 0);
  layout->addWidget(views[3], 1, 1);

  updateViews();
}

void ImageViewer::setHorizontalLayout() {
  COMMENT("Set horizontal layout.", 0)
  getNewLayout();
  layout->addWidget(views[0], 0, 0);
  layout->addWidget(views[1], 0, 1);
  layout->addWidget(views[2], 0, 2);
  layout->addWidget(views[3], 0, 3);

  updateViews();
}

void ImageViewer::setVerticalLayout() {
  COMMENT("Set vertical layout.", 0)
  getNewLayout();
  layout->addWidget(views[0], 0, 0);
  layout->addWidget(views[1], 1, 0);
  layout->addWidget(views[2], 2, 0);
  layout->addWidget(views[3], 3, 0);

  updateViews();
}

void ImageViewer::hideViews() {
  for (ImageWidget *view : views) {
    view->hide();
  }
  updateViews();
}

void ImageViewer::showViews() {
  for (ImageWidget *view : views) {
    view->show();
  }
  updateViews();
}

void ImageViewer::setViewMode(Views view) {
  hideViews();
  views[0]->setVisible((int)view & (int)Views::SHOW0);
  views[1]->setVisible((int)view & (int)Views::SHOW1);
  views[2]->setVisible((int)view & (int)Views::SHOW2);
  views[3]->setVisible((int)view & (int)Views::SHOW3);
  updateViews();
}

void ImageViewer::sliceChanged(size_t axis, size_t slice) {
  if (m_controller->currentImage()) {
    Tool *tool = m_controller->currentImage()->currentTool();
    if (tool) {
      tool->sliceChanged(axis, slice);
    }
  }
}

void ImageViewer::resizeEvent(QResizeEvent *) { changeImage(); }

bool ImageViewer::eventFilter(QObject *obj, QEvent *evt) {
  QGraphicsSceneMouseEvent *mouseEvt = dynamic_cast<QGraphicsSceneMouseEvent *>(evt);
  size_t axis = 0;
  for (size_t scn = 1; scn < views.size(); ++scn) {
    if (obj == views[scn]->scene()) {
      axis = scn;
    }
  }
  if (mouseEvt) {
    Tool *tool = m_controller->currentImage()->currentTool();
    QPointF scnPos = mouseEvt->scenePos();
    if (mouseEvt->type() == QEvent::GraphicsSceneMouseMove) {
      /*      qDebug() << "MouseEvt = " << scnPos; */
      if (dragging && (timer.elapsed() > 25)) {
        timer.restart();
        emit mouseDragged(scnPos, mouseEvt->buttons(), axis);
        if (tool) {
          tool->mouseDragged(scnPos, mouseEvt->buttons(), axis);
        }
      }
      if (tool) {
        tool->mouseMoved(scnPos, axis);
      }
      emit mouseMoved(scnPos, axis);
    } else if (mouseEvt->type() == QEvent::GraphicsSceneMousePress) {
      /*      qDebug() << "MouseEvt = " << scnPos; */
      if (mouseEvt->button() == Qt::LeftButton) {
        dragging = true;
        timer.restart();
      }
      emit mouseClicked(scnPos, mouseEvt->buttons(), axis);
      if (tool) {
        tool->mouseClicked(scnPos, mouseEvt->buttons(), axis);
      }
    } else if (mouseEvt->type() == QEvent::GraphicsSceneMouseRelease) {
      if (mouseEvt->button() == Qt::LeftButton) {
        dragging = false;
      }
      emit mouseReleased(scnPos, mouseEvt->buttons(), axis);
      if (tool) {
        tool->mouseReleased(scnPos, mouseEvt->buttons(), axis);
      }
    }
  }
  return (QWidget::eventFilter(obj, evt));
}

GraphicsScene *ImageViewer::getScene(size_t axis) {
  if (axis > views.size()) {
    throw std::out_of_range(BIAL_ERROR(QString("Invalid axis, expected < %1.").arg(views.size()).toStdString()));
  }
  return (views[axis]->scene());
}
