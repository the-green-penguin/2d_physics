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



void File_Handler::process(const std::string& file_name, std::shared_ptr<Scene> scene){
  this->scene = scene;
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
  file_content += ".";
  
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
  parse_object_array();
}



//------------------------------------------------------------------------------
void File_Handler::parse_scene_name(){
  check_char(':');
  scene->set_name( next_string() );
  check_char(',');
}



//------------------------------------------------------------------------------
void File_Handler::parse_background(){
  check_char(':');
  
  std::vector<float> colour = parse_float_array();
  if(colour.size() != 3)
    throw std::runtime_error("Invalid value for scene background colour.");
  
  scene->set_background_colour( glm::vec3(colour[0], colour[1], colour[2]) );
  
  check_char(',');
}



//------------------------------------------------------------------------------
void File_Handler::parse_time(){
  check_char(':');
  scene->set_time( next_uint() );
  check_char(',');
}



//------------------------------------------------------------------------------
void File_Handler::parse_object_array(){
  check_char(':');
  check_char('[');

  parse_object();
  
  while( ! optional_check_char(']') && ! file_end()){
    check_char(',');
    parse_object();
  }
}



//------------------------------------------------------------------------------
void File_Handler::parse_object(){
  parse_object_type();
  
  check_char(':');
  check_char('{');
  
  check_string("position");
  glm::vec2 pos = parse_object_position();
    
  check_string("rotation");
  float rot = parse_object_rotation();
    
  check_string("size");
  float size = parse_object_size();
    
  check_string("color");
  glm::vec3 colour = parse_object_colour();
  
  check_string("time");
  uint time = parse_object_time();
  
  check_char('}');
  
  scene->add_object(
    PhyObject(pos, rot, size, colour, time)
  );
}



//------------------------------------------------------------------------------
File_Handler::phy_obj_type File_Handler::parse_object_type(){
  if( optional_check_string("triangle") )
    return triangle;
  if( optional_check_string("rectangle") )
    return rectangle;
  if( optional_check_string("circle") )
    return circle;
    
  else{
    std::stringstream message;
    message << "Invalid file format! Expected 'triangle', 'rectangle' or 'circle' in line " << line << ".";
    throw std::runtime_error(message.str());
  }
}



//------------------------------------------------------------------------------
glm::vec2 File_Handler::parse_object_position(){
  check_char(':');
  
  std::vector<float> colour = parse_float_array();
  if(colour.size() != 2){
    std::stringstream message;
    message << "Invalid file format! Expected <[float, float]> after '\"position\":' in line " << line << ".";
    throw std::runtime_error(message.str());
  }

  check_char(',');
  return glm::vec2(colour[0], colour[1]);
}



//------------------------------------------------------------------------------
float File_Handler::parse_object_rotation(){
  check_char(':');
  float ret = next_float();
  check_char(',');
  
  return ret;
}



//------------------------------------------------------------------------------
float File_Handler::parse_object_size(){
  check_char(':');
  float ret = next_float();
  check_char(',');
  
  return ret;
}



//------------------------------------------------------------------------------
glm::vec3 File_Handler::parse_object_colour(){
  check_char(':');
  
  std::vector<float> colour = parse_float_array();
  if(colour.size() != 3){
    std::stringstream message;
    message << "Invalid file format! Expected <[float, float, float]> after '\"color\":' in line " << line << ".";
    throw std::runtime_error(message.str());
  }

  check_char(',');
  return glm::vec3(colour[0], colour[1], colour[2]);
}



//------------------------------------------------------------------------------
uint File_Handler::parse_object_time(){
  check_char(':');
  return next_uint();
}



//------------------------------------------------------------------------------
std::vector<float> File_Handler::parse_float_array(){
  check_char('[');
  std::vector<float> ret;
  
  ret.push_back( next_float() );
  while( ! optional_check_char(']') && ! file_end()){
    check_char(',');
    ret.push_back( next_float() );
  }
  
  return ret;
}



//------------------------------------------------------------------------------
char File_Handler::next_char(){
  // end of file
  file_pos++;
  if( file_end() )
    return file_content.at( (file_pos--) - 1 );
  
  // get next valid char
  char next = file_content.at(file_pos - 1);
  if(next == '\n')   // this should be 'translated' to be platform independend (?)
    line++;
    
  if( ! valid_char(next))
    return next_char();
        
  return next;
}



//------------------------------------------------------------------------------
std::string File_Handler::next_string(){
  check_char('"');
  
  std::string string = "";
  while( ! optional_check_char('"') && ! file_end())
    string += next_char();
  
  return string;
}



//------------------------------------------------------------------------------
float File_Handler::next_float(){
  // get substring
  std::string tmp = "";
  float ret = 0.0f;
  while( ! optional_check_char('f') && tmp.size() < 20 && ! file_end())
    tmp += next_char();
  
  // handle invalid input
  try{  ret = std::stof(tmp);  }
  catch(std::exception& e){
    std::stringstream message;
    message << "Invalid file format! Expected <float> but found '" << tmp << "' in line " << line << ".";
    throw std::runtime_error(message.str());
  }
  
  // result
  return ret;
}



//------------------------------------------------------------------------------
uint File_Handler::next_uint(){
  // get substring
  std::string tmp = "";
  uint ret = 0;
  while( ! optional_check_char(',') && ! optional_check_char('}') && ! optional_check_char(']') && tmp.size() < 20 && ! file_end())
    tmp += next_char();
  
  // handle invalid input
  try{  ret = std::stoul(tmp);  }
  catch(std::exception& e){
    std::stringstream message;
    message << "Invalid file format! Expected <uint> but found '" << tmp << "' in line " << line << ".";
    throw std::runtime_error(message.str());
  }
  
  // result
  file_pos--;   // revert from optional_check
  return ret;
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
  if(file_pos >= file_content.size())
    return true;
    
  return false;
}