#ifndef ACC_H
#define ACC_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> ACCfeature;

class ACC : public FeatureExtractor<Color, int>
{
  private:
    size_t dim;
    size_t n_distances;
    Vector<size_t> distances;

    static int Log( double value , double n );

  public:
    ACC(FeatureDetector<Color> *Fd);

    ACC(Vector<tuple<Image<Color>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    ACCfeature Run();
};

#endif
