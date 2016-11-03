#ifndef LAS_H
#define LAS_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> LASfeature;

class LAS : public FeatureExtractor<int , int>
{
  private:
    size_t bins;

    static int Log( double value , double n );
    static size_t QuantizeValue(int value);

  public:
    LAS(FeatureDetector<int>* Fd);

    LAS(Vector<tuple<Image<int>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    LASfeature Run();
};

#endif
