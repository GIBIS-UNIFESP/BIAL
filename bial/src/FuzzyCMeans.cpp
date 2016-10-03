/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file FuzzyCMeans.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2012/Jun/25 
 * @version 1.0.00
 * @brief
 * Content: FuzzyCMeans class 
 * <br> Description: Implementation of Fuzzy C-Means clustering algorithm
 * for 3D images. 
 */

#ifndef BIALCMEANS_C
#define BIALCMEANS_C

#include "FuzzyCMeans.hpp"

#if defined ( BIAL_EXPLICIT_LIB ) && ( BIAL_FuzzyCMeans )
#define BIAL_EXPLICIT_FuzzyCMeans
#endif
#if defined ( BIAL_EXPLICIT_FuzzyCMeans ) || ( BIAL_IMPLICIT_BIN )

namespace Bial {

  template< class D > FuzzyCMeans< D >::FuzzyCMeans( const Feature< D > &new_feats, int new_clusters, float new_m,
                                                     double new_epsilon, size_t nmax_iterations ) try 
    : feats( new_feats ), centroids( new_clusters, new_feats.Features( ) ),
        membership( new_clusters, new_feats.Elements( ) ), m( new_m ), epsilon( new_epsilon ), 
        max_iterations( nmax_iterations ) {
      UniformCentroidInitialization( );
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
  double FuzzyCMeans< D >::MembershipUpdate( ) {
    try {
      Vector< double > distances( feats.Elements( ) * centroids.Elements( ), 0.0 );
      COMMENT( "Computing mean distances.", 0 );
      for( size_t ctd_a = 0; ctd_a < centroids.Elements( ); ++ctd_a ) {
        for( size_t elm = 0; elm < feats.Elements( ); ++elm ) {
          for( size_t ftr = 0; ftr < feats.Features( ); ++ftr ) {
            distances( elm * centroids.Elements( ) + ctd_a ) += 
              static_cast< double >( ( feats( elm, ftr ) - centroids( ctd_a, ftr ) ) * 
                                     ( feats( elm, ftr ) - centroids( ctd_a, ftr ) ) );
          }
          distances( elm * centroids.Elements( ) + ctd_a ) = sqrt( distances( elm * centroids.Elements( ) + ctd_a ) );
        }
      }
      double max = 0.0;
      COMMENT( "Updating memberships.", 0 );
      for( size_t ctd_a = 0; ctd_a < centroids.Elements( ); ++ctd_a ) {
        double val = 0.0;
        for( size_t elm = 0; elm < feats.Elements( ); ++elm ) {
          if( distances( elm * centroids.Elements( ) + ctd_a ) <= 0.0000001 ) {
            val = 1.0;
          }
          else {
            for( size_t ctd_b = 0; ctd_b < centroids.Elements( ); ++ctd_b ) {
              if( distances( elm * centroids.Elements( ) + ctd_b ) <= 0.0000001 ) {
                val = 9.0e99;
                break;
              }
              val +=
                pow( distances( elm * centroids.Elements( ) + ctd_a ) / 
                     distances( elm * centroids.Elements( ) + ctd_b ), 2.0 / ( m - 1.0 ) );
            }
            val = 1.0 / val;
          }
          if( max < membership( ctd_a, elm ) - val ) {
            max = membership( ctd_a, elm ) - val;
          }
          membership( ctd_a, elm ) = val;
        }
      }
      return( max );
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
  void FuzzyCMeans< D >::CentroidUpdate( ) {
    try {
      Vector< double > sum( feats.Features( ) );
      for( size_t ctd = 0; ctd < centroids.Elements( ); ++ctd ) {
        double sum2 = 0.0;
        for( size_t ftr = 0; ftr < feats.Features( ); ++ftr ) {
          sum( ftr ) = 0.0;
          for( size_t elm = 0; elm < feats.Elements( ); ++elm ) {
            double power = pow( static_cast< double >( membership( ctd, elm ) ), m );
            sum( ftr ) += power * static_cast< double >( feats( elm, ftr ) );
            if( ftr == 0 ) {
              sum2 += power;
            }
          }
          if( ( sum2 > -0.0000001 ) && ( sum2 < 0.0000001 ) ) {
            std::string msg( BIAL_ERROR( "Division by zero while computing centroids." ) );
            throw( std::logic_error( msg ) );
          }
          centroids( ctd, ftr ) = static_cast< D >( sum( ftr ) / sum2 );
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
  }

  template< class D >
  Matrix< double > FuzzyCMeans< D >::Run( bool verbose ) {
    try {
      MembershipUpdate( );
      double max_change = epsilon + 1.0;
      size_t itr = 0;
      for( ; ( max_change > epsilon ) && ( itr < max_iterations ); ++itr ) {
        if( verbose ) {
          PrintIteration( std::cout, itr, max_change );
        }
        CentroidUpdate( );
        max_change = MembershipUpdate( );
      }
      if( verbose ) {
        PrintIteration( std::cout, itr, max_change );
      }
      return( membership );
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
  Matrix< double > FuzzyCMeans< D >::GetMembership( ) const {
    return( membership );
  }

  template< class D >
  void FuzzyCMeans< D >::UniformCentroidInitialization( ) {
    try {
      bool singular = true;
      /* Looking for first valid voxel. */
      for( size_t ftr = 0; ftr < feats.Features( ); ++ftr ) {
        centroids( 0, ftr ) = feats( 0, ftr );
        centroids( centroids.Elements( ) - 1, ftr ) = feats( 0, ftr );
        for( size_t elm = 0; elm < feats.Elements( ); ++elm ) {
          if( centroids( 0, ftr ) > feats( elm, ftr ) ) {
            centroids( 0, ftr ) = feats( elm, ftr );
          }
          if( centroids( centroids.Elements( ) - 1, ftr ) < feats( elm, ftr ) ) {
            centroids( centroids.Elements( ) - 1, ftr ) = feats( elm, ftr );
          }
        }
        if( centroids( 0, ftr ) != centroids( centroids.Elements( ) - 1, ftr ) ) {
          singular = false;
        }
        for( size_t ctd = 1; ctd < centroids.Elements( ) - 1; ++ctd ) {
          centroids( ctd, ftr ) = centroids( 0, ftr ) + ctd *
            ( centroids( centroids.Elements( ) - 1, ftr ) - centroids( 0, ftr ) ) / ( centroids.Elements( ) - 1 );
        }
      }
      if( singular ) {
        std::string msg( BIAL_ERROR( "All feature vectors have the same data." ) );
        throw( std::logic_error( msg ) );
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

  template< class D > template< class O >
  O &FuzzyCMeans< D >::PrintIteration( O &os, int itr, double max_change ) const {
    try {
      os << std::endl << "Iteration " << itr + 1 << " of " << max_iterations << ": change: " << max_change <<
	", epsilon: " << epsilon << std::endl;
      for( size_t ctd = 0; ctd < centroids.Elements( ); ++ctd ) {
        os << "centroid " << ctd << ": ";
        for( size_t ftr = 0; ftr < feats.Features( ); ++ftr ) {
          os << centroids( ctd, ftr ) << " ";
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

#ifdef BIAL_EXPLICIT_FuzzyCMeans

  template FuzzyCMeans< int >::FuzzyCMeans( const Feature< int > &new_feats, int new_clusters, float new_m,
                                                     double new_epsilon, size_t nmax_iterations );
  template double FuzzyCMeans< int >::MembershipUpdate( );
  template void FuzzyCMeans< int >::CentroidUpdate( );
  template Matrix< double > FuzzyCMeans< int >::Run( bool verbose );
  template Matrix< double > FuzzyCMeans< int >::GetMembership( ) const;
  template void FuzzyCMeans< int >::UniformCentroidInitialization( );

  template FuzzyCMeans< llint >::FuzzyCMeans( const Feature< llint > &new_feats, int new_clusters, float new_m,
                                                     double new_epsilon, size_t nmax_iterations );
  template double FuzzyCMeans< llint >::MembershipUpdate( );
  template void FuzzyCMeans< llint >::CentroidUpdate( );
  template Matrix< double > FuzzyCMeans< llint >::Run( bool verbose );
  template Matrix< double > FuzzyCMeans< llint >::GetMembership( ) const;
  template void FuzzyCMeans< llint >::UniformCentroidInitialization( );

  template FuzzyCMeans< float >::FuzzyCMeans( const Feature< float > &new_feats, int new_clusters, float new_m,
                                                     double new_epsilon, size_t nmax_iterations );
  template double FuzzyCMeans< float >::MembershipUpdate( );
  template void FuzzyCMeans< float >::CentroidUpdate( );
  template Matrix< double > FuzzyCMeans< float >::Run( bool verbose );
  template Matrix< double > FuzzyCMeans< float >::GetMembership( ) const;
  template void FuzzyCMeans< float >::UniformCentroidInitialization( );

  template FuzzyCMeans< double >::FuzzyCMeans( const Feature< double > &new_feats, int new_clusters, float new_m,
                                                     double new_epsilon, size_t nmax_iterations );
  template double FuzzyCMeans< double >::MembershipUpdate( );
  template void FuzzyCMeans< double >::CentroidUpdate( );
  template Matrix< double > FuzzyCMeans< double >::Run( bool verbose );
  template Matrix< double > FuzzyCMeans< double >::GetMembership( ) const;
  template void FuzzyCMeans< double >::UniformCentroidInitialization( );

  template OFile&FuzzyCMeans< int >::PrintIteration( OFile&os, int itr, double max_change ) const;
  template OFile&FuzzyCMeans< llint >::PrintIteration( OFile&os, int itr, double max_change ) const;
  template OFile&FuzzyCMeans< float >::PrintIteration( OFile&os, int itr, double max_change ) const;
  template OFile&FuzzyCMeans< double >::PrintIteration( OFile&os, int itr, double max_change ) const;

  template std::ostream &FuzzyCMeans< int >::PrintIteration( std::ostream &os, int itr, double max_change ) const;
  template std::ostream &FuzzyCMeans< llint >::PrintIteration( std::ostream &os, int itr, double max_change ) const;
  template std::ostream &FuzzyCMeans< float >::PrintIteration( std::ostream &os, int itr, double max_change ) const;
  template std::ostream &FuzzyCMeans< double >::PrintIteration( std::ostream &os, int itr, double max_change ) const;

#endif

}

#endif

#endif
