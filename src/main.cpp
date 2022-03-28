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

#include <iostream>
#include "../simple_arg_parser/simple_arg_parser.h"
#include "app.h"



int main(int argc, char* argv[]){
	
	// parse CLI options
	SArgParser parser;
	SArgParser::opt_id help = parser.define_option('h', "help", true);
	
	try{  parser.parse(argc, argv);  }
	catch(std::exception& e){
		std::cerr << "Error: Incorrect CLI argument: " << e.what() << "\n";
		return -1;
	}
	
	// handle CLI options
	App app;
	if(parser.found_option(help))
		app.print_help();
		
	else{
		// run program
		try{  app.run(parser.program_args());  }
		catch(std::exception& e){
			std::cerr << "Error: " << e.what() << "\n";
		}
	}
	
	return 0;
}