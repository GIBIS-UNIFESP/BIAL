#include "DescriptionCGI.hpp"

namespace Bial {

  CGI::CGI( std::string input ) : ParameterInterpreter( input ) {
  }

  CGI::CGI( ) : ParameterInterpreter( "" ) {
  }


  parameter CGI::InterpretParameter( std::string par ) {
    std::string name;
    double value;
    size_t match;

    match = par.find_first_of( '=' );
    if( match == std::string::npos ) {
      throw "error: invalid parameter";
    }
    name = par.substr( 0, match );
    value = stod( par.substr( match + 1, std::string::npos ) );

    return( std::tie( name, value ) );
  }

  void CGI::ChangeParameterValue( parameter p ) {
    std::string name1, name2;
    double value;
    for( size_t i = 0; i < this->parameters.size( ); i++ ) {
      std::tie( name1, std::ignore ) = this->parameters[ i ];
      std::tie( name2, value ) = p;
      if( name1 == name2 ) {
        this->parameters[ i ] = std::tie( name2, value );
      }
    }
  }

  parameter_list CGI::Interpret( ) {
    /* "altura=4.2&tamanho=7" */
    std::string par, name, in;

    size_t match;

    in = input;
    do {
      match = in.find_first_of( '&' );
      par = in.substr( 0, match );

      this->ChangeParameterValue( InterpretParameter( par ) );

      in = in.substr( match + 1, std::string::npos );

    } while( match != std::string::npos );
    return( this->parameters );
  }

  std::string CGI::GetExpectedParameters( ) {
    std::string output, name;
    double value;
    for( size_t i = 0; i < parameters.size( ); i++ ) {
      if( i != 0 ) {
        output += "&";
      }
      std::tie( name, value ) = parameters[ i ];
      output += name + "=" + std::to_string( value );
    }
    return( output );
  }

}
