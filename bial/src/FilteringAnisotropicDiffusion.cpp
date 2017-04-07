/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Jul/12 
 * @brief Anisotropic diffusion filtering to remove high-frequency noise.
 */

#ifndef BIALFILTERINGANISOTROPICDIFFUSION_C
#define BIALFILTERINGANISOTROPICDIFFUSION_C

#include "FilteringAnisotropicDiffusion.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FilteringAnisotropicDiffusion )
#define BIAL_EXPLICIT_FilteringAnisotropicDiffusion
#endif

#if defined ( BIAL_EXPLICIT_FilteringAnisotropicDiffusion ) || ( BIAL_IMPLICIT_BIN )

#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "DiffusionFunction.hpp"
#ifdef BIAL_DEBUG
#include "FileImage.hpp"
#endif
#include "Image.hpp"

namespace Bial {

  template< class D >
  Image< D > Filtering::AdaptiveAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func,
                                                      float init_kappa, float radius ) {
    try {
      COMMENT( "Avoid flitering flat image.", 1 );
      if( init_kappa <= 50.0 ) {
        return( Image< D >( img ) );
      }
      COMMENT( "Initial filtering with kappa: " << init_kappa, 1 );
      Image< D > res = Filtering::AnisotropicDiffusion( img, diff_func, init_kappa, 1, radius );
      COMMENT( "Reducing kappa.", 1 );
      float kappa = init_kappa - diff_func->Reduction( init_kappa );
      COMMENT( "Filtering until low value of kappa is reached.", 1 );
      while( kappa > 50.0 ) {
        COMMENT( "New kappa: " << kappa, 2 );
        std::swap( img, res );
        res = Filtering::AnisotropicDiffusion( img, diff_func, kappa, 1, radius );
        kappa -= diff_func->Reduction( kappa );
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
  Image< D > Filtering::AnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float kappa,
                                              size_t iterations, float radius ) {
    try {
      Image< D > res( img );
      Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );
      size_t adj_size = adj.size( );
      COMMENT( "Computing integration constant.", 2 );
      double integration_constant = 0.0;
      for( size_t idx = 1; idx < adj_size; ++idx ) {
        double distance = 0.0;
        for( size_t dim = 0; dim < img.Dims( ); ++dim )
          distance += std::abs( adj( idx, dim ) );
        integration_constant += 1.0 / distance;
      }
      integration_constant = 1.0 / integration_constant;
      for( size_t itr = 0; itr < iterations; ++itr ) {
        COMMENT( "Computing diffusion filter.", 2 );
        try {
          size_t total_threads = 12;
          Vector< std::thread > threads;
          for( size_t thd = 0; thd < total_threads; ++thd ) {
            threads.push_back( std::thread( &AnisotropicDiffusionThread< D >, std::ref( img ), std::ref( res ),
                                            integration_constant, diff_func, kappa, std::ref( adj ), thd,
                                            total_threads ) );
          }
          for( size_t thd = 0; thd < total_threads; ++thd ) {
            threads( thd ).join( );
          }
        }
        catch( std::exception &e ) {
          BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
          Filtering::AnisotropicDiffusionThread( img, res, integration_constant, diff_func, kappa, adj, 0, 1 );
        }
        COMMENT( "Updating image.", 2 );
        std::swap( img, res );
      }
      return( img );
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
  void Filtering::AnisotropicDiffusionThread( Image< D > &img, Image< D > &res, double integration_constant,
                                              const DiffusionFunction *diff_func, float kappa, Adjacency &adj,
                                              size_t thread, size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 3 );
      size_t min_index = thread * img.Size( ) / total_threads;
      size_t max_index = ( thread + 1 ) * img.Size( ) / total_threads;
      for( size_t img_index = min_index; img_index < max_index; ++img_index )
        res[ img_index ] = img[ img_index ] + integration_constant *
          Filtering::Flow( img, diff_func, kappa, img_index, adj );
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
  double Filtering::Flow( const Image< D > &img, const DiffusionFunction *diff_func, float kappa,
                          const size_t pxl, const Adjacency &adj ) {
    try {
      double flow = 0.0;
      AdjacencyIterator adj_itr( img, adj );
      size_t adj_size = adj.size( );
      size_t img_dims = img.Dims( );
      size_t adj_pxl;
      for( size_t idx = 1; idx < adj_size; ++idx ) {
        double distance = 0.0;
        if( adj_itr.AdjIdx( pxl, idx, adj_pxl ) ) {
          COMMENT( "Computing distance.", 4 );
          for( size_t dim = 0; dim < img_dims; ++dim )
            distance += pow( adj( idx, dim ), 2.0 );
          COMMENT( "Computing flow.", 4 );
          if( distance != 0.0 ) {
            double weight = 1.0 / distance;
            D grad = img[ adj_pxl ] - img[ pxl ];
            flow += weight * grad * ( *diff_func )( kappa, grad );
          }
        }
      }
      return( flow );
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

#ifdef BIAL_EXPLICIT_FilteringAnisotropicDiffusion

  template Image< int > Filtering::AdaptiveAnisotropicDiffusion( Image< int > img, const DiffusionFunction *diff_func,
                                                                 float init_kappa, float radius );
  template Image< int > Filtering::AnisotropicDiffusion( Image< int > img, const DiffusionFunction *diff_func,
                                                         float kappa, size_t iterations, float radius );
  template void Filtering::AnisotropicDiffusionThread( Image< int > &img, Image< int > &res, 
                                                       double integration_constant,
                                                       const DiffusionFunction *diff_func, float kappa,
                                                       Adjacency &adj, size_t thread, size_t total_threads );
  template double Filtering::Flow( const Image< int > &img, const DiffusionFunction *diff_func,
                                   float kappa, const size_t pxl, const Adjacency &adj );

  template Image< llint > Filtering::AdaptiveAnisotropicDiffusion( Image< llint > img, const DiffusionFunction *diff_func,
                                                               float init_kappa, float radius );
  template Image< llint > Filtering::AnisotropicDiffusion( Image< llint > img, const DiffusionFunction *diff_func,
                                                       float kappa, size_t iterations, float radius );
  template void Filtering::AnisotropicDiffusionThread( Image< llint > &img, Image< llint > &res,
                                                       double integration_constant,
                                                       const DiffusionFunction *diff_func, float kappa,
                                                       Adjacency &adj, size_t thread, size_t total_threads );
  template double Filtering::Flow( const Image< llint > &img, const DiffusionFunction *diff_func,
                                   float kappa, const size_t pxl, const Adjacency &adj );

  template Image< float > Filtering::AdaptiveAnisotropicDiffusion( Image< float > img, const DiffusionFunction *diff_func,
                                                               float init_kappa, float radius );
  template Image< float > Filtering::AnisotropicDiffusion( Image< float > img, const DiffusionFunction *diff_func,
                                                       float kappa, size_t iterations, float radius );
  template void Filtering::AnisotropicDiffusionThread( Image< float > &img, Image< float > &res,
                                                       double integration_constant,
                                                       const DiffusionFunction *diff_func, float kappa,
                                                       Adjacency &adj, size_t thread, size_t total_threads );
  template double Filtering::Flow( const Image< float > &img, const DiffusionFunction *diff_func,
                                   float kappa, const size_t pxl, const Adjacency &adj );

  template Image< double > Filtering::AdaptiveAnisotropicDiffusion( Image< double > img, const DiffusionFunction *diff_func,
                                                               float init_kappa, float radius );
  template Image< double > Filtering::AnisotropicDiffusion( Image< double > img, const DiffusionFunction *diff_func,
                                                       float kappa, size_t iterations, float radius );
  template void Filtering::AnisotropicDiffusionThread( Image< double > &img, Image< double > &res,
                                                       double integration_constant,
                                                       const DiffusionFunction *diff_func, float kappa,
                                                       Adjacency &adj, size_t thread, size_t total_threads );
  template double Filtering::Flow( const Image< double > &img, const DiffusionFunction *diff_func,
                                   float kappa, const size_t pxl, const Adjacency &adj );

#endif

}


#endif

#endif
