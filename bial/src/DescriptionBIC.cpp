#include "BIC.h"
#include "Adjacency.hpp"
#include "Color.hpp"

int BIC::Log(float  value , double n)
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

BIC::BIC(FeatureDetector<Color> *Fd) : BIC ( Fd->Run() ){ }

BIC::BIC(Vector<tuple<Image<Color>,Image<int>>> detected) : FeatureExtractor< Color , int >(detected)
{ this->dim = 4; }

void BIC::SetParameters( ParameterInterpreter* interpreter)
{
  Vector<parameter> vet;
  vet.push_back(tie("dim",dim));

  interpreter->SetExpectedParameters(vet);
  vet = interpreter->Interpret( );

  tie(ignore , dim) = vet[0];

}

string BIC::GetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("dim",dim));

  interpreter->SetExpectedParameters(vet);

  return interpreter->GetExpectedParameters(  );
}

BICfeature BIC::Run( )
{
  int size = dim * dim * dim;

  vector<int> border, interior;

  BICfeature feat;
  Image< Color > img;
  Image< int > mask;
  Image< unsigned > quantized;
  int same_color;
  unsigned char r,g,b;
  unsigned char fator_g = dim;
  unsigned char fator_b = fator_g * dim;

  Features<int> complete_histogram;

  for(size_t i=0; i<this->detected.size(); ++i)
  {

    //quantização------------------------------------------------
    tie(img,mask)=this->detected[i];
    quantized=Image<unsigned char>(img.size(0),img.size(1));

    for(size_t j=0; j<quantized.size(  ); ++j)
    {
      r=dim*img[j].channel[1]/256;
      g=dim*img[j].channel[2]/256;
      b=dim*img[j].channel[3]/256;

      quantized[j]=(r + fator_g*g + fator_b*b);
    }
    //-----------------------------------------------------------

    //Classificação dos pixels----------------------------------


    Adjacency adjpixels = Adjacency::Circular( 1.0 );
    for (size_t y = 0; y < quantized.size(1); y++)
    for (size_t x = 0; x < quantized.size(0); x++)
      if(mask(x,y)==1)
      {
        same_color=0;

        size_t p = x + y * quantized.size(0);

        for( size_t pos = 1; pos < adjpixels.size( ); ++pos )
        {
          int x_dsp = x + adjpixels( pos, 0 );
          int y_dsp = y + adjpixels( pos, 1 );
          int p_dsp = x_dsp + y_dsp * quantized.size(0);

          if(quantized.ValidCoordinate(x_dsp , y_dsp) && mask[p_dsp]==1  && quantized[p]==quantized[p_dsp] )
            ++same_color;
        }

        if(same_color==4)
          interior.push_back(quantized[p]);
        else
          border.push_back(quantized[p]);
      }

    //----------------------------------------------------------


    //Histograma------------------------------------------------
    complete_histogram = Features<int>(2 * size) ;

    for(size_t j=0; j<border.size(); j++)
      complete_histogram[ border[j] + size ]++;

    for(size_t j=0; j<interior.size(); j++)
      complete_histogram[ interior[j] ]++;

    for(size_t j=0; j<complete_histogram.size(); j++)
      complete_histogram[j]=this->Log(complete_histogram[j] , quantized.size());

    feat.push_back(complete_histogram);
  }

  return feat;
}
