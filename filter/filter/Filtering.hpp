/* Biomedical Image Analysis Library */
/* See README file in the root instalation directory for more information. */

/* Date: 2018/Nov/07 */
/* Content: Test file. */
/* Description: Computes sets of Edge and Flat pixels with standard deviation different of zero in their adjacency. */

#include "AdjacencyIterator.hpp"
#include "AdjacencyRound.hpp"
#include "Common.hpp"
#include "DiffusionFunction.hpp"
#include "FilteringMedian.cpp"
#include "Image.hpp"
#include "ImageFrame.hpp"

using namespace std;
using namespace Bial;

#ifndef BIALFILTERING_H
#define BIALFILTERING_H

namespace Bial {

  template< class D >
  bool NoisyPixels( const Image< D > &img, Image< D > &noise_mask );
  
  template< class D >
  void PatchSimilarityMap( Vector< Image< D > > &map, const Image< D > &framed_img, const Vector< size_t > &img_dim,
			   const float search_radius, const float patch_radius, const size_t patch_adjacents,
			   const size_t thread, const size_t total_threads );
  
  Vector< double > NonLocalWeightVector( const Adjacency &adj, const size_t adj_size, const size_t img_dims,
					 const float patch_distance );

  double NonLocalIntegrationConstant( const Adjacency &adj, const size_t adj_size, const size_t img_dims,
				      const size_t patch_adjacents, const float patch_distance );

  template< class D >
  float NonLocalFlatRegionKappa( const Image< D > &source, const Image< D > &mask, const Adjacency &adj,
				 const AdjacencyIterator &adj_itr );

  template< class D >
  void QuickNonLocalAnisotropicDiffusionThread( const Image< D > &img, Image< D > &res,
						const RobustDiffusionFunction &diff_func, float kappa,
						const Adjacency &adj, const AdjacencyIterator &adj_itr,
						const Vector< double > &weight, double integration_constant,
						const Vector< Image< D > > &map, size_t thread, size_t total_threads );

  template< class D >
  Image< D > QuickNonLocalAnisotropicDiffusion( const Image< D > &img, const RobustDiffusionFunction &diff_func,
						float kappa, const Adjacency &adj, const AdjacencyIterator &adj_itr,
						const Vector< double > &weight, double integration_constant,
						const Vector< Image< D > > &map );

  template< class D >
  Image< D > AdaptiveNonLocalAnisotropicDiffusion( Image< D > img, const RobustDiffusionFunction &diff_func,
                           float init_kappa, const Adjacency &adj, float reduction_factor,
						   const AdjacencyIterator &adj_itr, const Vector< double > &weight,
						   double integration_constant, const Vector< Image< D > > &map );


  template< class D >
  Image< D > NonLocalOptimumADFilter( Image< D > img, const RobustDiffusionFunction &diff_func, const Image< D > &backg,
			const float search_radius, const float patch_radius, const float patch_distance,
			const size_t patch_adjacents, const float reduction, const float conservativeness );
  
}

#endif
