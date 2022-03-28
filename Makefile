# MIT License
# 
# Copyright (c) 2022 the_green_penguin
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.



# compiler
CC = g++

# executable
EXE = ./bin/2d_physics.exe

# recursively find all .cpp files and get their '.o' name
SRC = $(shell find ./src/ -type f -name '*.cpp')
OBJ = $(patsubst %.cpp, %.o, $(SRC) )

# flags
CPP_V = -std=c++2a
DEBUG_FLAGS = -g -Wall -Wextra -pedantic -fPIC -pthread -O0
RELEASE_FLAGS = -fPIC -pthread -O3
CFLAGS = $(DEBUG_FLAGS)

# includes & libraries
INC = 
STA_LIB = -Lsimple_2d_graphics/bin/ -Wl,-Bstatic -lsimple_2d 
DYN_LIB = -Wl,-Bdynamic -lGL -lglfw -lGLEW
LIB = $(STA_LIB) $(DYN_LIB)

# all flags
FLAGS = $(INC) $(LIB) $(CFLAGS) $(CPP_V)



# actual compilation
# " $@ : $< "
all: rm-exe sub-make $(EXE)
	
# remove old .exe
rm-exe:
	rm -f ./bin/*.exe

# link object files -> executable
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(FLAGS)

# create object files
./src/%.o: ./src/%.cpp
	$(CC) -c $< -o $@ $(FLAGS)



# input arguments for 'make'
.PHONY: new clean release sub-make

new: clean all

clean:
	rm -f $(OBJ) ./bin/*.exe

release: CFLAGS = $(RELEASE_FLAGS)
release: clean all

sub-make:
	$(MAKE) -C simple_2d_graphics MAKEFLAGS=