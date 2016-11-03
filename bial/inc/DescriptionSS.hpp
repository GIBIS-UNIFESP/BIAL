#ifndef SS_H
#define SS_H

#include "FeatureExtractor.h"

#define INTERIOR 0
#define EXTERIOR 1
#define BOTH     2

#define WHITE 0
#define GRAY  1
#define BLACK 2

using namespace std;
using namespace Bial;

typedef Vector<Features<double>> SSfeature;

typedef Vector< tuple<int,int> > Curve;
typedef Vector< tuple<double,double> > CurveDouble;


class SS : public FeatureExtractor<int, double>
{
  private:

    size_t SAMPLES, MAXDIST, SIDE;

    static Image<int> Mbb( Image<int> img );//mover para segmentacion

    static bool ValidContPoint(Image<int> bin, Adjacency L, Adjacency R, int p);//mover
    static Image<int> LabelContPixel( Image<int> img);//mover para segmentacion
    static inline double find_angle(int deltax, int deltay);//mover ?
    static Curve ImageToCurve(Image<int> img);


  public:
    SS(FeatureDetector<int>* Fd);

    SS(Vector<tuple<Image<int>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    SSfeature Run();
};

#endif
