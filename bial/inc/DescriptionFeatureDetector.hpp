#ifndef FEATUREDETECTOR_H
#define FEATUREDETECTOR_H

#include "Image.hpp"
#include <tuple>

using namespace std;
using namespace Bial;

template<class T>  class FeatureDetector
{
  protected:
    Image<T> img;

  public:
    FeatureDetector(Image<T> img);
    virtual Vector<tuple<Image<T>,Image<int>>> Run( )=0;
};

template<class T> FeatureDetector<T>::FeatureDetector(Image<T> img){this->img = img;}

#include "NoDetector.h"

#endif
