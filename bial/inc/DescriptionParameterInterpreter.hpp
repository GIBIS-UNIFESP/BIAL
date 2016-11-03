#ifndef PARAMTERINTERPRETER_H
#define PARAMTERINTERPRETER_H

#include <string>
#include <tuple>
#include "Vector.hpp"

using namespace std;
using namespace Bial;

typedef tuple<string,double> parameter;
typedef Vector<parameter> parameter_list;

class ParameterInterpreter
{
  protected:
    parameter_list parameters;
    string input;

  public:
    ParameterInterpreter( string input );

    ParameterInterpreter( );

    void SetExpectedParameters( parameter_list p );

    virtual parameter_list Interpret(  ) = 0;

    virtual string GetExpectedParameters(  ) = 0;

};
#endif
