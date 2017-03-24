# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Author: Fábio Augusto Menocci Cappabianco
# Date: 2012/Sep/01
# Version: 1.0.00
# Content: Makefile for slic.
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

all: libslic

libslic: $(LIB)/libslic.a

# Slowest first
$(LIB)/libslic.a: \
$(OBJ)/SLIC.o
	ar csr $(LIB)/libslic.a $(OBJ)/*.o

$(OBJ)/SLIC.o: $(SRC)/SLIC.cpp
	$(CXX) $(BIAL_CC_FLAGS) -c -o $(@) $(SRC)/$(basename $(notdir $(@))).cpp

clean:
	rm -f $(LIB)/lib*.a; rm -f $(OBJ)/*.o; rm -f ./*/*~ ./*/*#* *~ *#
