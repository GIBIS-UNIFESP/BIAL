#include "GCH.h"

#include "Signal.hpp"
#include "Adjacency.hpp"

int GCH::Log(double  value , double n)
{
  value = 255.0 * value / n;
  if(value==0.0)
    return 0;
  else if(value<1.0)
    return 1;
  else if(value<2.0)
    return 2;
  else if(value<4.0)
    return 3;
  else if(value<8.0)
    return 4;
  else if(value<16.0)
    return 5;
  else if(value<32.0)
    return 6;
  else if(value<64.0)
    return 7;
  else if(value<128.0)
    return 8;
  else
    return 9;
}

GCH::GCH(FeatureDetector<Color> *Fd) : GCH ( Fd->Run() ){ }

GCH::GCH(Vector<tuple<Image<Color>,Image<int>>> detected) : FeatureExtractor< Color , int >(detected)
{this->dim = 4;}




void GCH::SetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("dim",dim));

  interpreter->SetExpectedParameters(vet);
  vet = interpreter->Interpret(  );

  tie(ignore , dim) = vet[0];
}

string GCH::GetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("dim",dim));

  interpreter->SetExpectedParameters(vet);

  return interpreter->GetExpectedParameters(  );
}

GCHfeature GCH::Run( )
{
  size_t size  = dim * dim * dim;

  GCHfeature feat;
  Image< Color > img;
  Image< int > mask;
  Image< unsigned char > quantized;
  unsigned char fator_g = dim;
  unsigned char fator_b = fator_g * dim;
  unsigned char r,g,b;
  Features<int> complete_histogram;

  for(size_t i=0; i<detected.size(); ++i)
  {
    //quantização------------------------------------------------
    tie(img,mask)=detected[i];
    quantized=Image<unsigned char>(img.size(0),img.size(1));

    for(size_t j=0; j<quantized.size(  ); ++j)
    {
      r=dim*img[j].channel[1]/256;
      g=dim*img[j].channel[2]/256;
      b=dim*img[j].channel[3]/256;

      quantized[j]=(r + fator_g*g + fator_b*b);
    }
      //-----------------------------------------------------------
    //Histograma------------------------------------------------
    complete_histogram = Features<int>(size) ;
    for(size_t i=0; i<quantized.size(); i++)
      if(mask[i]==1)
        (complete_histogram[ quantized[i] ])++;

    for(size_t i=0; i<complete_histogram.size(); i++)
      complete_histogram[i]=this->Log( complete_histogram[i]  , quantized.size());
    //----------------------------------------------------------Z
    feat.push_back(complete_histogram);
  }

  return feat;
}
