#include "CGI.h"

CGI::CGI( string input ) : ParameterInterpreter( input ){};

CGI::CGI( ) : ParameterInterpreter( "" ){};


parameter CGI::InterpretParameter(string par)
{
  string name;
  double value;
  size_t match;

  match = par.find_first_of('=');

  if(match==string::npos)
    throw "error: invalid parameter";

  name = par.substr(0,match);
  value = stod( par.substr(match+1 , string::npos) );

  return tie(name,value);
}

void CGI::ChangeParameterValue( parameter p )
{
  string name1, name2;
  double value;

  for(size_t i = 0 ; i<this->parameters.size() ; i++)
  {
    tie(name1, ignore) = this->parameters[i];
    tie(name2, value) = p;
    if(name1 == name2)
      this->parameters[i] = tie(name2, value);
  }
}

parameter_list CGI::Interpret(  )
{
  //"altura=4.2&tamanho=7"
  string par,name,in;

  size_t match;

  in = input;

  do
  {
    match = in.find_first_of('&');
    par = in.substr(0, match);

    this->ChangeParameterValue( InterpretParameter(par) );

    in = in.substr(match+1 , string::npos);

  }while(match!=string::npos);

  return this->parameters;
}

string CGI::GetExpectedParameters(  )
{
  string output, name;
  double value;

  for (size_t i = 0; i < parameters.size( ); i++)
  {
    if(i!=0)
      output+="&";

    tie(name,value) = parameters[i];
    output += name + "=" + to_string(value);
  }

  return output;
}
