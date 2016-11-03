#ifndef LUCOLOR_H
#define LUCOLOR_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> LUCOLORfeature;

class LUCOLOR : public FeatureExtractor<Color, int>
{
  private:
    size_t dim;
    size_t qtd_blocos;

    static int Log( double value , double n );

  public:
    LUCOLOR(FeatureDetector<Color>* Fd);

    LUCOLOR(Vector<tuple<Image<Color>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    LUCOLORfeature Run();
};

#endif
