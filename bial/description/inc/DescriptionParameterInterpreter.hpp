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
    const std::string &input;

public:
    ParameterInterpreter( const std::string &input ) : input( input ) {
    }

    ParameterInterpreter( );

    void SetExpectedParameters( const parameter_list &p ) {
      parameters = p;
    }

    virtual parameter_list Interpret( ) = 0;

    virtual std::string GetExpectedParameters( ) = 0;

  };
}

#endif
