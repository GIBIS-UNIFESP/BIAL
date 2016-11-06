#include "ParameterInterpreter.hpp"

ParameterInterpreter::ParameterInterpreter( string input ) {
  this->input = input;
}
void ParameterInterpreter::SetExpectedParameters( parameter_list p ) {
  this->parameters = p;
}
