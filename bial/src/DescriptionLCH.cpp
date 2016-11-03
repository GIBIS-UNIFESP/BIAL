#include "LCH.h"

#include "Signal.hpp"
#include "Adjacency.hpp"

#include <fstream>

int LCH::Log(double  value , double n)
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

LCH::LCH(FeatureDetector<Color> *Fd) : LCH ( Fd->Run() ){ }

LCH::LCH(Vector<tuple<Image<Color>,Image<int>>> detected) : FeatureExtractor< Color , int >(detected)
{
  this->dim = 4;
  this->BINS = 4;
}

void LCH::SetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("dim",dim));
  vet.push_back(tie("BINS",BINS));

  interpreter->SetExpectedParameters(vet);
  vet = interpreter->Interpret( );

  tie(ignore , dim) = vet[0];
  tie(ignore , BINS) = vet[1];
}

string LCH::GetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("dim",dim));
  vet.push_back(tie("BINS",BINS));

  interpreter->SetExpectedParameters(vet);

  return interpreter->GetExpectedParameters(  );
}

LCHfeature LCH::Run( )
{
  size_t size = dim * dim * dim;
  size_t NBINS = BINS * BINS * size;

  LCHfeature feat;
  Image< Color > img;
  Image< int > mask;
  Image< int > quantized;

  unsigned char fator_g = dim;
  unsigned char fator_b = fator_g * dim;
  unsigned char r,g,b;


  Features<int> histogram;

  for(size_t i=0; i<this->detected.size(); ++i)
  {
    //quantização------------------------------------------------
    tie(img,mask)=this->detected[i];

    quantized=Image<int>(img.size(0),img.size(1));

    for(size_t j=0; j<quantized.size(  ); ++j)
    {
      r=dim*img[j].channel[1]/256;
      g=dim*img[j].channel[2]/256;
      b=dim*img[j].channel[3]/256;

      quantized[j]=(r + fator_g*g + fator_b*b);
    }
    //-----------------------------------------------------------

    //Criação do Histograma--------------------------------------
    ulong fator_x, fator_y;
    ulong x, y;
    ulong r, c;

    histogram = Features<int>(NBINS);

    fator_x = size;
    fator_y = BINS * size;

    size_t mask_pixels = 0;

    for( r=0; r < quantized.size(1) ; r++ )
     for( c=0 ; c < quantized.size(0) ; c++ )
     if(mask(c,r)==1)
     {
       mask_pixels++;

       x = BINS*r/quantized.size(1);
       y = BINS*c/quantized.size(0);

       histogram[ quantized[ r * quantized.size(0) + c ] + fator_x * x + fator_y * y ]++;
     }

    //----------------------------------------------------------

    //Compressão do Histograma------------------------------------------------
    for(size_t j=0; j<histogram.size(); j++)
        histogram[j] = (uchar)(255. * ((float) histogram[j] / (float) mask_pixels ));

    //----------------------------------------------------------



    feat.push_back(histogram);
  }
  return feat;
}
