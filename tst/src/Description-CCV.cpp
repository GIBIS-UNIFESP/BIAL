#include <iostream>
#include <stdexcept>
#include "Color.hpp"
#include "FileImage.hpp"
#include "Image.hpp"

#include "DescriptionCCV.hpp"

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
  CCV desc ( detected ) ;

  CCVfeature v = desc.Run () ;
  cout<<v[0].size()<<endl;
  for(size_t i=0; i<v[0].size(); i++)
    cout<<v[0][i];
  cout<<endl;

  return 1;
}
