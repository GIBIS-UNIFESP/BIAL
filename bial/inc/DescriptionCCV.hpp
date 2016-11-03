#ifndef CCV_H
#define CCV_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

#define HIGH true
#define LOW false
#define MIN_AREA 1
#define NIL -1

typedef Vector<Features<int>> CCVfeature;

class CCV : public FeatureExtractor<Color, int>
{
  private:
    size_t dim;

    static int Log( double value , double n );

  public:
    CCV(FeatureDetector<Color>* Fd);

    CCV(Vector<tuple<Image<Color>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    CCVfeature Run();
};

#endif
