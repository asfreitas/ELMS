# Reference for makefile
# https://stackoverflow.com/questions/32105358/makefile-with-mixed-c-and-c-code%EF%BC%89


CXX = gcc
CXXFLAGS =  -Wall -pedantic


CPPSOURCES=Mobile.c
CPPSOURCES+=serial.c

CPPHEADERS=serialport.h

default: base

base: $(CPPSOURCES) $(CPPHEADERS)
	gcc -o mobile $(CPPSOURCES) $(CPPHEADERS) $(CXXFLAGS)

clean:
	-del mobile.exe