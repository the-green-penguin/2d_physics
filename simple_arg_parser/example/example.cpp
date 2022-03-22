// compile with e.g.: g++ example.cpp -o out.run

#include "../simple_arg_parser.h"

#include <iostream>
#include <vector>
#include <string>



void pr_args(const std::vector<std::string>& args){
  for(auto &a : args)
    std::cout << a << ", ";
  std::cout << "\n";
}



int main(int argc, char* argv[]){
  
  SArgParser p;
  SArgParser::opt_id help = p.define_option('h', "help", true);
  SArgParser::opt_id output = p.define_option('o', "output", false);
  SArgParser::opt_id example = p.define_option('x', "", false);
  p.parse(argc, argv);
  
  
  
  std::cout << "Program: " << p.program_name() << "\n";
  std::cout << "With arguments: ";
  pr_args( p.program_args() );
  
  
  
  std::cout << "And options...\n\n";
  
  if(p.found_option(help))
    std::cout << "Help needed.\n";
    
  if(p.found_option(output)){
    std::cout << "Output at: ";
    pr_args( p.option_args(output) );
  }
  
  if(p.found_option(example)){
    std::cout << "Example: ";
    pr_args( p.option_args(example) );
  }
  
  return 0;
}