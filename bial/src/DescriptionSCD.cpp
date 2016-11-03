#include "SCD.h"

#include "Signal.hpp"
#include "Adjacency.hpp"

#include <fstream>
#include <stdlib.h>

int SCD::Log(double  value , double n)
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

Image<Color> SCD::RgbToHsv(Image<Color> img)
{
  float r, g, b, h, s, v, minimum, maximum, f;
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
      {
          h=0.0;
          s=0.0;
          v=maximum;
      }
      else
      {
          f=(r==minimum)? g-b : ((g == minimum)? b - r : r - g);
          k=(r==minimum)? 3 : ((g==minimum)? 5 : 1);

          h=( (float)k-f/(maximum-minimum) );
          s=(maximum-minimum)/maximum;
          v=maximum;
      }

      nova[i].channel[1] = round( abs( 255./6 * h ) );
      nova[i].channel[2] = round( abs( 255. *   s ) );
      nova[i].channel[3] = round( abs( 255. *   v ) );
  }

  return nova;
}

SCD::SCD(FeatureDetector<Color> *Fd) : SCD ( Fd->Run() ){ }

SCD::SCD(Vector<tuple<Image<Color>,Image<int>>> detected) : FeatureExtractor< Color , int >(detected)
{ this->dim = 4; }

void SCD::SetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("dim",dim));

  interpreter->SetExpectedParameters(vet);
  vet = interpreter->Interpret( );

  tie(ignore , dim) = vet[0];
}

string SCD::GetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("dim",dim));
  interpreter->SetExpectedParameters(vet);

  return interpreter->GetExpectedParameters(  );
}



SCDfeature SCD::Run( )
{
  size_t size = dim * dim * dim * dim;

  SCDfeature feat;
  Image< Color > img, cimg;
  Image< int > mask;
  Image< int > quantized;

  ulong fator_s = dim * dim;
  ulong fator_v = fator_s * dim;
  ulong h,s,v;


  Features<int> histogram;

  for(size_t i=0; i<this->detected.size(); ++i)
  {
    tie(img,mask)=this->detected[i];

    //quantização------------------------------------------------
    quantized=Image<int>(img.size(0),img.size(1));
    cimg = RgbToHsv(img);

    for(size_t j=0; j<quantized.size(  ); ++j)
    {
      h=dim*dim*cimg[j].channel[1]/256;
      s=dim*cimg[j].channel[2]/256;
      v=dim*cimg[j].channel[3]/256;

      quantized[j]=(h + fator_s*s + fator_v*v);
    }
    //-----------------------------------------------------------

    //Histograma------------------------------------------------
    histogram = Features<int>( size ) ;

    for(size_t j=0; j< quantized.size() ; j++)
      if(mask[j]==1)
        histogram[ quantized[j] ] ++;

    for(size_t j=0; j< size ; j++)
        histogram[j]=(int)Log( histogram[j] , quantized.size() );

    feat.push_back(histogram);
    //----------------------------------------------------------
  }

  return feat;
}
