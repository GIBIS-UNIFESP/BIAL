#include "DescriptionParameterInterpreter.hpp"

#ifndef CGI_H
#define CGI_H

namespace Bial {
  class CGI : public ParameterInterpreter {
private:
    static parameter InterpretParameter( std::string par );
    void ChangeParameterValue( parameter p );

public:
    CGI( std::string input );
    CGI( );
    parameter_list Interpret( );
    std::string GetExpectedParameters( );

  };
}
#endif
