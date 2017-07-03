#ifndef ROBOTUSER_H
#define ROBOTUSER_H
#include "Image.hpp"

#include <QObject>
class ActiveContourTool;
class ActiveContourMethod;

typedef Bial::Vector< size_t > Path;

class RobotUser : public QObject {
  Q_OBJECT
  ActiveContourTool & m_tool;
  Bial::Image< int > m_groundTruth;
  Path m_contour;
  void plotPath( const Path &path, QColor clr );
public:
  RobotUser( ActiveContourTool &tool );
  QPointF toPointF( int pxl );
  void train( );
  void runSingle( std::shared_ptr< ActiveContourMethod > method );
  void run( );
  void report( const std::string &methodName, size_t total_errors );
};

#endif // ROBOTUSER_H
