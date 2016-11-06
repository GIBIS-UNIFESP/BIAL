namespace Bial {

  template< class T >
  Features< T >::Features( ) {
  }

  template< class T >
  Features< T >::Features( size_t n ) {
    this->item.clear( ); this->item.resize( n );
  }

  template< class T >
  T &Features< T >::operator[]( size_t i ) {
    if( i < item.size( ) ) {
      return( item[ i ] );
    }
    else { std::cout << " erro posição:" << i << std::endl; throw "error"; }
  }

  template< class T >
  const T &Features< T >::operator[]( size_t i ) const {
    if( i < item.size( ) ) {
      return( item[ i ] );
    }
    else {
      cout << item.size( ) << " " << i << endl;
      throw "error";
    }
  }

  template< class T >
  size_t Features< T >::size( ) const {
    return( item.size( ) );
  }

  template< class T >
  void Features< T >::clear( ) {
    item.clear( );
  }

  template< class T >
  void Features< T >::push_back( T i ) {
    item.push_back( i );
  }

  template< class T >
  void Features< T >::resize( size_t n ) {
    item.resize( n );
  }

  template< class T >
  T*Features< T >::data( ) {
    return( item.data( ) );
  }

}
