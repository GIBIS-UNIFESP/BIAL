# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# Date: 2012/Sep/01
# Version: 1.0.00
# Content: Makefile for lsh.
# Description: Defines compilation directives.
# Future add-on's: many.

INC=./inc
LIB=./lib
OBJ=./obj
SRC=./src

BIAL_CC_FLAGS=-Wall -O3 -I./inc -I./src -DREAL_FLOAT

CXX=g++ --std=c++0x
ifeq ($(clang), 1)
	CXX=clang++ --std=c++11
endif

all: liblsh

liblsh: $(LIB)/liblsh.a

# Slowest first
$(LIB)/liblsh.a: \
$(OBJ)/BucketHashing.o \
$(OBJ)/Geometry.o \
$(OBJ)/GlobalVars.o \
$(OBJ)/LocalitySensitiveHashing.o \
$(OBJ)/LSH.o \
$(OBJ)/NearNeighbors.o \
$(OBJ)/Random.o \
$(OBJ)/SelfTuning.o
	ar csr $(LIB)/liblsh.a $(OBJ)/*.o

$(OBJ)/BucketHashing.o: $(SRC)/BucketHashing.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/Geometry.o: $(SRC)/Geometry.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/GlobalVars.o: $(SRC)/GlobalVars.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/LocalitySensitiveHashing.o: $(SRC)/LocalitySensitiveHashing.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/LSH.o: $(SRC)/LSH.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/NearNeighbors.o: $(SRC)/NearNeighbors.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/Random.o: $(SRC)/Random.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/SelfTuning.o: $(SRC)/SelfTuning.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

clean:
	rm -f $(LIB)/lib*.a; rm -f $(OBJ)/*.o; rm -f ./*/*~ ./*/*#* *~ *#
