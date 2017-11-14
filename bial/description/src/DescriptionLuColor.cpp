#include "DescriptionLuColor.hpp"
#include "Signal.hpp"

namespace Bial {

  LUCOLOR::LUCOLOR( FeatureDetector< Color > *Fd ) try : LUCOLOR( Fd->Run( ) ) {
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

  LUCOLOR::LUCOLOR( const Vector< std::tuple< Image< Color >, Vector< size_t > > > &detected ) try :
    FeatureExtractor< Color, double >( detected ) {
    this->dim = 4;
    this->qtd_blocos = 100;
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

  void LUCOLOR::SetParameters( ParameterInterpreter *interpreter ) {
    try {
      std::string temp;
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      vet.push_back( std::tie( "qtd_blocos", qtd_blocos ) );
      interpreter->SetExpectedParameters( vet );
      vet = interpreter->Interpret( );
      std::tie( std::ignore, dim ) = vet[ 0 ];
      std::tie( std::ignore, qtd_blocos ) = vet[ 1 ];
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

  std::string LUCOLOR::GetParameters( ParameterInterpreter *interpreter ) {
    try {
      Vector< parameter > vet;
      vet.push_back( std::tie( "dim", dim ) );
      vet.push_back( std::tie( "qtd_blocos", qtd_blocos ) );
      interpreter->SetExpectedParameters( vet );
      return( interpreter->GetExpectedParameters( ) );
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

  LUCOLORfeature LUCOLOR::Run( ) {
    try {
      size_t size = 3 * this->qtd_blocos;
      LUCOLORfeature feat;

      for( size_t i = 0; i < this->detected.size( ); ++i ) {
        const Image< Color > &img( std::get< 0 >( detected[ i ] ) );
        const Vector< size_t > &mask( std::get< 1 >( detected[ i ] ) );
        size_t mask_size = mask.size( );
        if( mask_size == 0 ) {
          std::string msg( BIAL_ERROR( "Empty mask. " ) );
          throw( std::logic_error( msg ) );
        }
        size_t img_size = img.size( );
        Vector< bool > mask_vct( img_size, false );
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx )
          mask_vct[ mask[ pxl_idx ] ] = true;
        size_t nelem = img_size / qtd_blocos;
        size_t resto = img_size % qtd_blocos;
        COMMENT( "Calcula media das bandas R, G, B--------------------------------------", 3 );
        long double somaR = 0;
        long double somaG = 0;
        long double somaB = 0;
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t pxl = mask[ pxl_idx ];
          somaR += img[ pxl ].channel[ 1 ];
          somaG += img[ pxl ].channel[ 2 ];
          somaB += img[ pxl ].channel[ 3 ];
        }
        double mediaR = ( double ) somaR / mask_size;
        double mediaG = ( double ) somaG / mask_size;
        double mediaB = ( double ) somaB / mask_size;
        COMMENT( "Calculo do desvio padrão das bandas R, G, B--------------------------------------", 3 );
        somaR = 0;
        somaG = 0;
        somaB = 0;
        for( size_t pxl_idx = 0; pxl_idx < mask_size; ++pxl_idx ) {
          size_t pxl = mask[ pxl_idx ];
          somaR += std::pow( img[ pxl ].channel[ 1 ] - mediaR, 2 );
          somaG += std::pow( img[ pxl ].channel[ 2 ] - mediaG, 2 );
          somaB += std::pow( img[ pxl ].channel[ 3 ] - mediaB, 2 );
        }
        double desvioR = std::sqrt( ( somaR / ( mask_size - 1 ) ) );
        double desvioG = std::sqrt( ( somaG / ( mask_size - 1 ) ) );
        double desvioB = std::sqrt( ( somaB / ( mask_size - 1 ) ) );
        COMMENT( "Calculo das medias dos blocos--------------------------------------", 3 );
        Vector< double > mediaBlocosR( qtd_blocos );
        Vector< double > mediaBlocosG( qtd_blocos );
        Vector< double > mediaBlocosB( qtd_blocos );
        int aux = 0;
        for( size_t j = 0; j < qtd_blocos; ++j ) {
          somaR = 0;
          somaG = 0;
          somaB = 0;
          size_t size_soma = 0;
          if( j != qtd_blocos - 1 ) { /* senao for o ultimo bloco pq ele pode ter menos elementos */
            for( size_t k = aux; k < ( aux + nelem ); ++k ) {
              if( mask_vct[ k ] ) {
                somaR += img[ k ].channel[ 1 ];
                somaG += img[ k ].channel[ 2 ];
                somaB += img[ k ].channel[ 3 ];
                size_soma++;
              }
            }
            mediaBlocosR[ j ] = static_cast< double >( somaR / size_soma );
            mediaBlocosG[ j ] = static_cast< double >( somaG / size_soma );
            mediaBlocosB[ j ] = static_cast< double >( somaB / size_soma );
          }
          else {
            if( resto == 0 )
              size_soma = nelem;
            for( size_t k = aux; k < img_size; ++k ) {
              if( mask_vct[ k ] ) { /* percorrer até final da imagem */
                somaR += img[ k ].channel[ 1 ];
                somaG += img[ k ].channel[ 2 ];
                somaB += img[ k ].channel[ 3 ];
                size_soma++;
              }
            }
            mediaBlocosR[ j ] = static_cast< double >( somaR / size_soma );
            mediaBlocosG[ j ] = static_cast< double >( somaG / size_soma );
            mediaBlocosB[ j ] = static_cast< double >( somaB / size_soma );
          }
          aux = aux + nelem;
        }
        COMMENT( "Histograma------------------------------------------------", 3 );
        Features< double > histogram( size + 7 );
        histogram[ 0 ] = qtd_blocos;
        histogram[ 1 ] = mediaR;
        histogram[ 2 ] = mediaG;
        histogram[ 3 ] = mediaB;
        histogram[ 4 ] = desvioR;
        histogram[ 5 ] = desvioG;
        histogram[ 6 ] = desvioB;
        for( size_t j = 0; j < qtd_blocos; j++ ) {
          if( mediaBlocosR[ j ] > mediaR ) /* se media do blocoR e maior que a media da imagemR */
            histogram[ 7 + 3 * j ] = 1;
          else
            histogram[ 7 + 3 * j ] = 0;
          if( mediaBlocosG[ j ] > mediaG ) /* se media do blocoG e maior que a media da imagemG */
            histogram[ 7 + 3 * j + 1 ] = 1;
          else
            histogram[ 7 + 3 * j + 1 ] = 0;
          if( mediaBlocosB[ j ] > mediaB ) /* se media do blocoB e maior que a media da imagemB */
            histogram[ 7 + 3 * j + 2 ] = 1;
          else
            histogram[ 7 + 3 * j + 2 ] = 0;
        }
        feat.push_back( histogram );
      }
      return( feat );
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

}
