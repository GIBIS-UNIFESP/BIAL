#include <cstdio>
#include <iostream>
#include <stdexcept>
#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

#include "DescriptionBAS.hpp"

using namespace std;
using namespace Bial;

int main( int argc, char **argv ) {

  if( argc < 2 ) {
    cout << "Usage: " << argv[ 0 ] << " <input image>" << endl;
    exit( -1 );
  }

  Image < int > img = Read< int >( argv[ 1 ] ) ;

  FeatureDetector < int >* detector = new NoDetector < int >( img ) ;

  Vector< std::tuple< Image< int >, Vector< size_t > > > detected = detector->Run();
  BAS desc ( detected ) ;

  BASfeature v = desc.Run () ;
  cout<<v[0].size()<<endl;

  cout.precision(6);
  for(size_t i=0; i<v[0].size(); i++)
    printf("%f\n", v[0][i]);

  return 1;
}
