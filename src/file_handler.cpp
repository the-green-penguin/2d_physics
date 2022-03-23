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
#include <fstream>



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
}



//------------------------------------------------------------------------------
void File_Handler::parse(){
  
}