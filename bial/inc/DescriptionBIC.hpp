#ifndef BIC_H
#define BIC_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> BICfeature;

class BIC : public FeatureExtractor<Color, int>
{
  private:
    size_t dim;

    static int Log( float value , double n );

  public:
    BIC(FeatureDetector<Color>* Fd);

    BIC(Vector<tuple<Image<Color>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    BICfeature Run();
};

#endif
