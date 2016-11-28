# Biomedical Image Analysis Library
# See README file in the root instalation directory for more information.

# Version: 1.0.00
# Content: Makefile for testing files.

debug ?= 0
verbose ?= 0
test ?= 0
clang ?= 0
implicit ?= 0
profile ?= 0
#ccache=$(shell ccache --version 2>/dev/null)

LIB=../build/linux/release/lib
OBJ=../build/linux/release/obj
ifneq ($(debug), 0)
	OBJ=../build/linux/debug/obj
	LIB=../build/linux/debug/lib
endif

BIAL_LINK_FLAGS= -L$(LIB) -lbial -L../bial/SLIC/obj/* -L../bial/cpplex/obj/* -L../bial/lsh/obj/* -fopenmp -lpthread -lz -lm
ifneq ($(profile), 0)
	BIAL_LINK_FLAGS=-L$(LIB) -lbial -L../bial/SLIC/obj/* -L../bial/cpplex/obj/* -L../bial/lsh/obj/* -lpthread -lz -lm -lprofiler
endif

BIAL_CC_FLAGS=-Wall -Wno-unused-function -Wno-literal-conversion -I../bial/inc -I../bial/src -I../bial/SLIC/inc -I../bial/cpplex/inc -I../bial/lsh/inc -DREAL_FLOAT -O3
ifneq ($(verbose), 0)
	BIAL_CC_FLAGS=-Wall -Wno-unused-function -Wno-literal-conversion -I../bial/inc -I../bial/src -I../bial/SLIC/inc -I../bial/cpplex/inc -I../bial/lsh/inc -DREAL_FLOAT -O3 -DBIAL_DEBUG=$(verbose)
endif
ifneq ($(debug), 0)
	BIAL_CC_FLAGS=-g -Wno-unused-function -Wno-literal-conversion -O0 -I../bial/inc -I../bial/src -I../bial/SLIC/inc -I../bial/cpplex/inc -I../bial/lsh/inc -DREAL_FLOAT -DBIAL_DEBUG=$(debug)
endif
ifneq ($(test), 0)
	BIAL_CC_FLAGS=-g -Wno-unused-function -Wno-literal-conversion -O0 -I../bial/inc -I../bial/src -I../bial/SLIC/inc -I../bial/cpplex/inc -I../bial/lsh/inc -DREAL_FLOAT
endif

ifeq ($(debug), 1)
	QMAKE_PARAMS=../master.pro -r -spec linux-g++ CONFIG+=debug -o ../Makefile
else
	QMAKE_PARAMS=../master.pro -r -spec linux-g++ CONFIG+=release -o ../Makefile
endif

CXX=g++ -Wl,--no-as-needed --std=c++0x
ifdef ccache
	ifeq ($(clang), 1)
		CXX=ccache clang++ -Wl,--no-as-needed --std=c++0x -Qunused-arguments
	else
		CXX=ccache g++ -Wl,--no-as-needed --std=c++0x
	endif
else
	ifeq ($(clang), 1)
		CXX=clang++ -Wl,--no-as-needed --std=c++0x -Qunused-arguments
	else
		CXX=g++ -Wl,--no-as-needed --std=c++0x
	endif
endif

BIAL_SRC=../bial/src/*.cpp
SRC=./src
BIN=./bin

all: Adjacency Bit Brain Clustering Color DataSet Draw Edge Feature File Filtering Gradient Geometrics Heart Hough Image ImageInterpolation Insert-Inhomogeneity Kernel Lungs MarchingCubes Matrix MRI OPF Plate PNM Relaxometria Segmentation Signal Sorting Statistics Table Transform Vector

libbial:
	export LD_LIBRARY_PATH=$(LIB)
#	rm -f $(OBJ)/Common.o
#	qmake-qt5 $(QMAKE_PARAMS)
#	$(MAKE) -C ../ --no-print-directory


Matrix-Gradient: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)


Adjacency: Adjacency-Displacement Adjacency-Gray

Adjacency-Displacement: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Adjacency-Gray: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Bit: Bit-Invert Bit-Operations

Bit-Invert: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Bit-Operations: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Brain: Brain-CMeansClustering Brain-OPFClustering Brain-COG Brain-InhomogeneityCorrection Brain-IntensityCuts Brain-TissueThresholds Brain-SplitOPFClustering Brain-HeadTopSlice

Brain-CMeansClustering: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Brain-OPFClustering: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Brain-COG: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Brain-InhomogeneityCorrection: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Brain-IntensityCuts: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Brain-TissueThresholds: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Brain-SplitOPFClustering: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Brain-HeadTopSlice: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Clustering: Clustering-OPFDistance

Clustering-OPFDistance: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Color: Color-CMeansClustering Color-RGBtoHSI

Color-CMeansClustering: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Color-RGBtoHSI: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



DataSet: DataSet-Validate

DataSet-Validate: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Draw: Drawing Draw-Circle Draw-Line Draw-Line-3D Draw-SVGForest Draw-SVGDGraph

Drawing: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Draw-Circle: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Draw-Line: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Draw-Line-3D: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Draw-SVGForest: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Draw-SVGDGraph: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)


Edge: Edge-Trace Edge-Trace2D LazyWalk LiveWire RiverBed

Edge-Trace: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Edge-Trace2D: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

LazyWalk: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

LiveWire: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

RiverBed: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

# SuperPixel-RiverBed: libbial
# 	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Feature: Feature-Read Feature-Write

Feature-Read: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Feature-Write: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



File: File-ReadDir

File-ReadDir: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Filtering: Filtering-Anisotropic Filtering-Gaussian Filtering-Mean Filtering-Median Filtering-OptimalAnisotropic

Filtering-AdaptiveAnisotropic: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Filtering-Anisotropic: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Filtering-Gaussian: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Filtering-Median: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Filtering-Mean: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Filtering-OptimalAnisotropic: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)


Gradient: Gradient-AutoCanny Gradient-Canny Gradient-DirectionalSobel Gradient-Gabor Gradient-HoleClosing Gradient-Morphological Gradient-MultiScaleCanny Gradient-MultiSubScaleCanny Gradient-ScaleCanny Gradient-Sobel Gradient-SuppressedSobel

Gradient-AutoCanny: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-Canny: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-DirectionalSobel: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-Gabor: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-HoleClosing: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-Morphological: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-MultiScaleCanny: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-MultiScaleSizeCanny: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-MultiSubScaleCanny: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-ScaleCanny: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-Sobel: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Gradient-SuppressedSobel: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)


Geometrics: AffineTransform-2D AffineTransform-3D

AffineTransform-2D: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

AffineTransform-3D: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)


Heart: Heart-COG Heart-Segmentation Heart-SequencyLocation

Heart-COG: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Heart-Segmentation: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Heart-SequencyLocation: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Hough: Hough-Circles

Hough-Circles: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image: Image-AND Image-BinaryMask Image-Copy Image-CMeansClustering Image-Compare Image-Convert Image-CutHorizontal Image-Diff Image-Dilate Image-Equalize Image-Erode Image-Flip Image-GetSlice Image-MinimumSpanningForest Image-Merge Image-Print Image-ROI Image-Rotate_90 Image-Size Image-Sum

Image-AND: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-BinaryMask: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-CMeansClustering: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Compare: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Convert: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Copy: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-CutHorizontal: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Diff: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Dilate: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Equalize: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Erode: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Flip: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-GetSlice: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-MinimumSpanningForest: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-IFTWatershed: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Merge: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Multiply: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-MultiplyMask: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Print: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-ROI: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Rotate_90: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Size: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Image-Sum: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



ImageInterpolation: ImageInterpolation-Isometric

ImageInterpolation-Isometric: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Insert-Inhomogeneity: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Kernel: Kernel-Gabor Kernel-Gaussian Kernel-NormalizedGaussian Kernel-NormalizedSobel Kernel-Sobel

Kernel-Gabor: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Kernel-Gaussian: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Kernel-NormalizedGaussian: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Kernel-NormalizedSobel: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Kernel-Sobel: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Lungs: Lungs-Segmentation Lungs-Segmentation2

Lungs-Segmentation: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Lungs-Segmentation2: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)


MarchingCubes: MarchingCubes-Binary MarchingCubes-Clean MarchingCubes-ConvertToSTL

MarchingCubes-Binary: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

MarchingCubes-Clean: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

MarchingCubes-ConvertToSTL: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Matrix: Matrix-3DCompare Matrix-Cofactor Matrix-Determinant Matrix-Exceptions Matrix-Invert Matrix-Move Matrix-Multiplication Matrix-Read Matrix-Read_Write Matrix-Sum Matrix-Scalars

Matrix-3DCompare: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Cofactor: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Determinant: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Exceptions: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Invert: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Move: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Multiplication: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Read: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Read_Write: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Sum: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Scalars: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Matrix-Time: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



MRI: MRI-CopyNiftiHeader MRI-Dimensions MRI-NiftiHeader MRI-Orientation MRI-SetOffset MRI-SetNiftiHeader MRI-Threshold

MRI-CopyNiftiHeader: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

MRI-Dimensions: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

MRI-NiftiHeader: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

MRI-Orientation: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

MRI-SetOffset: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

MRI-SetNiftiHeader: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

MRI-Threshold: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



MultiImage: MultiImage-IO

MultiImage-IO: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



OPF: OPF-FeatureClustering OPF-Hierarchical OPF-ImageHierarchical OPF-KClustering OPF-LabelMatching OPF-LSH

OPF-FeatureClustering: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

OPF-Hierarchical: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

OPF-ImageHierarchical: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

OPF-KClustering: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

OPF-LabelMatching: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

OPF-LSH: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)


OPF-LSH_minimal: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Plate: Plate-Finder Plate-Graphics Plate-Statistics

Plate-Finder: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Plate-Graphics: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Plate-Statistics: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



PNM: PNM-Read PNM-Write PNM-Rotation PNM-Sum PNM-Otsu PNM-Color

PNM-Read: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

PNM-Write: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

PNM-Rotation: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

PNM-Sum: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

PNM-Otsu: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

PNM-Color: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Relaxometria: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Segmentation: Segmentation-Background Segmentation-ConnectedComponents Segmentation-ConvertSeeds Segmentation-Disconnected Segmentation-OrientedExternGeodesicStar Segmentation-Octants Segmentation-OrientedInternGeodesicStar Segmentation-Quadrants Segmentation-Threshold Segmentation-Watershed

Segmentation-Background: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-ConnectedComponents: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-ConvertSeeds: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-Disconnected: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-Octants: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-OrientedExternGeodesicStar: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-OrientedInternGeodesicStar: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-Quadrants: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-Threshold: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Segmentation-Watershed: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Signal: Signal-Equalize Signal-Histogram Signal-Normalize

Signal-Equalize: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Signal-Histogram: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Signal-Normalize: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Sorting: Sorting-BucketQueue Sorting-BucketQueueMax Sorting-InfBucketQueue Sorting-RandomQueueMedians Sorting-QuickIndex

Sorting-BucketQueue: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Sorting-BucketQueueMax: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Sorting-InfBucketQueue: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Sorting-RandomQueueMedians: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Sorting-QuickIndex: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



# SuperPixel: SuperPixel-Image SuperPixel-Brain

# SuperPixel-Brain: libbial
# 	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

# SuperPixel-Image: libbial
# 	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Statistics: Statistics-Accuracy Statistics-BorderValidate Statistics-EdgeCompareBaddeley Statistics-MAD Statistics-MultiClassLabeling

Statistics-Accuracy: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Statistics-BorderValidate: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Statistics-EdgeCompareBaddeley:libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Statistics-MAD: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Statistics-MultiClassLabeling: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Table: Table-Print

Table-Print: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Transform: Transform-Euclidean Transform-InverseEuclidean

Transform-Euclidean: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)

Transform-InverseEuclidean: libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)



Vector: Vector-Basic

Vector-Basic:libbial
	$(CXX) $(BIAL_CC_FLAGS) -o $(BIN)/$@ $(SRC)/$(@).cpp $(BIAL_LINK_FLAGS)


clean:
	rm -f src/*~ bin/* *~
	rm -rf dat/*
	$(MAKE) clean -C ../ --no-print-directory
