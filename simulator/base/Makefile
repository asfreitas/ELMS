# Reference for makefile
# https://stackoverflow.com/questions/32105358/makefile-with-mixed-c-and-c-code%EF%BC%89


CXX = g++
CXXFLAGS =  -Wall -pedantic


CPPSOURCES=main.cpp
CPPSOURCES+=vehicle.cpp
CPPSOURCES+=vector_utilities.cpp
CPPSOURCES+=serial.cpp

CPPHEADERS=vehicle.hpp
CPPHEADERS+=vector_utilities.hpp
CPPHEADER+=serialport.hpp

default: base

base: $(CPPSOURCES) $(CPPHEADERS)
	g++ -o base $(CPPSOURCES) $(CPPHEADERS) $(CXXFLAGS)

clean:
	-rm base