#ifndef NODETECTOR_H
#define NODETECTOR_H

template<class T>  class NoDetector:public FeatureDetector<T>
{
  public:
    NoDetector(Image<T>);
    Vector<tuple<Image<T>,Image<int>>> Run( );
};

#include "NoDetector.cpp"

#endif
