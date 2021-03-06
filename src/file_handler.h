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

#pragma once

#include <string>
#include <string>
#include <fstream>
#include <memory>

#include "scene.h"



class File_Handler{
public:
  void process(const std::string& file_name, std::shared_ptr<Scene> scene);
  
private:
  std::string file_content;
  std::size_t file_pos;
  std::size_t line;
  std::shared_ptr<Scene> scene;
  
  void load_file_content(const std::string& file_name);
  void parse_file();
    void parse_scene();
      void parse_scene_name();
      void parse_background();
      void parse_time();
      void parse_object_array();
        void parse_object();
          phy_obj_type parse_object_type();
          glm::vec2 parse_object_position();
          float parse_object_rotation();
          float parse_object_size();
          glm::vec3 parse_object_colour();
          uint parse_object_time();
    std::vector<float> parse_float_array();
  char next_char();
    bool valid_char(char c);
  char next_num();
  std::string next_string();
  float next_float();
  uint next_uint();
  void check_char(char c);
  void check_string(const std::string& string);
  bool optional_check_char(char c);
  bool optional_check_string(const std::string& string);
  bool file_end();
};