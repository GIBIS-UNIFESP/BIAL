#ifndef BAS_H
#define BAS_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

#define NIL -1
#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef Vector<Features<double>> BASfeature;

typedef Vector< tuple<int,int> > Curve;

class BAS : public FeatureExtractor<int, double>
{
  private:

    size_t SAMPLES;

    static bool ValidContPoint(Image<int> bin, Adjacency L, Adjacency R, int p);//mover
    static Image<int> LabelContPixel( Image<int> img);//mover para segmentacion
    static inline double find_angle(int deltax, int deltay);//mover ?
    static Curve ImageToCurve(Image<int> img , Image<int> mask);


  public:
    BAS(FeatureDetector<int>* Fd);

    BAS(Vector<tuple<Image<int>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    BASfeature Run();
};

#endif
