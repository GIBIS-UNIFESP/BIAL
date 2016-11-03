#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "Features.h"
#include "FeatureDetector.h"
#include "ParameterInterpreter.h"

using namespace std;
using namespace Bial;

template<class I, class F>  class FeatureExtractor
{
  protected:
    Vector<tuple<Image<I>,Image<int>>> detected;

  public:
    FeatureExtractor( Vector<tuple<Image<I>,Image<int>>> detected);

    virtual void SetParameters( ParameterInterpreter* interpreter )=0;

    virtual string GetParameters( ParameterInterpreter* interpreter )=0;

    virtual Vector<Features<F>> Run()=0;
};

template<class I, class F>  FeatureExtractor<I,F>::FeatureExtractor( Vector<tuple<Image<I>,Image<int>>> detected )
{ this->detected = detected; }

#endif
