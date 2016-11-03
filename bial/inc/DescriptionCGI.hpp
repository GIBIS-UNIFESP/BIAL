#ifndef CGI_H
#define CGI_H

#include "ParameterInterpreter.h"

class CGI : public ParameterInterpreter
{
  private:
    static parameter InterpretParameter( string par );
    void ChangeParameterValue( parameter p );

  public:
    CGI( string input );
    CGI(  );
    parameter_list Interpret(  );
    string GetExpectedParameters(  );

};

#endif
