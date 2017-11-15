#ifndef ROBOTUSER_H
#define ROBOTUSER_H
#include "Image.hpp"
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
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
  CvSVM* m_SVM;



public:
  Bial::Vector< Path > m_GTpaths;
  RobotUser( ActiveContourTool &tool );
  QPointF toPointF( int pxl );
  void train( );
  void runSingle( std::shared_ptr< ActiveContourMethod > method );
  void run( );
  void runTest1( );
  void runTest2( );
  void report( const std::string &methodName, size_t total_errors );
  void setSVM(CvSVM *SVM);
  bool trained( );

};

#endif // ROBOTUSER_H
