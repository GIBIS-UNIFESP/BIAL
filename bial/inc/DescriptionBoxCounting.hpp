#ifndef BOXCOUNTING_H
#define BOXCOUNTING_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

typedef Vector<Features<double>> BOXCOUNTINGfeature;

class BOXCOUNTING : public FeatureExtractor<int, double>
{
  private:
    static Image<int> Mbb( Image<int> img , Image<int> mask  );

  public:
    BOXCOUNTING(FeatureDetector<int>* Fd);

    BOXCOUNTING(Vector<tuple<Image<int>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    BOXCOUNTINGfeature Run();
};

#endif
