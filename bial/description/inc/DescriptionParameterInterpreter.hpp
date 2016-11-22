#ifndef PARAMTERINTERPRETER_H
#define PARAMTERINTERPRETER_H

#include "Vector.hpp"
#include <string>
#include <tuple>

namespace Bial {

  typedef std::tuple< std::string, double > parameter;
  typedef Vector< parameter > parameter_list;

  class ParameterInterpreter {
protected:
    parameter_list parameters;
    std::string input;

public:
    ParameterInterpreter( std::string input );

    ParameterInterpreter( );

    void SetExpectedParameters( parameter_list p );

    virtual parameter_list Interpret( ) = 0;

    virtual std::string GetExpectedParameters( ) = 0;

  };
}

#endif
