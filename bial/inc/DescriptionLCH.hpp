#ifndef LCH_H
#define LCH_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> LCHfeature;

class LCH : public FeatureExtractor<Color, int>
{
  private:
    size_t dim;
    size_t BINS;

    static int Log( double value , double n );

  public:
    LCH(FeatureDetector<Color>* Fd);

    LCH(Vector<tuple<Image<Color>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    LCHfeature Run();
};

#endif
