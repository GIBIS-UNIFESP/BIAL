#include "TAR.h"

#include "Signal.hpp"
#include "Adjacency.hpp"
#include "Geometrics.hpp"
#include "Sorting.hpp"

#include <fstream>
#include <stdlib.h>


bool TAR::ValidContPoint(Image<int> bin, Adjacency L, Adjacency R, int p)
{
  int left_side,right_side;

  int u_x, u_y ,v_x, v_y , l_x, l_y, r_x, r_y;

  bool found=false;

  u_x = p % bin.size(0);
  u_y = p / bin.size(0);

  for (size_t i=0; i < L.size( ); i++)
  {
    v_x = u_x + L( i , 0 );
    v_y = u_y + L( i , 1 );

    if( bin.ValidCoordinate( v_x , v_y ) )
    {
      size_t q = v_x + bin.size(0) * v_y;

      if ( (bin[q] == 1 )&&( p != q ) )
      {
        l_x = u_x + L( i , 0 );
        l_y = u_y + L( i , 1 );

        r_x = u_x + R( i , 0 );
        r_y = u_y + R( i , 1 );

        if ( bin.ValidCoordinate(l_x , l_y) )
          left_side = l_x + bin.size(0) * l_y;
        else
          left_side = -1;

        if (bin.ValidCoordinate(r_x , r_y) )
          right_side = r_x + bin.size(0) * r_y;
        else
          right_side = -1;

        if (((left_side!=-1)&&(right_side!=-1)&&(bin[left_side]!=bin[right_side]))||
            ((left_side==-1)&&(right_side!=-1)&&(bin[right_side]==1)) ||
            ((right_side==-1)&&(left_side!=-1)&&(bin[left_side]==1)))
        {
          found = true;
          break;
        }
      }
    }
  }

  return(found);
}


Image<int> TAR::LabelContPixel( Image<int> img) // MOVER PARA SEGMEN
{
  int u_x, u_y, v_x, v_y, w_x, w_y, q, p, left_side, right_side;

  Adjacency A = Adjacency::Circular( 1.0f );
  Adjacency::FixAdj(A);

  Image<int> bndr (img.size(0), img.size(1));

  for (p=0 ; p < img.size() ; p++)
  {
    if (img[p] == 1)
    {
      u_x = p % img.size(0);
      u_y = p / img.size(0);

      for (size_t i=1 ; i < A.size() ; i++)
      {
        v_x = u_x + A( i , 0 );
        v_y = u_y + A( i , 1 );

        if (img.ValidCoordinate(v_x,v_y))
        {
          q = v_x + v_y * img.size(0);

          if (img[q] == 0)
          {
            bndr[p] = 1;
            break;
          }
        }
        else
        {
          bndr[p] = 1;
          break;
        }
      }
    }
  }

  A = Adjacency::Circular( 1.5f );
  Adjacency::FixAdj(A);

  Adjacency L = Adjacency::LeftSide(A);
  Adjacency R = Adjacency::RightSide(A);

  Image<int> label ( bndr.size(0) , bndr.size(1) );
  Image<int> color ( bndr.size(0) , bndr.size(1) );
  Image<int> pred  ( bndr.size(0) , bndr.size(1) );

  Vector<int> LIFO ( bndr.size() );


  int last   = -1;


  for (size_t j = 0 ; j < bndr.size() ; j++)
  {
    if ( (bndr[j] == 1) && (color[j] != BLACK) && ValidContPoint(img,L,R,j) )
    {
      last++;
      LIFO[last] = j;
      color[j] = GRAY;
      pred[j] = j;
      while(last != -1)
      {
        p = LIFO[last];
        last--;
        color[p] = BLACK;
        u_x = p % bndr.size(0);
        u_y = p / bndr.size(0);

        for (size_t i=1 ; i < A.size() ; i++)
        {
          v_x = u_x + A( i , 0 );
          v_y = u_y + A( i , 1 );

          if (bndr.ValidCoordinate( v_x,v_y ) )
          {
            q = v_x + bndr.size(0) * v_y;
            if ((q==j)&&(pred[p]!=j))
            {
              last = -1;
              break;
            }

            w_x = u_x + L( i , 0 );
            w_y = u_y + L( i , 1 );

            if ( bndr.ValidCoordinate( w_x,w_y ) )
              left_side = w_x + bndr.size(0) * w_y;
            else
              left_side = -1;

            w_x = u_x + R( i , 0 );
            w_y = u_y + R( i , 1 );

            if (bndr.ValidCoordinate( w_x , w_y ) )
              right_side = w_x + bndr.size(0) * w_y;
            else
              right_side = -1;

            if ((bndr[q]==1)&&(color[q] != BLACK)&&
               (((left_side!=-1)&&(right_side!=-1)&&(img[left_side]!=img[right_side]))||
               ((left_side==-1)&&(right_side!=-1)&&(img[right_side]==1)) ||
               ((right_side==-1)&&(left_side!=-1)&&(img[left_side]==1))))
            {
              pred[q] = p;
              if (color[q] == WHITE)
              {
                last++;
                LIFO[last] = q;
                color[q] = GRAY;
              }
            }
          }
        }
      }
      int r = p;
      int l = 1;

      while(pred[p] != p)
      {
        label[p] = l;
        p = pred[p];
        l++;
      }
      if (r != p)
        label[p] = l;
    }
  }

  return(label);
}


double TAR::find_angle(int deltax, int deltay)
{
  double angle;
  double pi;

  pi=22.0/7.0;

  if((deltax==0) && (deltay==0))
    angle=0.0;
  else
  {
    angle=atan((10.0*abs(deltax))/(10.0*abs(deltay)));
    angle=angle*180.0/pi;
    if((deltax <= 0) && (deltay >= 0))
      angle=360.0-angle;
    else if((deltax <= 0) && (deltay <=0))
      angle=180.0 + angle;
    else if((deltax >= 0) && (deltay <=0))
      angle=180.0 - angle;
  }

  return(angle);
}



Curve TAR::ImageToCurve(Image<int> img , Image<int> mask)
{
  Image<int> contour = LabelContPixel( img );

  Vector<int> order ,curve_x,curve_y,curve_z;

  for (size_t i = 0; i < contour.size( ) ; i++)
    if ( contour[i] != 0 && mask[i]!=0  )
    {
      curve_x.push_back(  (int)(i % contour.size(0)) ) ;
      curve_y.push_back(  (int)(i / contour.size(0)) ) ;
      curve_z.push_back( contour[i] );
    }

  if(curve_z.size() == 0)
    throw "empty image";

  order =  Sorting::Sort( curve_z , true );

  Sorting::Sort( curve_x , order );
  Sorting::Sort( curve_y , order );

  Curve curve( order.size( ) );

  for (size_t i = 0; i < order.size( ); i++)
    curve[i] = make_tuple( curve_x[i] , curve_y[i] );

  return (curve);

}

Vector<double> TAR::ComputeScale(Curve contour, int n, int ts)
{
  Vector<double> tar(n);

  for (size_t i=0; i < tar.size(); i++)
  {
    int l = (n + i - ts) % n;
    int r = (i + ts) % n;

    tar[i] = 0.5 * ( get<0>(contour[l]) * get<1>(contour[i]) + get<0>(contour[i]) * get<1>(contour[r])
                    +get<0>(contour[r]) * get<1>(contour[l]) -
                     get<0>(contour[r]) * get<1>(contour[i]) - get<0>(contour[i]) * get<1>(contour[l])
                    -get<0>(contour[l]) * get<1>(contour[r])  );
  }

  return(tar);
}


Vector< Vector <double> > TAR::ComputeSignature(Curve contour , int n)
{
  int nscales = (n - 1) / 2;

  Vector< Vector <double> > tar(nscales);

  for (size_t ts=0; ts < nscales; ts++)
  {
    Vector<double> scale = ComputeScale(contour , n, ts+1);

    Vector<double> abs_scale(scale);
    abs_scale.Abs( );

    double max = abs_scale.Maximum( );

    if(max!=0)
      scale /= max;
    else
      throw "divisao por zero";

    tar[ts] = scale;

  }
  return(tar);
}

double TAR::ShapeComplexity(Vector< Vector <double> > tar, int samples)
{
  double sc = 0.0;

  int nscales = (samples - 1) / 2;

  Vector< Vector <double> > reverse_tar(samples);

  for (size_t i=0; i < samples; i++)
    for (size_t ts=0; ts < nscales; ts++)
      reverse_tar[i].push_back(tar[ts][i]);

  for (size_t i=0; i < samples; i++)
  {
    double max = reverse_tar[i].Maximum( );
    double min = reverse_tar[i].Minimum( );

    sc += fabs(max - min);
  }

  sc /= samples;

  return(sc);
}

TAR::TAR(FeatureDetector<int> *Fd) : TAR ( Fd->Run() ){ }

TAR::TAR(Vector<tuple<Image<int>,Image<int>>> detected) : FeatureExtractor< int , double >(detected)
{
  this->SAMPLES = 128;
}

void TAR::SetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("SAMPLES" , SAMPLES));

  interpreter->SetExpectedParameters(vet);

  vet = interpreter->Interpret( );

  tie( ignore , SAMPLES) = vet[0] ;

}

string TAR::GetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;

  vet.push_back(tie("SAMPLES" , SAMPLES));

  interpreter->SetExpectedParameters(vet);

  return interpreter->GetExpectedParameters(  );
}

TARfeature TAR::Run( )
{
  TARfeature feat;
  Image< int > img;
  Image< int > mask;

  Features<double> histogram;

  Curve contour;

  for(size_t i=0; i<this->detected.size(); ++i)
  {
    tie(img,mask)=this->detected[i];

    contour = ImageToCurve( img , mask );

    //Resample------------------------------------------------------------------

    Image<int> img1( contour.size( ) , 2 );

    for (size_t x=0; x < contour.size( ); x++)
      tie(img1[ x ] , img1[ x + img1.size(0)]) = contour[x];

    Image<int> img2 = Geometrics::Scale( img1 , ( ((float) SAMPLES)/contour.size( ) ), 1.0,true);

    Curve rcontour(SAMPLES);

    for (size_t x=0; x<SAMPLES; x++)
      rcontour[x] = tie(img2[x] , img2[x + img1.size(0)]);



    //--------------------------------------------------------------------------

    Vector< Vector <double> >tar = ComputeSignature(rcontour , SAMPLES);
    double sc = ShapeComplexity(tar,SAMPLES);

    //Histogram-----------------------------------------------------------------
    histogram = Features<double>( tar.size() * SAMPLES + 1 );

    size_t p = 0;


    for (size_t ts=0; ts < tar.size(); ts++)
      for (size_t j=0; j < tar[ts].size(); j++)
          histogram[p++] = tar[ts][j];

    histogram[p] = sc;
    //--------------------------------------------------------------------------

    feat.push_back(histogram);

  }
  return feat;
}
