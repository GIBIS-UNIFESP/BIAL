# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# Date: 2012/Sep/01
# Version: 1.0.00
# Content: Makefile for cpplex.
# Description: Defines compilation directives.
# Future add-on's: many.

INC=./inc
LIB=./lib
OBJ=./obj
SRC=./src

BIAL_CC_FLAGS=-Wall -O3 -I./inc -I./src

CXX=g++ --std=c++0x
ifeq ($(clang), 1)
	CXX=clang++ --std=c++11
endif

all: libcpplex

libcpplex: $(LIB)/libcpplex.a

# Slowest first
$(LIB)/libcpplex.a: \
$(OBJ)/columnset.o \
$(OBJ)/constraint.o \
$(OBJ)/matrix.o \
$(OBJ)/objectivefunction.o \
$(OBJ)/simplex.o \
$(OBJ)/variable.o
	ar csr $(LIB)/libcpplex.a $(OBJ)/*.o

$(OBJ)/columnset.o: $(SRC)/columnset.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/constraint.o: $(SRC)/constraint.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/matrix.o: $(SRC)/matrix.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/objectivefunction.o: $(SRC)/objectivefunction.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/simplex.o: $(SRC)/simplex.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

$(OBJ)/variable.o: $(SRC)/variable.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

clean:
	rm -f $(LIB)/lib*.a; rm -f $(OBJ)/*.o; rm -f ./*/*~ ./*/*#* *~ *#
