#ifndef CSD_H
#define CDS_H

#include "FeatureExtractor.h"
#include <tuple>

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> CSDfeature;

class CSD : public FeatureExtractor<Color, int>
{
  private:
    static int Log( double value , double n );
    static Image<Color> RgbToHmmd(Image<Color> img);

  public:
    CSD(FeatureDetector<Color>* Fd);

    CSD(Vector<tuple<Image<Color>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    CSDfeature Run();
};

#endif
