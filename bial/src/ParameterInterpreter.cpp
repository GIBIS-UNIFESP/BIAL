#include "DescriptionParameterInterpreter.hpp"
namespace Bial {
  ParameterInterpreter::ParameterInterpreter( std::string input ) {
    this->input = input;
  }
  void ParameterInterpreter::SetExpectedParameters( parameter_list p ) {
    this->parameters = p;
  }
}
