#include <iostream>
#include <stdexcept>
#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

#include "DescriptionSS.hpp"

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
  SS desc ( detected ) ;

  SSfeature v = desc.Run () ;
  cout<<v[0].size()<<endl;
  for(size_t i=0; i<v[0].size(); i++)
    cout<<v[0][i]<<endl;

  return 1;
}
