#ifndef ROBOTUSER_H
#define ROBOTUSER_H
#include "Image.hpp"

#include <QObject>
class LiveWireTool;

typedef Bial::Vector< size_t > Path;

class RobotUser : public QObject {
  Q_OBJECT
  LiveWireTool & m_tool;
  Bial::Image< int > m_groundTruth;
  Path m_contour;
  void plotPath( const Path &path, QColor clr );
public:
  RobotUser( LiveWireTool &tool );
  void run( );
  QPointF toPointF( int pxl );
};

#endif // ROBOTUSER_H
