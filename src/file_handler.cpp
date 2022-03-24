/*

MIT License

Copyright (c) 2022 the_green_penguin

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

#include "file_handler.h"

#include <iostream>
#include <sstream>



void File_Handler::process(const std::string& file_name){
  load_file_content(file_name);
}




////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////

void File_Handler::load_file_content(const std::string& file_name){
  // open file
  std::ifstream file;
  file.exceptions(std::ios::failbit | std::ios::badbit);   // without this, no exception will be thrown when ".open()" fails
  try{  file.open(file_name);  }
  catch(std::exception& e){
    std::cerr << "Error: Unable to open file '" + file_name + "'!\n";
    return;
  }
  
  // get file content
  file_content = std::string(
    (std::istreambuf_iterator<char>(file)),
    (std::istreambuf_iterator<char>())
  );
  
  // try parsing it
  try{  parse_file();  }
  catch(std::exception& e){
    std::cerr << "Error: Unable to parse file '" + file_name + "'!\n" + e.what() + "\n";
    return;
  }
}



//------------------------------------------------------------------------------
void File_Handler::parse_file(){
  check_char('{');  
  parse_scene();
  check_char('}');
}



//------------------------------------------------------------------------------
void File_Handler::parse_scene(){
    
  check_string("scene");
  parse_scene_name();
    
  check_string("background");
  parse_background();
    
  check_string("time");
  parse_time();
    
  check_string("objects");
  parse_objects();
}



//------------------------------------------------------------------------------
void File_Handler::parse_scene_name(){
  check_char(':');
  std::cout << next_string() << "\n";
  check_char(',');
}



//------------------------------------------------------------------------------
void File_Handler::parse_background(){
  check_char(':');
  std::cout << next_string() << "\n";
  check_char(',');
}



//------------------------------------------------------------------------------
void File_Handler::parse_time(){
  check_char(':');
  std::cout << next_string() << "\n";
  check_char(',');
}



//------------------------------------------------------------------------------
void File_Handler::parse_objects(){
  check_char(':');
  std::cout << next_string() << "\n";
}



//------------------------------------------------------------------------------
char File_Handler::next_char(){
  char next = file_content.at(file_pos++);
  if(next == '\n')   // this should be 'translated' to be platform independend (?)
    line++;
    
  if( ! valid_char(next) && ! file_end())
    return next_char();
    
  return next;
}



//------------------------------------------------------------------------------
std::string File_Handler::next_string(){
  check_char('"');
  
  std::string string = "";
  while( ! optional_check_char('"'))
    string += next_char();
  
  return string;
}



//------------------------------------------------------------------------------
bool File_Handler::valid_char(char c){
  if(isalnum(c) || c == '"' || c == ':' || c == ',' || c == '.' || c == '{' || c == '}' || c == '[' || c == ']')
    return true;
    
  return false;
}



//------------------------------------------------------------------------------
void File_Handler::check_char(char c){
  char next = next_char();
  if(next != c){
    std::stringstream message;
    message << "Invalid file format! Expected '" << c << "' but found '" << next << "' in line " << line << ".";
    throw std::runtime_error(message.str());
  }
}



//------------------------------------------------------------------------------
void File_Handler::check_string(const std::string& string){
  std::string next = next_string();
  if(next != string){
    std::stringstream message;
    message << "Invalid file format! Expected '" << string << "' but found '" << next << "' in line " << line << ".";
    throw std::runtime_error(message.str());
  }
}



//------------------------------------------------------------------------------
bool File_Handler::optional_check_char(char c){
  // record previous state
  std::size_t tmp_pos = file_pos;
  std::size_t tmp_line = line;
  
  // check
  if(next_char() == c)
    return true;
    
  // revert
  file_pos = tmp_pos;
  line = tmp_line;
  return false;
}



//------------------------------------------------------------------------------
bool File_Handler::optional_check_string(const std::string& string){
  // record previous state
  std::size_t tmp_pos = file_pos;
  std::size_t tmp_line = line;
  
  // check
  if(next_string() == string)
    return true;
  
  // revert
  file_pos = tmp_pos;
  line = tmp_line;
  return false;
}



//------------------------------------------------------------------------------
bool File_Handler::file_end(){
  if(file_pos == file_content.size() - 1)
    return true;
    
  return false;
}