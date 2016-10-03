/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Feature vector class. It is essentialy a matrix where, the first dimention refers to the feature and
 * the other dimension to the element.
 * Example: Using constructor Feature( 4, 4 ) the vector will be allocated as follows: f_0_0, f_0_1, f_0_2, f_0_3,
 * f_1_0, f_1_1, ..., where f_i_j is the feature i of sample j. 
 */

#ifndef BIALFEATURE_C
#define BIALFEATURE_C

#include "Feature.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_Feature )
#define BIAL_EXPLICIT_Feature
#endif

#if defined ( BIAL_EXPLICIT_Feature ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "Color.hpp"
#include "Image.hpp"
#include "Sample.hpp"

namespace Bial {

  template< class D > Feature< D >::Feature( size_t elements, size_t features ) try : 
    feature( features, elements ), index( elements ), label( elements ), nlabels( 0 ) {
    }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D > template< class D2 > 
  Feature< D >::Feature( const Feature< D2 > &feat, const Sample &sample ) try 
    : Feature( sample.size( ), feat.feature.size( 0 ) ) {
      
      COMMENT( "Checking if both feature vectors have sufficient elements required by sample vector.", 1 );
      if( feature.size( 1 ) < sample.size( ) ) {
        std::string msg( BIAL_ERROR( "Requesting for more samples than the original feature vector contains. Original: "
                                     + std::to_string( feature.size( 1 ) ) + ". Requested: " +
                                     std::to_string( sample.size( ) ) + "." ) );
        throw( std::logic_error( msg ) );
      }
      COMMENT( "Assigning data.", 0 );
      for( size_t tgt_elm = 0; tgt_elm < sample.size( ); ++tgt_elm ) {
        size_t src_elm = sample[ tgt_elm ];
        index( tgt_elm ) = feat.index( src_elm );
        label( tgt_elm ) = feat.label( src_elm );
        for( size_t ftr = 0; ftr < feature.size( 0 ); ++ftr ) {
          feature( ftr, tgt_elm ) = static_cast< D >( feat.feature( ftr, src_elm ) );
        }
      }
    }
  catch( std::bad_alloc &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( std::runtime_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
    throw( std::runtime_error( msg ) );
  }
  catch( const std::out_of_range &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
    throw( std::out_of_range( msg ) );
  }
  catch( const std::logic_error &e ) {
    std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
    throw( std::logic_error( msg ) );
  }

  template< class D >
  Vector< D > Feature< D >::operator[]( size_t elm ) const {
    try {
      Vector< D > res( feature.size( 0 ) );
      for( size_t ftr = 0; ftr < feature.size( 0 ); ++ftr ) {
        res( ftr ) = feature( ftr, elm );
      }
      return( res );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  Vector< D > Feature< D >::operator()( size_t elm ) const {
    try {
      return( ( *this )[ elm ] );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  void Feature< D >::operator()( size_t elm, Vector< D > ftr_vct ) {
    try {
      memcpy( &feature( 0, elm ), &ftr_vct[ 0 ], ftr_vct.size( ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  const D &Feature< D >::operator()( size_t elm, size_t ftr ) const {
    try {
      return( feature( ftr, elm ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  D &Feature< D >::operator()( size_t elm, size_t ftr ) {
    try {
      return( feature( ftr, elm ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t Feature< D >::Elements( ) const {
    try {
      return( feature.size( 1 ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t Feature< D >::size( ) const {
    try {
      return( feature.size( ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t Feature< D >::Size( ) const {
    try {
      return( feature.size( ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  const Vector< size_t > &Feature< D >::Index( ) const {
    return( index );
  }

  template< class D >
  Vector< size_t > &Feature< D >::Index( ) {
    return( index );
  }

  template< class D >
  const size_t &Feature< D >::Index( size_t elm ) const {
    try {
      return( index( elm ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t &Feature< D >::Index( size_t elm ) {
    try {
      return( index( elm ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  const Vector< int > &Feature< D >::Label( ) const {
    return( label );
  }

  template< class D >
  Vector< int > &Feature< D >::Label( ) {
    return( label );
  }

  template< class D >
  const int &Feature< D >::Label( size_t elm ) const {
    try {
      return( label( elm ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  int &Feature< D >::Label( size_t elm ) {
    try {
      return( label( elm ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  size_t Feature< D >::Labels( ) const {
    return( nlabels );
  }

  template< class D >
  void Feature< D >::Labels( size_t max ) {
    nlabels = max;
  }

  template< class D >
  size_t Feature< D >::Features( ) const {
    try {
      return( feature.size( 0 ) );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

  template< class D >
  const Matrix< D > &Feature< D >::FeatureVector( ) const {
    return( feature );
  }

  template< class D >
  Matrix< D > &Feature< D >::FeatureVector( ) {
    return( feature );
  }

  template< class D >
  const D*Feature< D >::data( ) const {
    return( feature.data( ) );
  }

  template< class D >
  D*Feature< D >::data( ) {
    return( feature.data( ) );
  }

  template< class D >
  template< class O >
  O &Feature< D >::Print( O &os ) const {
    try {
      for( size_t elm = 0; elm < feature.size( 1 ); ++elm ) {
        os << elm;
        os << "(" << index( elm ) << ")";
        os << " lbl[ " << label( elm ) << " ]: ";
        for( size_t ftr = 0; ftr < feature.size( 0 ); ++ftr ) {
          os << feature( ftr, elm );
          if( ftr != feature.size( 0 ) - 1 ) {
            os << ", ";
          }
        }
        os << std::endl;
      }
      return( os );
    }
    catch( std::bad_alloc &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Memory allocation error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( std::runtime_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Runtime error." ) );
      throw( std::runtime_error( msg ) );
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }
  }

#ifdef BIAL_EXPLICIT_Feature

  template class Feature< int >;
  template class Feature< llint >;
  template class Feature< float >;
  template class Feature< double >;

  template Feature< int >::Feature( const Feature< char > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< char > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< char > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< char > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< uchar > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< uchar > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< uchar > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< uchar > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< short > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< short > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< short > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< short > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< int > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< int > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< int > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< int > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< uint > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< uint > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< uint > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< uint > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< ulint > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< ulint > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< ulint > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< ulint > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< llint > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< llint > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< llint > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< llint > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< ullint > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< ullint > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< ullint > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< ullint > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< float > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< float > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< float > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< float > &feat, const Sample &sample );

  template Feature< int >::Feature( const Feature< double > &feat, const Sample &sample );
  template Feature< llint >::Feature( const Feature< double > &feat, const Sample &sample );
  template Feature< float >::Feature( const Feature< double > &feat, const Sample &sample );
  template Feature< double >::Feature( const Feature< double > &feat, const Sample &sample );

  template std::ostream &Feature< int >::Print( std::ostream &os ) const;
  template std::ostream &Feature< llint >::Print( std::ostream &os ) const;
  template std::ostream &Feature< float >::Print( std::ostream &os ) const;
  template std::ostream &Feature< double >::Print( std::ostream &os ) const;

  template OFile &Feature< int >::Print( OFile &os ) const;
  template OFile &Feature< llint >::Print( OFile &os ) const;
  template OFile &Feature< float >::Print( OFile &os ) const;
  template OFile &Feature< double >::Print( OFile &os ) const;

#endif

}

#endif

#endif
