namespace Bial {

  template< class T >
  NoDetector< T >::NoDetector( Image< T > img ) : FeatureDetector< T >( img ) {
  }

  template< class T >
  Vector < tuple < Image< T >, Image< int >> > NoDetector< T >::Run( ) {
    Vector < tuple < Image< T >, Image< int >> > v;
    tuple < Image< T >, Image < int >> t;
    Image< int > mask( this->img.size( 0 ), this->img.size( 1 ) );
    for( size_t i = 0; i < mask.size( ); ++i ) {
      mask[ i ] = 1;
    }
    t = tie( this->img, mask );
    v.push_back( t );

    return( v );
  }

}
