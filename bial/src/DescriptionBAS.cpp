#include "BAS.h"

#include "Signal.hpp"
#include "Adjacency.hpp"
#include "Geometrics.hpp"
#include "Sorting.hpp"

#include <fstream>
#include <stdlib.h>

bool BAS::ValidContPoint(Image<int> bin, Adjacency L, Adjacency R, int p)
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


Image<int> BAS::LabelContPixel( Image<int> img) // MOVER PARA SEGMEN
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


double BAS::find_angle(int deltax, int deltay)
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



Curve BAS::ImageToCurve(Image<int> img , Image<int> mask)
{
  Image<int> contour = LabelContPixel( img );

  Vector<int> order ,curve_x,curve_y,curve_z;

  for (size_t i = 0; i < contour.size( ) ; i++)
    if ( contour[i] != 0 && mask[i]!=0 )
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

BAS::BAS(FeatureDetector<int> *Fd) : BAS ( Fd->Run() ){ }

BAS::BAS(Vector<tuple<Image<int>,Image<int>>> detected) : FeatureExtractor< int , double >(detected)
{
  this->SAMPLES = 40;
}

void BAS::SetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;
  vet.push_back(tie("SAMPLES" , SAMPLES));

  interpreter->SetExpectedParameters(vet);

  vet = interpreter->Interpret( );

  tie( ignore , SAMPLES) = vet[0] ;

}

string BAS::GetParameters( ParameterInterpreter* interpreter )
{
  Vector<parameter> vet;

  vet.push_back(tie("SAMPLES" , SAMPLES));

  interpreter->SetExpectedParameters(vet);

  return interpreter->GetExpectedParameters(  );
}

BASfeature BAS::Run( )
{
  BASfeature feat;
  Image< int > img;
  Image< int > mask;

  Features<double> histogram;

  Curve contour, curve, rcurve;




  for(size_t i=0; i<this->detected.size(); ++i)
  {
    tie(img,mask)=this->detected[i];

    contour = ImageToCurve( img , mask );

    //Extract Features----------------------------------------------------------
    int x1,x2,x3,y1,y2,y3,curvelength;
    double angle_1,angle_2,angle_curve,total,previous;
    int delta_x, delta_y;

    Vector<int> mean  ( contour.size() );
    Vector<int> second( contour.size() );
    Vector<int> third ( contour.size() );

    curvelength=(int)( contour.size( )/2);

    Vector<int> bearing_array (curvelength-1);

    for(size_t j=0; j<contour.size(); j++)
    {
      total=0.0;
      tie(x1,y1) = contour[ ((j-1)+contour.size( ))%contour.size( ) ];
      tie(x2,y2) = contour[j];
      tie(x3,y3) = contour[ ((j+1)+contour.size( ))%contour.size( ) ];

      delta_x = x1-x2;
      delta_y = -(y1-y2);
      angle_1 = find_angle(delta_x,delta_y);
      delta_x = x3-x2;
      delta_y = -(y3-y2);
      angle_2 = find_angle(delta_x,delta_y);
      if(angle_1 >= angle_2)
        angle_curve = angle_1-angle_2;
      else
        angle_curve = 360.0 + angle_1-angle_2;

      total += angle_curve;

      bearing_array[0]=(int)angle_curve;

      previous = angle_curve;

      for(size_t k=2; k<curvelength; k++)
      {
        tie(x1,y1) = contour[ ((j-k)+contour.size( ))%contour.size( ) ];
        tie(x2,y2) = contour[j];
        tie(x3,y3) = contour[ ((j+k)+contour.size( ))%contour.size( ) ];

        delta_x = x1-x2;
        delta_y = -(y1-y2);
        angle_1 = find_angle(delta_x,delta_y);
        delta_x = x3-x2;
        delta_y = -(y3-y2);
        angle_2 = find_angle(delta_x,delta_y);

        if(angle_1 >= angle_2)
  	     angle_curve = angle_1-angle_2;
        else
  	     angle_curve = 360.0 + angle_1-angle_2;

        if(k > 3)
        {
  	      if(((angle_curve - previous) > 180))
  	        angle_curve = angle_curve - 360.0;

          else if(((previous - angle_curve) > 180))
            angle_curve = angle_curve + 360.0;
        }

        bearing_array[k-1] = (int)angle_curve;
        total += angle_curve;
        previous = angle_curve;
      }

      mean[j]=(int)(total/(double)(curvelength-1));

      total=0.0;
      for(size_t k=0; k<curvelength-1; k++)
        total += pow((bearing_array[k]-mean[j]) , 2.0);
      second[j] = pow(total/(double)(curvelength-2),0.5);

      total=0.0;
      for(size_t k=0; k<curvelength-1; k++)
        total+=pow(abs(bearing_array[k]-mean[j]),3.0);
      third[j]=pow(total/(double)(curvelength-2),(1.0/3.0));

    }
    //--------------------------------------------------------------------------



    //Resample------------------------------------------------------------------

    Image<int> img1( mean.size( ) , 3 );

    for (size_t x=0; x < mean.size( ); x++)
    {
      img1[ x ] = mean[x];
      img1[ x + img1.size(0) * 1 ] = second[x];
      img1[ x + img1.size(0) * 2 ] = third[x];
    }

    Image<int> img2 = Geometrics::Scale( img1 , ( ((float) SAMPLES)/mean.size( ) ),1,1,true);

    //histogram-----------------------------------------------------------------
    histogram = Features<double>( img2.size() );

    for (size_t j = 0; j < img2.size(); j++)
      histogram[ j ] = img2[j];
    //--------------------------------------------------------------------------

    feat.push_back(histogram);
  }
  return feat;
}
