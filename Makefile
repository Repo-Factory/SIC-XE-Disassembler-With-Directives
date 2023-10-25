# CXX Make variable for compiler
CXX=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = byte_operations.o input_handler.o instructions.o output_handler.o parser.o  main.o
HEADERS = byte_operations.hpp input_handler.hpp instructions.hpp output_handler.hpp parser.hpp
# Program name
PROGRAM = disassem

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
$(PROGRAM) : $(OBJS) $(HEADERS)
	$(CXX) -o $(PROGRAM) $^

byte_operations.o : byte_operations.hpp byte_operations.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) byte_operations.cpp
	
input_handler.o : input_handler.hpp input_handler.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) input_handler.cpp
	
instructions.o : instructions.hpp instructions.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) instructions.cpp
	
output_handler.o : output_handler.hpp output_handler.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) output_handler.cpp

parser.o : parser.hpp parser.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) parser.cpp

main.o : byte_operations.cpp instructions.cpp output_handler.cpp input_handler.cpp main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) main.cpp
	
clean :
	rm -f *.o $(PROGRAM)
