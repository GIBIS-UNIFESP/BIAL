#include "CSD.h"

#include "Signal.hpp"
#include "Adjacency.hpp"

#include <fstream>
#include <stdlib.h>

int CSD::Log(double  value , double n)
{
  value = 255.0 * value / n;
  if(value==0.)
    return 0;
  else if(value<1.)
    return 1;
  else if(value<2.)
    return 2;
  else if(value<4.)
    return 3;
  else if(value<8.)
    return 4;
  else if(value<16.)
    return 5;
  else if(value<32.)
    return 6;
  else if(value<64.)
    return 7;
  else if(value<128.)
    return 8;
  else
    return 9;
}

Image<Color> CSD::RgbToHmmd(Image<Color> img)
{
  float r, g, b, h, s, d, minimum, maximum, f;
  int k;

  Image<Color> nova( img.size(0), img.size(1) );

  for(int i=0; i<img.size(); i++)
  {
      r=(float)img[i].channel[1]/255.0;
      g=(float)img[i].channel[2]/255.0;
      b=(float)img[i].channel[3]/255.0;

      minimum=min(r,min(g,b));
      maximum=max(r,max(g,b));

      if(maximum==minimum)
          h=0.0;
      else
      {
          f=(r==minimum)? g-b : ((g == minimum)? b - r : r - g);
          k=(r==minimum)? 3 : ((g==minimum)? 5 : 1);

          h=( (float)k-f/(maximum-minimum) );
      }

      s=(maximum+minimum)/2;
      d=maximum-minimum;

      nova[i].channel[1] = round( abs( 255./6 * h ) );
      nova[i].channel[2] = round( abs( 255. *   s ) );
      nova[i].channel[3] = round( abs( 255. *   d ) );
  }

  return nova;
}

CSD::CSD(FeatureDetector<Color> *Fd) : CSD ( Fd->Run() ){ }

CSD::CSD(Vector<tuple<Image<Color>,Image<int>>> detected) : FeatureExtractor< Color , int >(detected){ }

void CSD::SetParameters( ParameterInterpreter* interpreter ){ }

string CSD::GetParameters( ParameterInterpreter* interpreter )
{
  return "";
}

CSDfeature CSD::Run( )
{
  size_t size = 64;

  CSDfeature feat;
  Image< Color > img, hmmd;
  Image< int > mask;
  Image< int > quantized;

  ulong h,s,d;

  int k, m,  w;
  size_t maximum;

  Vector<int> dx;
  Vector<int> dy;
  vector<bool> colors;

  Features<int> histogram;

  for(size_t i=0; i<this->detected.size(); ++i)
  {
    tie(img,mask)=this->detected[i];

    //quantização------------------------------------------------
    quantized=Image<int>(img.size(0),img.size(1));
    hmmd = RgbToHmmd(img);

    for(size_t j=0; j<quantized.size(  ); ++j)
    {
      d = 5 * hmmd[j].channel[3]/256;
      switch (d) {
        case 0:
          s = 8 * hmmd[j].channel[2]/256;
          quantized[j] = s;
          break;
        case 1:
          h = 4 * hmmd[j].channel[1]/256;
          s = 4 * hmmd[j].channel[2]/256;
          quantized[j] = 8 + 4 * s + h;
          break;
        case 2:
          h = 6*hmmd[j].channel[1]/256;
          s = 4*hmmd[j].channel[2]/256;
          quantized[j] = 24 + 6 * s + h;
          break;
        case 3:
        case 4:
          h = 4*hmmd[j].channel[1]/256;
          s = 4*hmmd[j].channel[2]/256;
          quantized[j] = 48 + 4 * s + h;
          break;
      }
    }
    //-----------------------------------------------------------

    //Histograma------------------------------------------------
    histogram = Features<int>( size ) ;
    colors = vector<bool>( size );


    k = max(0 , ((int)(log( quantized.size() )/log(2)))/2-8);
    w = 1 << k;
    m = 8 * w;

    Adjacency adj = Adjacency::AdjacencyBox( m , m );

    maximum = 0;

    for(size_t y=0; y <= quantized.size(1) - m; y += w)
      for(size_t x=0; x <= quantized.size(0) - m; x += w)
        if(mask(x,y)==1)
        {
          for(size_t j=0; j< size ; j++)
            colors[j] = false;

          for( size_t pos = 0; pos < adj.size( ); ++pos )
          {
            int x_dsp = x + adj(pos , 0);
            int y_dsp = y + adj(pos , 1);


            if(quantized.ValidCoordinate(x_dsp , y_dsp) && mask(x_dsp , y_dsp)==1)
            {
              size_t p = x_dsp + y_dsp * quantized.size(0);
              colors[ quantized[p] ] = true;
            }
          }

          for (size_t j = 0; j < size; j++)
            if(colors[j])
              histogram[j]++;

          maximum++;
        }

    for(size_t j=0; j< size ; j++)
        histogram[j]=(int)Log( histogram[j] , maximum );
    //----------------------------------------------------------



    feat.push_back(histogram);
  }

  return feat;
}
