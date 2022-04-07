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

#include "app.h"

#include <iostream>



void App::print_help(){
  std::cout
    << "2D Physics v0.1\n"
    << "Usage: 2d_physics [options] [files]\n"
    << "\n"
    << "2D Physics is a simplistic two-dimensional physics simulation. "
    << "It's main purpose was to act as a learning opportunity for the author. "
    << "The program is started from the terminal and will then open a second, interactable window with a minimalistic graphical user interface.\n"
    << "Predefined scenes are loaded from files and will be executed consecutively.\n"
    << "Close the window of a scene to start the next scene.\n"
    << "\n"
    << "Options:\n"
    << "  -h, --help: Displays this message.\n"
    << "\n";
}



//------------------------------------------------------------------------------
void App::run(const std::vector< std::string >& file_names){  
  for(auto &f : file_names){
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    file_handler.process(f, scene);
    scene->start();
  }
}



////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////
