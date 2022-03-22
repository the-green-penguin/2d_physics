/*

MIT License

Copyright (c) 2021 the_green_penguin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


*/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <exception>
#include <limits>



class SArgParser{

public:
  
  typedef unsigned int opt_id;
  
  
  
//------------------------------------------------------------------------------

  opt_id define_option(char short_name, const std::string& long_name, bool is_flag){
    opt_id new_id = defined_options.size();
    std::string s_name_str(1, short_name);
    
    defined_options.push_back({s_name_str, long_name, is_flag});
    
    return new_id;
  }
  
  
  
//------------------------------------------------------------------------------
  
  void parse(int argc, char* argv[]){
    // convert to std types
    for(int i = 1; i < argc; i++)  // ignore [0] (program name)
      input.push_back(argv[i]);
      
    // process
    pgr_name = argv[0];
    parse_input();
  }
  
  
  
//------------------------------------------------------------------------------

  bool found_option(opt_id id){
    auto search = options.find(id);
    if(search != options.end())
      return true;
      
    return false;
  }
  
  
  
//------------------------------------------------------------------------------

  std::string program_name(){
    return pgr_name;
  }
  
  
  
//------------------------------------------------------------------------------

  std::vector<std::string> program_args(){
    return options[program_id];
  }
  
  
  
//------------------------------------------------------------------------------

  std::vector<std::string> option_args(opt_id id){
    // only access map if ID was found (would create entry for non-existing key otherwise)
    auto search = options.find(id);
    if(search != options.end())
      return options[id];
      
    return {};
  }
  
  
  
////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////

private:
  
  typedef struct{
    std::string short_name;
    std::string long_name;
    bool is_flag;
  } option;
  
  std::vector<option> defined_options;
  std::vector<std::string> input;
  opt_id program_id = std::numeric_limits<unsigned int>::max();  // non-option arguments are stored under this ID
  std::unordered_map< opt_id, std::vector<std::string> > options = { {program_id, {}} };
  std::string pgr_name = "";
  opt_id curr_option = program_id;  // if != program_id -> short option awaiting argument
  
  
  
//------------------------------------------------------------------------------

  void parse_input(){
    bool stop_parsing_options = false;
    
    for(auto &i : input){
      
      // special arguments
      if(i == "-"){
        options[program_id].push_back("-");
        continue;
      }
      if(i == "--"){
        stop_parsing_options = true;
        continue;
      }
      
      // option(s)
      if( ! stop_parsing_options && i.size() > 0 && i[0] == '-')
        parse_option(i);
        
      // non-option
      else{
        options[curr_option].push_back(i);
        curr_option = program_id;
      }
    }
    
    // handle last short option
    if(curr_option != program_id)
      error("Option '" + defined_options[curr_option].short_name + "' expects an argument.\n");
  }
  
  
  
//------------------------------------------------------------------------------

  void parse_option(const std::string& opt){
    // handle previous option
    if(curr_option != program_id)
      error("Option '" + defined_options[curr_option].short_name + "' expects an argument.\n");
    
    // process current option
    if(opt.size() > 1 && opt[1] == '-')
      parse_option_long(opt);
      
    else
      parse_option_short(opt);
  }
  
  
  
//------------------------------------------------------------------------------

  void parse_option_long(const std::string& opt){
    std::string option_name = "";
    std::string option_argument = "";
    
    bool has_arg = split_long_option(opt, option_name, option_argument);
    opt_id id = get_option_id(option_name);
    
    if(id == program_id)
      error("Found undefined option '" + option_name + "'.\n");
      
    if(defined_options[id].is_flag && has_arg)
      error("Option '" + option_name + "' does not expect an argument.\n");
      
    if( ! defined_options[id].is_flag && ! has_arg)
      error("Option '" + option_name + "' expects an argument.\n");
      
    options[id].push_back(option_argument);
  }
  
  
  
//------------------------------------------------------------------------------

  bool split_long_option(const std::string& opt, std::string& o_name, std::string& o_argument){
    bool found_arg = false;
    
    for(unsigned int i = 2; i < opt.size(); i++){  // ignore "--"
      // extract name
      if( ! found_arg){
        if(opt[i] == '='){
          found_arg = true;
          continue;
        }
        o_name += opt[i];
      }
      // extract argument
      else
        o_argument += opt[i];
    }
    
    return found_arg;
  }
  
  
  
//------------------------------------------------------------------------------

  void parse_option_short(const std::string& opt){
    opt_id id = get_option_id( std::string(1, opt[1]) );
    
    if(id == program_id)
      error("Found undefined option(s) '" + opt + "'.\n");
      
    if(defined_options[id].is_flag)
      parse_flag_list(opt);  // '-abc' = '-a -b -c'
      
    else
      parse_option_short_arg(opt, id);  // '-abc' = '-a bc'
  }
  
  
  
//------------------------------------------------------------------------------

  void parse_flag_list(const std::string& opt){
    for(unsigned int i = 1; i < opt.size(); i++){
      // get id & store with empty arg list
      std::string opt_name(1, opt[i]);
      opt_id id = get_option_id( opt_name );
      options[id] = {};
    }
  }
  
  
  
//------------------------------------------------------------------------------

  void parse_option_short_arg(const std::string& opt, opt_id id){
    // '-oArgument'
    if(opt.size() > 2){
      std::string arg = "";
      for(unsigned int i = 2; i < opt.size(); i++){
        arg += opt[i];
      }
      
      options[id].push_back(arg);
    }
    
    // '-o argument'
    else
      curr_option = id;
  }
  
  
  
//------------------------------------------------------------------------------

  opt_id get_option_id(const std::string& option_name){    
    for(unsigned int i = 0; i < defined_options.size(); i++){
      if(defined_options[i].short_name == option_name)
        return i;
        
      if(defined_options[i].long_name == option_name)
        return i;
    }
        
    return program_id;  // not found
  }
  
  
  
//------------------------------------------------------------------------------

  void error(const std::string& message){
    options.clear();
    throw std::runtime_error(message);
  }
  
  
  
};
