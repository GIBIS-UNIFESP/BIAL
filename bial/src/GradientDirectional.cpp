/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/21 
 * @brief Computes directional gradient. This gradient is the contribution of the intensity differences from all 
 * adjacents. 
 */

#ifndef BIALGRADIENTDIRECTIONAL_C
#define BIALGRADIENTDIRECTIONAL_C

#include "GradientDirectional.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_GradientDirectional )
#define BIAL_EXPLICIT_GradientDirectional
#endif
#if defined ( BIAL_EXPLICIT_GradientDirectional ) || ( BIAL_IMPLICIT_BIN )

#include "Adjacency.hpp"
#include "AdjacencyIterator.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Gradient::Directional( const Image< D > &img, const Adjacency &adj ) {
    try {
      Image< D > grad( img );
      AdjacencyIterator adj_itr( img, adj );
      size_t adj_size = adj.size( );
      size_t img_size = img.size( );
      size_t adj_pxl;
      for( size_t pxl = 0; pxl < img_size; ++pxl ) {
        double sum = 0;
        for( size_t idx = 0; idx < adj_size; ++idx ) {
          if( ( adj_itr.*adj_itr.AdjIdx )( pxl, idx, adj_pxl ) )
            sum += ( img[ pxl ] > img[ adj_pxl ] ) ? img[ pxl ] - img[ adj_pxl ] : img[ adj_pxl ] - img[ pxl ];
        }
        grad[ pxl ] = static_cast< D >( sum );
      }
      return( grad );
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

#ifdef BIAL_EXPLICIT_GradientDirectional

  template Image< int > Gradient::Directional( const Image< int > &img, const Adjacency &adj );
  template Image< llint > Gradient::Directional( const Image< llint > &img, const Adjacency &adj );
  template Image< float > Gradient::Directional( const Image< float > &img, const Adjacency &adj );
  template Image< double > Gradient::Directional( const Image< double > &img, const Adjacency &adj );

#endif

}

#endif

#endif
