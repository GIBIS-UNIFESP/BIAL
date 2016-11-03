#ifndef CCH_H
#define CCH_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> CCHfeature;

class CCH : public FeatureExtractor<Color, int >
{
  private:
    size_t dim;
    size_t grid;

    static int Log( double value , double n );

  public:
    CCH(FeatureDetector<Color>* Fd);

    CCH(Vector<tuple<Image<Color>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    CCHfeature Run();
};

#endif
