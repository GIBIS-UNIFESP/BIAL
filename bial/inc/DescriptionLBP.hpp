#ifndef LBP_H
#define LBP_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<int>> LBPfeature;

class LBP : public FeatureExtractor<int, int>
{
  private:
    static int Log( double value , double n );

    static uchar Uniformity(uchar s[8]);

    static uchar Signal(int x);

  public:
    LBP(FeatureDetector<int>* Fd);

    LBP(Vector<tuple<Image<int>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    LBPfeature Run();
};

#endif
