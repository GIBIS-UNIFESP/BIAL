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

  Vector< double > Filtering::WeightVector( const Adjacency &adj, size_t adj_size, size_t img_dims ) {
    try {
      COMMENT( "Creating weight applied to each adjacent pixel based to its relative position to the filtered pixel.", 2 );
      Vector< double > weight( adj_size, 0 );
      for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	COMMENT( "Computing L2 distance.", 4 );
	double distance = 0.0;
	for( size_t dim = 0; dim < img_dims; ++dim )
	  distance += adj( adj_idx, dim ) * adj( adj_idx, dim );
	weight[ adj_idx ] = 1.0 / std::sqrt( distance );
      }
      return( weight );
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

  double Filtering::IntegrationConstant( const Adjacency &adj, size_t adj_size, size_t img_dims ) {
    try {
      COMMENT( "Computing integration constant.", 2 );
      double integration_constant = 0.0;
      for( size_t idx = 1; idx < adj_size; ++idx ) {
        double distance = 0.0;
        for( size_t dim = 0; dim < img_dims; ++dim )
          distance += std::abs( adj( idx, dim ) );
        integration_constant += 1.0 / distance;
      }
      integration_constant = 1.0 / integration_constant;
      return( integration_constant );
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
  Image< D > Filtering::AdaptiveAnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func,
                                                      float init_kappa, float radius ) {
    try {
      COMMENT( "Avoid flitering flat image.", 1 );
      if( init_kappa <= 10.0 )
        return( Image< D >( img ) );
      COMMENT( "Initial filtering with kappa: " << init_kappa, 1 );
      Image< D > res = Filtering::AnisotropicDiffusion( img, diff_func, init_kappa, 1, radius ); // Change this for the quick format.
      COMMENT( "Reducing kappa.", 1 );
      float kappa = init_kappa - diff_func->Reduction( init_kappa );
      COMMENT( "Filtering until low value of kappa is reached.", 1 );
      while( kappa > 10.0 ) {
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
  Image< D > Filtering::QuickAnisotropicDiffusion( const Image< D > &img, const Vector< size_t > &mask,
						   const DiffusionFunction *diff_func, float kappa,
						   const Adjacency &adj, const AdjacencyIterator &adj_itr,
						   const Vector< double > &weight, double integration_constant ) {
    try {
      Image< D > res( img );
      size_t adj_size = adj.size( );
      size_t mask_size = mask.size( );
      COMMENT( "Computing diffusion filter.", 2 );
      size_t adj_pxl;
      for( size_t src_pxl = 0; src_pxl < mask_size; ++src_pxl ) {
	size_t pxl = mask[ src_pxl ];
	COMMENT( "Computing intensity flow from adjacents.", 4 );
	double flow = 0.0;
	for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	  if( adj_itr.AdjIdx( pxl, adj_idx, adj_pxl ) ) {
	    D grad = img[ adj_pxl ] - img[ pxl ];
	    flow += weight[ adj_idx ] * grad * ( *diff_func )( kappa, grad );
	  }
	}
	COMMENT( "Updating pixel intensity with adjacent flow.", 4 );
	res[ pxl ] = img[ pxl ] + integration_constant * flow;
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
  
  /* Main diffusion function */
  template< class D >
  Image< D > Filtering::AnisotropicDiffusion( Image< D > img, const DiffusionFunction *diff_func, float kappa,
                                              size_t iterations, float radius ) {
    try {
      Image< D > res( img );
      Adjacency adj( AdjacencyType::HyperSpheric( radius, img.Dims( ) ) );
      size_t adj_size = adj.size( );
      size_t img_dims = img.Dims( );
      COMMENT( "Creating weight applied to each adjacent pixel based to its relative position to the filtered pixel.", 2 );
      Vector< double > weight( Filtering::WeightVector( adj, adj_size, img_dims ) );
      COMMENT( "Computing integration constant.", 2 );
      double integration_constant = Filtering::IntegrationConstant( adj, adj_size, img_dims );
      COMMENT( "Computing diffusion filter.", 2 );
      for( size_t itr = 0; itr < iterations; ++itr ) {
	try {
          size_t total_threads = 12;
          Vector< std::thread > threads;
          for( size_t thd = 0; thd < total_threads; ++thd ) {
            threads.push_back( std::thread( &AnisotropicDiffusionThread< D >, std::ref( img ), std::ref( res ),
                                            weight, integration_constant, diff_func, kappa, std::ref( adj ), thd,
                                            total_threads ) );
          }
          for( size_t thd = 0; thd < total_threads; ++thd ) {
            threads( thd ).join( );
          }
        }
        catch( std::exception &e ) {
          BIAL_WARNING( "Failed to run in multi-thread. Exception: " << e.what( ) );
          Filtering::AnisotropicDiffusionThread( img, res, weight, integration_constant, diff_func, kappa, adj, 0, 1 );
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
  void Filtering::AnisotropicDiffusionThread( Image< D > &img, Image< D > &res, const Vector< double > &weight,
					      double integration_constant, const DiffusionFunction *diff_func,
					      float kappa, Adjacency &adj, size_t thread, size_t total_threads ) {
    try {
      COMMENT( "Dealing with thread limits.", 3 );
      size_t min_index = thread * img.Size( ) / total_threads;
      size_t max_index = ( thread + 1 ) * img.Size( ) / total_threads;
      AdjacencyIterator adj_itr( img, adj );
      size_t adj_size = adj.size( );
      size_t adj_pxl;
      for( size_t pxl = min_index; pxl < max_index; ++pxl ) {
	COMMENT( "Computing intensity flow from adjacents.", 4 );
	double flow = 0.0;
	for( size_t adj_idx = 1; adj_idx < adj_size; ++adj_idx ) {
	  if( adj_itr.AdjIdx( pxl, adj_idx, adj_pxl ) ) {
	    D grad = img[ adj_pxl ] - img[ pxl ];
	    flow += weight[ adj_idx ] * grad * ( *diff_func )( kappa, grad );
	  }
	}
	COMMENT( "Updating pixel intensity with adjacent flow.", 4 );
	res[ pxl ] = img[ pxl ] + integration_constant * flow;
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
  }

#ifdef BIAL_EXPLICIT_FilteringAnisotropicDiffusion

  template Image< int > Filtering::AdaptiveAnisotropicDiffusion( Image< int > img, const DiffusionFunction *diff_func,
                                                                 float init_kappa, float radius );
  template Image< int > Filtering::AnisotropicDiffusion( Image< int > img, const DiffusionFunction *diff_func,
                                                         float kappa, size_t iterations, float radius );
  template void Filtering::AnisotropicDiffusionThread( Image< int > &img, Image< int > &res,
						       const Vector< double > &weight,
                                                       double integration_constant,
                                                       const DiffusionFunction *diff_func, float kappa,
                                                       Adjacency &adj, size_t thread, size_t total_threads );

  template Image< llint > Filtering::AdaptiveAnisotropicDiffusion( Image< llint > img, const DiffusionFunction *diff_func,
                                                               float init_kappa, float radius );
  template Image< llint > Filtering::AnisotropicDiffusion( Image< llint > img, const DiffusionFunction *diff_func,
                                                       float kappa, size_t iterations, float radius );
  template void Filtering::AnisotropicDiffusionThread( Image< llint > &img, Image< llint > &res,
						       const Vector< double > &weight,
                                                       double integration_constant,
                                                       const DiffusionFunction *diff_func, float kappa,
                                                       Adjacency &adj, size_t thread, size_t total_threads );

  template Image< float > Filtering::AdaptiveAnisotropicDiffusion( Image< float > img, const DiffusionFunction *diff_func,
                                                               float init_kappa, float radius );
  template Image< float > Filtering::AnisotropicDiffusion( Image< float > img, const DiffusionFunction *diff_func,
                                                       float kappa, size_t iterations, float radius );
  template void Filtering::AnisotropicDiffusionThread( Image< float > &img, Image< float > &res,
						       const Vector< double > &weight,
                                                       double integration_constant,
                                                       const DiffusionFunction *diff_func, float kappa,
                                                       Adjacency &adj, size_t thread, size_t total_threads );

  template Image< double > Filtering::AdaptiveAnisotropicDiffusion( Image< double > img, const DiffusionFunction *diff_func,
                                                               float init_kappa, float radius );
  template Image< double > Filtering::AnisotropicDiffusion( Image< double > img, const DiffusionFunction *diff_func,
                                                       float kappa, size_t iterations, float radius );
  template void Filtering::AnisotropicDiffusionThread( Image< double > &img, Image< double > &res,
						       const Vector< double > &weight,
                                                       double integration_constant,
                                                       const DiffusionFunction *diff_func, float kappa,
                                                       Adjacency &adj, size_t thread, size_t total_threads );

  template Image< int > Filtering::QuickAnisotropicDiffusion( const Image< int > &img, const Vector< size_t > &mask,
							      const DiffusionFunction *diff_func, float kappa,
							      const Adjacency &adj, const AdjacencyIterator &adj_itr,
							      const Vector< double > & weight, double integration_constant );

  template Image< llint > Filtering::QuickAnisotropicDiffusion( const Image< llint > &img, const Vector< size_t > &mask,
								const DiffusionFunction *diff_func, float kappa,
								const Adjacency &adj, const AdjacencyIterator &adj_itr,
								const Vector< double > & weight, double integration_constant );
  
  template Image< float > Filtering::QuickAnisotropicDiffusion( const Image< float > &img, const Vector< size_t > &mask,
								const DiffusionFunction *diff_func, float kappa,
								const Adjacency &adj, const AdjacencyIterator &adj_itr,
								const Vector< double > & weight, double integration_constant );

  template Image< double > Filtering::QuickAnisotropicDiffusion( const Image< double > &img, const Vector< size_t > &mask,
								 const DiffusionFunction *diff_func, float kappa,
								 const Adjacency &adj, const AdjacencyIterator &adj_itr,
								 const Vector< double > & weight, double integration_constant );

#endif

}


#endif

#endif
