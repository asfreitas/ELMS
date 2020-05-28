# Reference for makefile
# https://stackoverflow.com/questions/32105358/makefile-with-mixed-c-and-c-code%EF%BC%89


CXX = g++
CXXFLAGS =  


CPPSOURCES= base_unit.cpp base_unit_gui.cpp calculations.cpp connect_database.cpp fileio.cpp main.cpp outgoing.cpp parse_incoming.cpp port.cpp utilities.cpp vehicle.cpp


CPPHEADERS=base_unit.h base_unit_gui.h calculations.h connect_database.h fileio.h outgoing.h parse_incoming.h port.h utilities.h vehicle.h

BOOST="C:\Users\win8\Desktop\boost_1_73_0"

MONGOC="C:\mongo-c-driver\include"

MONGOCXX="C:\mongo-cxx-driver\include\bsoncxx\v_noabi"

MONGOCXX2="C:\mongo-cxx-driver\include\mongocxx\v_noabi"

default: base

base: $(CPPSOURCES) $(CPPHEADERS)
	g++ -o ELMS2 -std=c++11 $(CPPSOURCES) $(CPPHEADERS) $(CXXFLAGS) -pthread -I${BOOST} -I${MONGOC} -I${MONGOCXX} -I${MONGOCXX2} 

clean:
	-del mobile.exe