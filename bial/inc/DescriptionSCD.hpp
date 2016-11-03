#ifndef SCD_H
#define SCD_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> SCDfeature;

class SCD : public FeatureExtractor<Color, int>
{
  private:
    size_t dim;
    static int Log( double value , double n );
    static Image<Color> RgbToHsv(Image<Color> img);

  public:
    SCD(FeatureDetector<Color>* Fd);

    SCD(Vector<tuple<Image<Color>,Image<int>>> detected);


    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    SCDfeature Run();
};

#endif
