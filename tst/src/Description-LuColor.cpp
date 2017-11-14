#include <iostream>
#include <stdexcept>
#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

#include "DescriptionLuColor.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {

  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    exit( -1 );
  }

  Image < Color > img = Read< Color >( argv[ 1 ] ) ;

  FeatureDetector < Color >* detector = new NoDetector < Color >( img ) ;

  Vector< std::tuple< Image< Color >, Vector< size_t > > > detected = detector->Run();
  LUCOLOR desc ( detected ) ;

  LUCOLORfeature v = desc.Run () ;
  cout<<int(v[0][0])<<endl;

  cout<<v[0][1]<< "\t"<<v[0][2]<<"\t"<<v[0][3]<<endl;
  cout<<v[0][4]<< "\t"<<v[0][5]<<"\t"<<v[0][6]<<endl;

  for(size_t i=7; i<v[0].size(); i+=3)
    cout<<v[0][i]<<"\t"<<v[0][i+1]<<"\t"<<v[0][i+2]<<endl;
  cout<<endl;

  return 1;
}
