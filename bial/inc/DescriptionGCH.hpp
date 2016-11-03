#ifndef GCH_H
#define GCH_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> GCHfeature;

class GCH : public FeatureExtractor< Color, int >
{
  private:
    size_t dim;
    static int Log( double value , double n );

  public:
    GCH(FeatureDetector<Color>* Fd);

    GCH(Vector<tuple<Image<Color>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    GCHfeature Run();
};

#endif
