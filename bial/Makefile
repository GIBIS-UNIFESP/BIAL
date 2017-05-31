#############################################################################
# Makefile for building: libbial.so.1.0.0
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DBIAL_EXPLICIT_LIB
CFLAGS        = -pipe -O2 -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -Wno-unused-function -Wno-unused-parameter -I./inc -I./src -I./lsh/inc -I./SLIC/inc -I./description/inc -I./cpplex/inc -I./zlib -DREAL_FLOAT -DBIAL_$(basename $(notdir $(@))) -fopenmp -std=c++11 -DBIAL_EXPLICIT_LIB -fopenmp -std=c++11 -O3 -std=gnu++11 -Wall -W -fPIC $(DEFINES)
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
DISTNAME      = bial1.0.0
LINK          = g++
LFLAGS        = -std=c++11 -pg -lm -fopenmp -std=c++11 -Wl,-O1 -shared -Wl,-soname,libbial.so.1
LIBS          = $(SUBLIBS) -lm 
AR            = ar cqs
RANLIB        = 
SED           = sed
STRIP         = strip

####### Output directory

OBJECTS_DIR   = ../build/linux/release/obj/

####### Files

SOURCES       = cpplex/src/columnset.cpp \
		cpplex/src/constraint.cpp \
		cpplex/src/matrix.cpp \
		cpplex/src/objectivefunction.cpp \
		cpplex/src/simplex.cpp \
		cpplex/src/variable.cpp \
		lsh/src/BucketHashing.cpp \
		lsh/src/Geometry.cpp \
		lsh/src/GlobalVars.cpp \
		lsh/src/LocalitySensitiveHashing.cpp \
		lsh/src/LSH.cpp \
		lsh/src/NearNeighbors.cpp \
		lsh/src/Random.cpp \
		lsh/src/SelfTuning.cpp \
		SLIC/src/SLIC.cpp \
		zlib/adler32.c \
		zlib/compress.c \
		zlib/crc32.c \
		zlib/deflate.c \
		zlib/gzclose.c \
		zlib/gzlib.c \
		zlib/gzread.c \
		zlib/gzwrite.c \
		zlib/infback.c \
		zlib/inffast.c \
		zlib/inflate.c \
		zlib/inftrees.c \
		zlib/trees.c \
		zlib/uncompr.c \
		zlib/zutil.c \
		description/src/DescriptionACC.cpp \
		description/src/DescriptionBAS.cpp \
		description/src/DescriptionBIC.cpp \
		description/src/DescriptionBoxCounting.cpp \
		description/src/DescriptionCCH.cpp \
		description/src/DescriptionCCV.cpp \
		description/src/DescriptionCGI.cpp \
		description/src/DescriptionCLD.cpp \
		description/src/DescriptionCSD.cpp \
		description/src/DescriptionEHD.cpp \
		description/src/DescriptionFeatureExtractor.cpp \
		description/src/DescriptionGCH.cpp \
		description/src/DescriptionLAS.cpp \
		description/src/DescriptionLBP.cpp \
		description/src/DescriptionLCH.cpp \
		description/src/DescriptionLuColor.cpp \
		description/src/DescriptionSCD.cpp \
		description/src/DescriptionSS.cpp \
		description/src/DescriptionTAR.cpp \
		description/src/DescriptionUNSER.cpp \
		src/Adjacency.cpp \
		src/AdjacencyGrayCode.cpp \
		src/AdjacencyIterator.cpp \
		src/AdjacencyMarchingCube.cpp \
		src/AdjacencyRound.cpp \
		src/AdjacencySquare.cpp \
		src/BinaryCOG.cpp \
		src/BinaryComplement.cpp \
		src/Bit.cpp \
		src/BrainCOG.cpp \
		src/BrainInhomogeneity.cpp \
		src/BrainIntensities.cpp \
		src/BrainSkullStripping.cpp \
		src/BrainTissueSegmentation.cpp \
		src/BSpline.cpp \
		src/BucketQueue.cpp \
		src/ChessBoardDistanceFunction.cpp \
		src/CityBlockDistanceFunction.cpp \
		src/ClusteringIFT.cpp \
		src/Color.cpp \
		src/ColorChannel.cpp \
		src/ColorFeature.cpp \
		src/ColorHSI.cpp \
		src/ColorLab.cpp \
		src/ColorMedianFeature.cpp \
		src/ColorRGB.cpp \
		src/Common.cpp \
		src/ConnPathFunction.cpp \
		src/Correlation.cpp \
		src/DegeneratedIFT.cpp \
		src/DFIDE.cpp \
		src/DicomHeader.cpp \
		src/DiffPathFunction.cpp \
		src/DiffusionFunction.cpp \
		src/Display.cpp \
		src/DrawBox.cpp \
		src/DrawCircle.cpp \
		src/DrawIntersection.cpp \
		src/DrawLine.cpp \
		src/DrawPoint.cpp \
		src/DrawSequence.cpp \
		src/DrawShape.cpp \
		src/DrawSVGGraph.cpp \
		src/EdgeLazyWalk.cpp \
		src/EdgeLiveWire.cpp \
		src/EdgeMaxPathFunction.cpp \
		src/EdgeRiverBed.cpp \
		src/EuclideanDistanceFunction.cpp \
		src/FastBucketQueue.cpp \
		src/FastDecreasingFifoBucketQueue.cpp \
		src/FastDecreasingLifoBucketQueue.cpp \
		src/FastIncreasingFifoBucketQueue.cpp \
		src/FastIncreasingLifoBucketQueue.cpp \
		src/Feature.cpp \
		src/FeaturePathFunction.cpp \
		src/FeatureResize.cpp \
		src/File.cpp \
		src/FileFeature.cpp \
		src/FileSignal.cpp \
		src/FilteringAnisotropicDiffusion.cpp \
		src/FilteringGaussian.cpp \
		src/FilteringMean.cpp \
		src/FilteringMedian.cpp \
		src/FilteringOptimalAnisotropicDiffusion.cpp \
		src/Fourier.cpp \
		src/FuzzyCMeans.cpp \
		src/GeodesicPathFunction.cpp \
		src/Geometrics.cpp \
		src/GeometricsAffine.cpp \
		src/GeometricsRotate.cpp \
		src/GeometricsScale.cpp \
		src/GeometricsShear.cpp \
		src/GeometricsTranslate.cpp \
		src/GeoSumPathFunction.cpp \
		src/GradientCanny.cpp \
		src/GradientDirectional.cpp \
		src/GradientGabor.cpp \
		src/GradientMorphological.cpp \
		src/GradientPosOperations.cpp \
		src/GradientScaleCanny.cpp \
		src/GradientSobel.cpp \
		src/Graph.cpp \
		src/GrowingBucketQueue.cpp \
		src/HeartCOG.cpp \
		src/HeartSegmentation.cpp \
		src/HierarchicalGraph.cpp \
		src/HierarchicalPathFunction.cpp \
		src/Histogram.cpp \
		src/HoughCircle.cpp \
		src/Image.cpp \
		src/ImageEquals.cpp \
		src/ImageFrame.cpp \
		src/ImageIFT.cpp \
		src/ImageInterpolation.cpp \
		src/ImageMerge.cpp \
		src/ImageResize.cpp \
		src/ImageROI.cpp \
		src/ImageSplit.cpp \
		src/ImageSwapDims.cpp \
		src/InfBucketQueue.cpp \
		src/Integral.cpp \
		src/IntensityGlobals.cpp \
		src/IntensityLocals.cpp \
		src/Kernel.cpp \
		src/KernelBox.cpp \
		src/KernelGabor.cpp \
		src/KernelGaussian.cpp \
		src/KernelRound.cpp \
		src/KernelSobel.cpp \
		src/KernelSquare.cpp \
		src/KnnGraphAdjacency.cpp \
		src/LocalMaxPathFunction.cpp \
		src/LSHGraphAdjacency.cpp \
		src/MarchingCubes.cpp \
		src/MatrixDeterminant.cpp \
		src/MatrixIdentity.cpp \
		src/MatrixInverse.cpp \
		src/MatrixPolar.cpp \
		src/MatrixResize.cpp \
		src/MatrixTranspose.cpp \
		src/MaxPathFunction.cpp \
		src/MaxSumPathFunction.cpp \
		src/MedianFeature.cpp \
		src/MinimumSpanningTree.cpp \
		src/MinPathFunction.cpp \
		src/MorphologyDilation.cpp \
		src/MorphologyErosion.cpp \
		src/MultiImage.cpp \
		src/NiftiHeader.cpp \
		src/OPFClusterMatching.cpp \
		src/OPFHierarchicalClustering.cpp \
		src/OPFSpatialClustering.cpp \
		src/OPFSpectralClustering.cpp \
		src/OrientedExternPathFunction.cpp \
		src/OrientedInternPathFunction.cpp \
		src/PathFunction.cpp \
		src/PixelInterpolation.cpp \
		src/Plotting.cpp \
		src/PNMHeader.cpp \
		src/PreEuclideanDistanceFunction.cpp \
		src/RandomQueue.cpp \
		src/RealColor.cpp \
		src/Sample.cpp \
		src/SampleRandom.cpp \
		src/SampleUniform.cpp \
		src/SegmentationBackground.cpp \
		src/SegmentationBorder.cpp \
		src/SegmentationConnectedComponents.cpp \
		src/SegmentationFSum.cpp \
		src/SegmentationGeoStar.cpp \
		src/SegmentationHysteresis.cpp \
		src/SegmentationIntensityThreshold.cpp \
		src/SegmentationMST.cpp \
		src/SegmentationOctants.cpp \
		src/SegmentationOtsuThreshold.cpp \
		src/SegmentationQuadrants.cpp \
		src/SegmentationSurface.cpp \
		src/SegmentationWatershed.cpp \
		src/Signal.cpp \
		src/SignalAccumulate.cpp \
		src/SignalEmptyBins.cpp \
		src/SignalEqualize.cpp \
		src/SignalHysteresis.cpp \
		src/SignalKeyPoints.cpp \
		src/SignalMedianFilter.cpp \
		src/SignalNormalize.cpp \
		src/SignalOtsuThreshold.cpp \
		src/SortingBinarySearch.cpp \
		src/SortingSort.cpp \
		src/SpatialFeature.cpp \
		src/SquareEuclideanDistanceFunction.cpp \
		src/StatisticsAverage.cpp \
		src/StatisticsBaddeley.cpp \
		src/StatisticsDice.cpp \
		src/StatisticsJaccard.cpp \
		src/StatisticsKappa.cpp \
		src/StatisticsMAD.cpp \
		src/StatisticsObsAgree.cpp \
		src/StatisticsPosNeg.cpp \
		src/StatisticsStdDev.cpp \
		src/SumPathFunction.cpp \
		src/Superpixel.cpp \
		src/Table.cpp \
		src/TransformEuclDist.cpp \
		src/TransformEuclDistInv.cpp 
OBJECTS       = ../build/linux/release/obj/columnset.o \
		../build/linux/release/obj/constraint.o \
		../build/linux/release/obj/matrix.o \
		../build/linux/release/obj/objectivefunction.o \
		../build/linux/release/obj/simplex.o \
		../build/linux/release/obj/variable.o \
		../build/linux/release/obj/BucketHashing.o \
		../build/linux/release/obj/Geometry.o \
		../build/linux/release/obj/GlobalVars.o \
		../build/linux/release/obj/LocalitySensitiveHashing.o \
		../build/linux/release/obj/LSH.o \
		../build/linux/release/obj/NearNeighbors.o \
		../build/linux/release/obj/Random.o \
		../build/linux/release/obj/SelfTuning.o \
		../build/linux/release/obj/SLIC.o \
		../build/linux/release/obj/adler32.o \
		../build/linux/release/obj/compress.o \
		../build/linux/release/obj/crc32.o \
		../build/linux/release/obj/deflate.o \
		../build/linux/release/obj/gzclose.o \
		../build/linux/release/obj/gzlib.o \
		../build/linux/release/obj/gzread.o \
		../build/linux/release/obj/gzwrite.o \
		../build/linux/release/obj/infback.o \
		../build/linux/release/obj/inffast.o \
		../build/linux/release/obj/inflate.o \
		../build/linux/release/obj/inftrees.o \
		../build/linux/release/obj/trees.o \
		../build/linux/release/obj/uncompr.o \
		../build/linux/release/obj/zutil.o \
		../build/linux/release/obj/DescriptionACC.o \
		../build/linux/release/obj/DescriptionBAS.o \
		../build/linux/release/obj/DescriptionBIC.o \
		../build/linux/release/obj/DescriptionBoxCounting.o \
		../build/linux/release/obj/DescriptionCCH.o \
		../build/linux/release/obj/DescriptionCCV.o \
		../build/linux/release/obj/DescriptionCGI.o \
		../build/linux/release/obj/DescriptionCLD.o \
		../build/linux/release/obj/DescriptionCSD.o \
		../build/linux/release/obj/DescriptionEHD.o \
		../build/linux/release/obj/DescriptionFeatureExtractor.o \
		../build/linux/release/obj/DescriptionGCH.o \
		../build/linux/release/obj/DescriptionLAS.o \
		../build/linux/release/obj/DescriptionLBP.o \
		../build/linux/release/obj/DescriptionLCH.o \
		../build/linux/release/obj/DescriptionLuColor.o \
		../build/linux/release/obj/DescriptionSCD.o \
		../build/linux/release/obj/DescriptionSS.o \
		../build/linux/release/obj/DescriptionTAR.o \
		../build/linux/release/obj/DescriptionUNSER.o \
		../build/linux/release/obj/Adjacency.o \
		../build/linux/release/obj/AdjacencyGrayCode.o \
		../build/linux/release/obj/AdjacencyIterator.o \
		../build/linux/release/obj/AdjacencyMarchingCube.o \
		../build/linux/release/obj/AdjacencyRound.o \
		../build/linux/release/obj/AdjacencySquare.o \
		../build/linux/release/obj/BinaryCOG.o \
		../build/linux/release/obj/BinaryComplement.o \
		../build/linux/release/obj/Bit.o \
		../build/linux/release/obj/BrainCOG.o \
		../build/linux/release/obj/BrainInhomogeneity.o \
		../build/linux/release/obj/BrainIntensities.o \
		../build/linux/release/obj/BrainSkullStripping.o \
		../build/linux/release/obj/BrainTissueSegmentation.o \
		../build/linux/release/obj/BSpline.o \
		../build/linux/release/obj/BucketQueue.o \
		../build/linux/release/obj/ChessBoardDistanceFunction.o \
		../build/linux/release/obj/CityBlockDistanceFunction.o \
		../build/linux/release/obj/ClusteringIFT.o \
		../build/linux/release/obj/Color.o \
		../build/linux/release/obj/ColorChannel.o \
		../build/linux/release/obj/ColorFeature.o \
		../build/linux/release/obj/ColorHSI.o \
		../build/linux/release/obj/ColorLab.o \
		../build/linux/release/obj/ColorMedianFeature.o \
		../build/linux/release/obj/ColorRGB.o \
		../build/linux/release/obj/Common.o \
		../build/linux/release/obj/ConnPathFunction.o \
		../build/linux/release/obj/Correlation.o \
		../build/linux/release/obj/DegeneratedIFT.o \
		../build/linux/release/obj/DFIDE.o \
		../build/linux/release/obj/DicomHeader.o \
		../build/linux/release/obj/DiffPathFunction.o \
		../build/linux/release/obj/DiffusionFunction.o \
		../build/linux/release/obj/Display.o \
		../build/linux/release/obj/DrawBox.o \
		../build/linux/release/obj/DrawCircle.o \
		../build/linux/release/obj/DrawIntersection.o \
		../build/linux/release/obj/DrawLine.o \
		../build/linux/release/obj/DrawPoint.o \
		../build/linux/release/obj/DrawSequence.o \
		../build/linux/release/obj/DrawShape.o \
		../build/linux/release/obj/DrawSVGGraph.o \
		../build/linux/release/obj/EdgeLazyWalk.o \
		../build/linux/release/obj/EdgeLiveWire.o \
		../build/linux/release/obj/EdgeMaxPathFunction.o \
		../build/linux/release/obj/EdgeRiverBed.o \
		../build/linux/release/obj/EuclideanDistanceFunction.o \
		../build/linux/release/obj/FastBucketQueue.o \
		../build/linux/release/obj/FastDecreasingFifoBucketQueue.o \
		../build/linux/release/obj/FastDecreasingLifoBucketQueue.o \
		../build/linux/release/obj/FastIncreasingFifoBucketQueue.o \
		../build/linux/release/obj/FastIncreasingLifoBucketQueue.o \
		../build/linux/release/obj/Feature.o \
		../build/linux/release/obj/FeaturePathFunction.o \
		../build/linux/release/obj/FeatureResize.o \
		../build/linux/release/obj/File.o \
		../build/linux/release/obj/FileFeature.o \
		../build/linux/release/obj/FileSignal.o \
		../build/linux/release/obj/FilteringAnisotropicDiffusion.o \
		../build/linux/release/obj/FilteringGaussian.o \
		../build/linux/release/obj/FilteringMean.o \
		../build/linux/release/obj/FilteringMedian.o \
		../build/linux/release/obj/FilteringOptimalAnisotropicDiffusion.o \
		../build/linux/release/obj/Fourier.o \
		../build/linux/release/obj/FuzzyCMeans.o \
		../build/linux/release/obj/GeodesicPathFunction.o \
		../build/linux/release/obj/Geometrics.o \
		../build/linux/release/obj/GeometricsAffine.o \
		../build/linux/release/obj/GeometricsRotate.o \
		../build/linux/release/obj/GeometricsScale.o \
		../build/linux/release/obj/GeometricsShear.o \
		../build/linux/release/obj/GeometricsTranslate.o \
		../build/linux/release/obj/GeoSumPathFunction.o \
		../build/linux/release/obj/GradientCanny.o \
		../build/linux/release/obj/GradientDirectional.o \
		../build/linux/release/obj/GradientGabor.o \
		../build/linux/release/obj/GradientMorphological.o \
		../build/linux/release/obj/GradientPosOperations.o \
		../build/linux/release/obj/GradientScaleCanny.o \
		../build/linux/release/obj/GradientSobel.o \
		../build/linux/release/obj/Graph.o \
		../build/linux/release/obj/GrowingBucketQueue.o \
		../build/linux/release/obj/HeartCOG.o \
		../build/linux/release/obj/HeartSegmentation.o \
		../build/linux/release/obj/HierarchicalGraph.o \
		../build/linux/release/obj/HierarchicalPathFunction.o \
		../build/linux/release/obj/Histogram.o \
		../build/linux/release/obj/HoughCircle.o \
		../build/linux/release/obj/Image.o \
		../build/linux/release/obj/ImageEquals.o \
		../build/linux/release/obj/ImageFrame.o \
		../build/linux/release/obj/ImageIFT.o \
		../build/linux/release/obj/ImageInterpolation.o \
		../build/linux/release/obj/ImageMerge.o \
		../build/linux/release/obj/ImageResize.o \
		../build/linux/release/obj/ImageROI.o \
		../build/linux/release/obj/ImageSplit.o \
		../build/linux/release/obj/ImageSwapDims.o \
		../build/linux/release/obj/InfBucketQueue.o \
		../build/linux/release/obj/Integral.o \
		../build/linux/release/obj/IntensityGlobals.o \
		../build/linux/release/obj/IntensityLocals.o \
		../build/linux/release/obj/Kernel.o \
		../build/linux/release/obj/KernelBox.o \
		../build/linux/release/obj/KernelGabor.o \
		../build/linux/release/obj/KernelGaussian.o \
		../build/linux/release/obj/KernelRound.o \
		../build/linux/release/obj/KernelSobel.o \
		../build/linux/release/obj/KernelSquare.o \
		../build/linux/release/obj/KnnGraphAdjacency.o \
		../build/linux/release/obj/LocalMaxPathFunction.o \
		../build/linux/release/obj/LSHGraphAdjacency.o \
		../build/linux/release/obj/MarchingCubes.o \
		../build/linux/release/obj/MatrixDeterminant.o \
		../build/linux/release/obj/MatrixIdentity.o \
		../build/linux/release/obj/MatrixInverse.o \
		../build/linux/release/obj/MatrixPolar.o \
		../build/linux/release/obj/MatrixResize.o \
		../build/linux/release/obj/MatrixTranspose.o \
		../build/linux/release/obj/MaxPathFunction.o \
		../build/linux/release/obj/MaxSumPathFunction.o \
		../build/linux/release/obj/MedianFeature.o \
		../build/linux/release/obj/MinimumSpanningTree.o \
		../build/linux/release/obj/MinPathFunction.o \
		../build/linux/release/obj/MorphologyDilation.o \
		../build/linux/release/obj/MorphologyErosion.o \
		../build/linux/release/obj/MultiImage.o \
		../build/linux/release/obj/NiftiHeader.o \
		../build/linux/release/obj/OPFClusterMatching.o \
		../build/linux/release/obj/OPFHierarchicalClustering.o \
		../build/linux/release/obj/OPFSpatialClustering.o \
		../build/linux/release/obj/OPFSpectralClustering.o \
		../build/linux/release/obj/OrientedExternPathFunction.o \
		../build/linux/release/obj/OrientedInternPathFunction.o \
		../build/linux/release/obj/PathFunction.o \
		../build/linux/release/obj/PixelInterpolation.o \
		../build/linux/release/obj/Plotting.o \
		../build/linux/release/obj/PNMHeader.o \
		../build/linux/release/obj/PreEuclideanDistanceFunction.o \
		../build/linux/release/obj/RandomQueue.o \
		../build/linux/release/obj/RealColor.o \
		../build/linux/release/obj/Sample.o \
		../build/linux/release/obj/SampleRandom.o \
		../build/linux/release/obj/SampleUniform.o \
		../build/linux/release/obj/SegmentationBackground.o \
		../build/linux/release/obj/SegmentationBorder.o \
		../build/linux/release/obj/SegmentationConnectedComponents.o \
		../build/linux/release/obj/SegmentationFSum.o \
		../build/linux/release/obj/SegmentationGeoStar.o \
		../build/linux/release/obj/SegmentationHysteresis.o \
		../build/linux/release/obj/SegmentationIntensityThreshold.o \
		../build/linux/release/obj/SegmentationMST.o \
		../build/linux/release/obj/SegmentationOctants.o \
		../build/linux/release/obj/SegmentationOtsuThreshold.o \
		../build/linux/release/obj/SegmentationQuadrants.o \
		../build/linux/release/obj/SegmentationSurface.o \
		../build/linux/release/obj/SegmentationWatershed.o \
		../build/linux/release/obj/Signal.o \
		../build/linux/release/obj/SignalAccumulate.o \
		../build/linux/release/obj/SignalEmptyBins.o \
		../build/linux/release/obj/SignalEqualize.o \
		../build/linux/release/obj/SignalHysteresis.o \
		../build/linux/release/obj/SignalKeyPoints.o \
		../build/linux/release/obj/SignalMedianFilter.o \
		../build/linux/release/obj/SignalNormalize.o \
		../build/linux/release/obj/SignalOtsuThreshold.o \
		../build/linux/release/obj/SortingBinarySearch.o \
		../build/linux/release/obj/SortingSort.o \
		../build/linux/release/obj/SpatialFeature.o \
		../build/linux/release/obj/SquareEuclideanDistanceFunction.o \
		../build/linux/release/obj/StatisticsAverage.o \
		../build/linux/release/obj/StatisticsBaddeley.o \
		../build/linux/release/obj/StatisticsDice.o \
		../build/linux/release/obj/StatisticsJaccard.o \
		../build/linux/release/obj/StatisticsKappa.o \
		../build/linux/release/obj/StatisticsMAD.o \
		../build/linux/release/obj/StatisticsObsAgree.o \
		../build/linux/release/obj/StatisticsPosNeg.o \
		../build/linux/release/obj/StatisticsStdDev.o \
		../build/linux/release/obj/SumPathFunction.o \
		../build/linux/release/obj/Superpixel.o \
		../build/linux/release/obj/Table.o \
		../build/linux/release/obj/TransformEuclDist.o \
		../build/linux/release/obj/TransformEuclDistInv.o

QMAKE_TARGET  = bial
DESTDIR       = ../build/linux/release/lib/
TARGET        = libbial.so.1.0.0
TARGETA       = ../build/linux/release/lib/libbial.a
TARGET0       = libbial.so
TARGETD       = libbial.so.1.0.0
TARGET1       = libbial.so.1
TARGET2       = libbial.so.1.0


first: all
####### Build rules

directories:
	@test -d ${DESTDIR} || mkdir -p ${DESTDIR}
	@test -d ${OBJECTS_DIR} || mkdir -p ${OBJECTS_DIR}



../build/linux/release/lib/$(TARGET): directories $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  

	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)
	-$(DEL_FILE) ../build/linux/release/lib/$(TARGET)
	-$(MOVE) $(TARGET)  ../build/linux/release/lib/ 
	-$(DEL_FILE) ../build/linux/release/lib/$(TARGET0)
	-$(DEL_FILE) ../build/linux/release/lib/$(TARGET1)
	-$(DEL_FILE) ../build/linux/release/lib/$(TARGET2)
	-$(MOVE) $(TARGET0) ../build/linux/release/lib/ 
	-$(MOVE) $(TARGET1) ../build/linux/release/lib/ 
	-$(MOVE) $(TARGET2) ../build/linux/release/lib/ 



staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS) $(OBJCOMP) 
	-$(DEL_FILE) $(TARGETA) 
	$(AR) $(TARGETA) $(OBJECTS)


all: ../build/linux/release/lib/$(TARGET)

clean: compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core

####### Compile

../build/linux/release/obj/columnset.o: cpplex/src/columnset.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/columnset.o cpplex/src/columnset.cpp

../build/linux/release/obj/constraint.o: cpplex/src/constraint.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/constraint.o cpplex/src/constraint.cpp

../build/linux/release/obj/matrix.o: cpplex/src/matrix.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/matrix.o cpplex/src/matrix.cpp

../build/linux/release/obj/objectivefunction.o: cpplex/src/objectivefunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/objectivefunction.o cpplex/src/objectivefunction.cpp

../build/linux/release/obj/simplex.o: cpplex/src/simplex.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/simplex.o cpplex/src/simplex.cpp

../build/linux/release/obj/variable.o: cpplex/src/variable.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/variable.o cpplex/src/variable.cpp

../build/linux/release/obj/BucketHashing.o: lsh/src/BucketHashing.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BucketHashing.o lsh/src/BucketHashing.cpp

../build/linux/release/obj/Geometry.o: lsh/src/Geometry.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Geometry.o lsh/src/Geometry.cpp

../build/linux/release/obj/GlobalVars.o: lsh/src/GlobalVars.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GlobalVars.o lsh/src/GlobalVars.cpp

../build/linux/release/obj/LocalitySensitiveHashing.o: lsh/src/LocalitySensitiveHashing.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/LocalitySensitiveHashing.o lsh/src/LocalitySensitiveHashing.cpp

../build/linux/release/obj/LSH.o: lsh/src/LSH.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/LSH.o lsh/src/LSH.cpp

../build/linux/release/obj/NearNeighbors.o: lsh/src/NearNeighbors.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/NearNeighbors.o lsh/src/NearNeighbors.cpp

../build/linux/release/obj/Random.o: lsh/src/Random.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Random.o lsh/src/Random.cpp

../build/linux/release/obj/SelfTuning.o: lsh/src/SelfTuning.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SelfTuning.o lsh/src/SelfTuning.cpp

../build/linux/release/obj/SLIC.o: SLIC/src/SLIC.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SLIC.o SLIC/src/SLIC.cpp

../build/linux/release/obj/adler32.o: zlib/adler32.c zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/adler32.o zlib/adler32.c

../build/linux/release/obj/compress.o: zlib/compress.c zlib/zlib.h \
		zlib/zconf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/compress.o zlib/compress.c

../build/linux/release/obj/crc32.o: zlib/crc32.c zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h \
		zlib/crc32.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/crc32.o zlib/crc32.c

../build/linux/release/obj/deflate.o: zlib/deflate.c zlib/deflate.h \
		zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/deflate.o zlib/deflate.c

../build/linux/release/obj/gzclose.o: zlib/gzclose.c zlib/gzguts.h \
		zlib/zlib.h \
		zlib/zconf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/gzclose.o zlib/gzclose.c

../build/linux/release/obj/gzlib.o: zlib/gzlib.c zlib/gzguts.h \
		zlib/zlib.h \
		zlib/zconf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/gzlib.o zlib/gzlib.c

../build/linux/release/obj/gzread.o: zlib/gzread.c zlib/gzguts.h \
		zlib/zlib.h \
		zlib/zconf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/gzread.o zlib/gzread.c

../build/linux/release/obj/gzwrite.o: zlib/gzwrite.c zlib/gzguts.h \
		zlib/zlib.h \
		zlib/zconf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/gzwrite.o zlib/gzwrite.c

../build/linux/release/obj/infback.o: zlib/infback.c zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h \
		zlib/inftrees.h \
		zlib/inflate.h \
		zlib/inffast.h \
		zlib/inffixed.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/infback.o zlib/infback.c

../build/linux/release/obj/inffast.o: zlib/inffast.c zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h \
		zlib/inftrees.h \
		zlib/inflate.h \
		zlib/inffast.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/inffast.o zlib/inffast.c

../build/linux/release/obj/inflate.o: zlib/inflate.c zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h \
		zlib/inftrees.h \
		zlib/inflate.h \
		zlib/inffast.h \
		zlib/inffixed.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/inflate.o zlib/inflate.c

../build/linux/release/obj/inftrees.o: zlib/inftrees.c zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h \
		zlib/inftrees.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/inftrees.o zlib/inftrees.c

../build/linux/release/obj/trees.o: zlib/trees.c zlib/deflate.h \
		zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h \
		zlib/trees.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/trees.o zlib/trees.c

../build/linux/release/obj/uncompr.o: zlib/uncompr.c zlib/zlib.h \
		zlib/zconf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/uncompr.o zlib/uncompr.c

../build/linux/release/obj/zutil.o: zlib/zutil.c zlib/zutil.h \
		zlib/zlib.h \
		zlib/zconf.h \
		zlib/gzguts.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o ../build/linux/release/obj/zutil.o zlib/zutil.c

../build/linux/release/obj/DescriptionACC.o: description/src/DescriptionACC.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionACC.o description/src/DescriptionACC.cpp

../build/linux/release/obj/DescriptionBAS.o: description/src/DescriptionBAS.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionBAS.o description/src/DescriptionBAS.cpp

../build/linux/release/obj/DescriptionBIC.o: description/src/DescriptionBIC.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionBIC.o description/src/DescriptionBIC.cpp

../build/linux/release/obj/DescriptionBoxCounting.o: description/src/DescriptionBoxCounting.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionBoxCounting.o description/src/DescriptionBoxCounting.cpp

../build/linux/release/obj/DescriptionCCH.o: description/src/DescriptionCCH.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionCCH.o description/src/DescriptionCCH.cpp

../build/linux/release/obj/DescriptionCCV.o: description/src/DescriptionCCV.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionCCV.o description/src/DescriptionCCV.cpp

../build/linux/release/obj/DescriptionCGI.o: description/src/DescriptionCGI.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionCGI.o description/src/DescriptionCGI.cpp

../build/linux/release/obj/DescriptionCLD.o: description/src/DescriptionCLD.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionCLD.o description/src/DescriptionCLD.cpp

../build/linux/release/obj/DescriptionCSD.o: description/src/DescriptionCSD.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionCSD.o description/src/DescriptionCSD.cpp

../build/linux/release/obj/DescriptionEHD.o: description/src/DescriptionEHD.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionEHD.o description/src/DescriptionEHD.cpp

../build/linux/release/obj/DescriptionFeatureExtractor.o: description/src/DescriptionFeatureExtractor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionFeatureExtractor.o description/src/DescriptionFeatureExtractor.cpp

../build/linux/release/obj/DescriptionGCH.o: description/src/DescriptionGCH.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionGCH.o description/src/DescriptionGCH.cpp

../build/linux/release/obj/DescriptionLAS.o: description/src/DescriptionLAS.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionLAS.o description/src/DescriptionLAS.cpp

../build/linux/release/obj/DescriptionLBP.o: description/src/DescriptionLBP.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionLBP.o description/src/DescriptionLBP.cpp

../build/linux/release/obj/DescriptionLCH.o: description/src/DescriptionLCH.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionLCH.o description/src/DescriptionLCH.cpp

../build/linux/release/obj/DescriptionLuColor.o: description/src/DescriptionLuColor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionLuColor.o description/src/DescriptionLuColor.cpp

../build/linux/release/obj/DescriptionSCD.o: description/src/DescriptionSCD.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionSCD.o description/src/DescriptionSCD.cpp

../build/linux/release/obj/DescriptionSS.o: description/src/DescriptionSS.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionSS.o description/src/DescriptionSS.cpp

../build/linux/release/obj/DescriptionTAR.o: description/src/DescriptionTAR.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionTAR.o description/src/DescriptionTAR.cpp

../build/linux/release/obj/DescriptionUNSER.o: description/src/DescriptionUNSER.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DescriptionUNSER.o description/src/DescriptionUNSER.cpp

../build/linux/release/obj/Adjacency.o: src/Adjacency.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Adjacency.o src/Adjacency.cpp

../build/linux/release/obj/AdjacencyGrayCode.o: src/AdjacencyGrayCode.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/AdjacencyGrayCode.o src/AdjacencyGrayCode.cpp

../build/linux/release/obj/AdjacencyIterator.o: src/AdjacencyIterator.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/AdjacencyIterator.o src/AdjacencyIterator.cpp

../build/linux/release/obj/AdjacencyMarchingCube.o: src/AdjacencyMarchingCube.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/AdjacencyMarchingCube.o src/AdjacencyMarchingCube.cpp

../build/linux/release/obj/AdjacencyRound.o: src/AdjacencyRound.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/AdjacencyRound.o src/AdjacencyRound.cpp

../build/linux/release/obj/AdjacencySquare.o: src/AdjacencySquare.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/AdjacencySquare.o src/AdjacencySquare.cpp

../build/linux/release/obj/BinaryCOG.o: src/BinaryCOG.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BinaryCOG.o src/BinaryCOG.cpp

../build/linux/release/obj/BinaryComplement.o: src/BinaryComplement.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BinaryComplement.o src/BinaryComplement.cpp

../build/linux/release/obj/Bit.o: src/Bit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Bit.o src/Bit.cpp

../build/linux/release/obj/BrainCOG.o: src/BrainCOG.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BrainCOG.o src/BrainCOG.cpp

../build/linux/release/obj/BrainInhomogeneity.o: src/BrainInhomogeneity.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BrainInhomogeneity.o src/BrainInhomogeneity.cpp

../build/linux/release/obj/BrainIntensities.o: src/BrainIntensities.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BrainIntensities.o src/BrainIntensities.cpp

../build/linux/release/obj/BrainSkullStripping.o: src/BrainSkullStripping.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BrainSkullStripping.o src/BrainSkullStripping.cpp

../build/linux/release/obj/BrainTissueSegmentation.o: src/BrainTissueSegmentation.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BrainTissueSegmentation.o src/BrainTissueSegmentation.cpp

../build/linux/release/obj/BSpline.o: src/BSpline.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BSpline.o src/BSpline.cpp

../build/linux/release/obj/BucketQueue.o: src/BucketQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/BucketQueue.o src/BucketQueue.cpp

../build/linux/release/obj/ChessBoardDistanceFunction.o: src/ChessBoardDistanceFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ChessBoardDistanceFunction.o src/ChessBoardDistanceFunction.cpp

../build/linux/release/obj/CityBlockDistanceFunction.o: src/CityBlockDistanceFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/CityBlockDistanceFunction.o src/CityBlockDistanceFunction.cpp

../build/linux/release/obj/ClusteringIFT.o: src/ClusteringIFT.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ClusteringIFT.o src/ClusteringIFT.cpp

../build/linux/release/obj/Color.o: src/Color.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Color.o src/Color.cpp

../build/linux/release/obj/ColorChannel.o: src/ColorChannel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ColorChannel.o src/ColorChannel.cpp

../build/linux/release/obj/ColorFeature.o: src/ColorFeature.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ColorFeature.o src/ColorFeature.cpp

../build/linux/release/obj/ColorHSI.o: src/ColorHSI.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ColorHSI.o src/ColorHSI.cpp

../build/linux/release/obj/ColorLab.o: src/ColorLab.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ColorLab.o src/ColorLab.cpp

../build/linux/release/obj/ColorMedianFeature.o: src/ColorMedianFeature.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ColorMedianFeature.o src/ColorMedianFeature.cpp

../build/linux/release/obj/ColorRGB.o: src/ColorRGB.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ColorRGB.o src/ColorRGB.cpp

../build/linux/release/obj/Common.o: src/Common.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Common.o src/Common.cpp

../build/linux/release/obj/ConnPathFunction.o: src/ConnPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ConnPathFunction.o src/ConnPathFunction.cpp

../build/linux/release/obj/Correlation.o: src/Correlation.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Correlation.o src/Correlation.cpp

../build/linux/release/obj/DegeneratedIFT.o: src/DegeneratedIFT.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DegeneratedIFT.o src/DegeneratedIFT.cpp

../build/linux/release/obj/DFIDE.o: src/DFIDE.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DFIDE.o src/DFIDE.cpp

../build/linux/release/obj/DicomHeader.o: src/DicomHeader.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DicomHeader.o src/DicomHeader.cpp

../build/linux/release/obj/DiffPathFunction.o: src/DiffPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DiffPathFunction.o src/DiffPathFunction.cpp

../build/linux/release/obj/DiffusionFunction.o: src/DiffusionFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DiffusionFunction.o src/DiffusionFunction.cpp

../build/linux/release/obj/Display.o: src/Display.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Display.o src/Display.cpp

../build/linux/release/obj/DrawBox.o: src/DrawBox.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DrawBox.o src/DrawBox.cpp

../build/linux/release/obj/DrawCircle.o: src/DrawCircle.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DrawCircle.o src/DrawCircle.cpp

../build/linux/release/obj/DrawIntersection.o: src/DrawIntersection.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DrawIntersection.o src/DrawIntersection.cpp

../build/linux/release/obj/DrawLine.o: src/DrawLine.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DrawLine.o src/DrawLine.cpp

../build/linux/release/obj/DrawPoint.o: src/DrawPoint.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DrawPoint.o src/DrawPoint.cpp

../build/linux/release/obj/DrawSequence.o: src/DrawSequence.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DrawSequence.o src/DrawSequence.cpp

../build/linux/release/obj/DrawShape.o: src/DrawShape.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DrawShape.o src/DrawShape.cpp

../build/linux/release/obj/DrawSVGGraph.o: src/DrawSVGGraph.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/DrawSVGGraph.o src/DrawSVGGraph.cpp

../build/linux/release/obj/EdgeLazyWalk.o: src/EdgeLazyWalk.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/EdgeLazyWalk.o src/EdgeLazyWalk.cpp

../build/linux/release/obj/EdgeLiveWire.o: src/EdgeLiveWire.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/EdgeLiveWire.o src/EdgeLiveWire.cpp

../build/linux/release/obj/EdgeMaxPathFunction.o: src/EdgeMaxPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/EdgeMaxPathFunction.o src/EdgeMaxPathFunction.cpp

../build/linux/release/obj/EdgeRiverBed.o: src/EdgeRiverBed.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/EdgeRiverBed.o src/EdgeRiverBed.cpp

../build/linux/release/obj/EuclideanDistanceFunction.o: src/EuclideanDistanceFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/EuclideanDistanceFunction.o src/EuclideanDistanceFunction.cpp

../build/linux/release/obj/FastBucketQueue.o: src/FastBucketQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FastBucketQueue.o src/FastBucketQueue.cpp

../build/linux/release/obj/FastDecreasingFifoBucketQueue.o: src/FastDecreasingFifoBucketQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FastDecreasingFifoBucketQueue.o src/FastDecreasingFifoBucketQueue.cpp

../build/linux/release/obj/FastDecreasingLifoBucketQueue.o: src/FastDecreasingLifoBucketQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FastDecreasingLifoBucketQueue.o src/FastDecreasingLifoBucketQueue.cpp

../build/linux/release/obj/FastIncreasingFifoBucketQueue.o: src/FastIncreasingFifoBucketQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FastIncreasingFifoBucketQueue.o src/FastIncreasingFifoBucketQueue.cpp

../build/linux/release/obj/FastIncreasingLifoBucketQueue.o: src/FastIncreasingLifoBucketQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FastIncreasingLifoBucketQueue.o src/FastIncreasingLifoBucketQueue.cpp

../build/linux/release/obj/Feature.o: src/Feature.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Feature.o src/Feature.cpp

../build/linux/release/obj/FeaturePathFunction.o: src/FeaturePathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FeaturePathFunction.o src/FeaturePathFunction.cpp

../build/linux/release/obj/FeatureResize.o: src/FeatureResize.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FeatureResize.o src/FeatureResize.cpp

../build/linux/release/obj/File.o: src/File.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/File.o src/File.cpp

../build/linux/release/obj/FileFeature.o: src/FileFeature.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FileFeature.o src/FileFeature.cpp

../build/linux/release/obj/FileSignal.o: src/FileSignal.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FileSignal.o src/FileSignal.cpp

../build/linux/release/obj/FilteringAnisotropicDiffusion.o: src/FilteringAnisotropicDiffusion.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FilteringAnisotropicDiffusion.o src/FilteringAnisotropicDiffusion.cpp

../build/linux/release/obj/FilteringGaussian.o: src/FilteringGaussian.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FilteringGaussian.o src/FilteringGaussian.cpp

../build/linux/release/obj/FilteringMean.o: src/FilteringMean.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FilteringMean.o src/FilteringMean.cpp

../build/linux/release/obj/FilteringMedian.o: src/FilteringMedian.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FilteringMedian.o src/FilteringMedian.cpp

../build/linux/release/obj/FilteringOptimalAnisotropicDiffusion.o: src/FilteringOptimalAnisotropicDiffusion.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FilteringOptimalAnisotropicDiffusion.o src/FilteringOptimalAnisotropicDiffusion.cpp

../build/linux/release/obj/Fourier.o: src/Fourier.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Fourier.o src/Fourier.cpp

../build/linux/release/obj/FuzzyCMeans.o: src/FuzzyCMeans.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/FuzzyCMeans.o src/FuzzyCMeans.cpp

../build/linux/release/obj/GeodesicPathFunction.o: src/GeodesicPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GeodesicPathFunction.o src/GeodesicPathFunction.cpp

../build/linux/release/obj/Geometrics.o: src/Geometrics.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Geometrics.o src/Geometrics.cpp

../build/linux/release/obj/GeometricsAffine.o: src/GeometricsAffine.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GeometricsAffine.o src/GeometricsAffine.cpp

../build/linux/release/obj/GeometricsRotate.o: src/GeometricsRotate.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GeometricsRotate.o src/GeometricsRotate.cpp

../build/linux/release/obj/GeometricsScale.o: src/GeometricsScale.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GeometricsScale.o src/GeometricsScale.cpp

../build/linux/release/obj/GeometricsShear.o: src/GeometricsShear.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GeometricsShear.o src/GeometricsShear.cpp

../build/linux/release/obj/GeometricsTranslate.o: src/GeometricsTranslate.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GeometricsTranslate.o src/GeometricsTranslate.cpp

../build/linux/release/obj/GeoSumPathFunction.o: src/GeoSumPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GeoSumPathFunction.o src/GeoSumPathFunction.cpp

../build/linux/release/obj/GradientCanny.o: src/GradientCanny.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GradientCanny.o src/GradientCanny.cpp

../build/linux/release/obj/GradientDirectional.o: src/GradientDirectional.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GradientDirectional.o src/GradientDirectional.cpp

../build/linux/release/obj/GradientGabor.o: src/GradientGabor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GradientGabor.o src/GradientGabor.cpp

../build/linux/release/obj/GradientMorphological.o: src/GradientMorphological.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GradientMorphological.o src/GradientMorphological.cpp

../build/linux/release/obj/GradientPosOperations.o: src/GradientPosOperations.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GradientPosOperations.o src/GradientPosOperations.cpp

../build/linux/release/obj/GradientScaleCanny.o: src/GradientScaleCanny.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GradientScaleCanny.o src/GradientScaleCanny.cpp

../build/linux/release/obj/GradientSobel.o: src/GradientSobel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GradientSobel.o src/GradientSobel.cpp

../build/linux/release/obj/Graph.o: src/Graph.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Graph.o src/Graph.cpp

../build/linux/release/obj/GrowingBucketQueue.o: src/GrowingBucketQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/GrowingBucketQueue.o src/GrowingBucketQueue.cpp

../build/linux/release/obj/HeartCOG.o: src/HeartCOG.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/HeartCOG.o src/HeartCOG.cpp

../build/linux/release/obj/HeartSegmentation.o: src/HeartSegmentation.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/HeartSegmentation.o src/HeartSegmentation.cpp

../build/linux/release/obj/HierarchicalGraph.o: src/HierarchicalGraph.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/HierarchicalGraph.o src/HierarchicalGraph.cpp

../build/linux/release/obj/HierarchicalPathFunction.o: src/HierarchicalPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/HierarchicalPathFunction.o src/HierarchicalPathFunction.cpp

../build/linux/release/obj/Histogram.o: src/Histogram.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Histogram.o src/Histogram.cpp

../build/linux/release/obj/HoughCircle.o: src/HoughCircle.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/HoughCircle.o src/HoughCircle.cpp

../build/linux/release/obj/Image.o: src/Image.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Image.o src/Image.cpp

../build/linux/release/obj/ImageEquals.o: src/ImageEquals.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageEquals.o src/ImageEquals.cpp

../build/linux/release/obj/ImageFrame.o: src/ImageFrame.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageFrame.o src/ImageFrame.cpp

../build/linux/release/obj/ImageIFT.o: src/ImageIFT.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageIFT.o src/ImageIFT.cpp

../build/linux/release/obj/ImageInterpolation.o: src/ImageInterpolation.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageInterpolation.o src/ImageInterpolation.cpp

../build/linux/release/obj/ImageMerge.o: src/ImageMerge.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageMerge.o src/ImageMerge.cpp

../build/linux/release/obj/ImageResize.o: src/ImageResize.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageResize.o src/ImageResize.cpp

../build/linux/release/obj/ImageROI.o: src/ImageROI.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageROI.o src/ImageROI.cpp

../build/linux/release/obj/ImageSplit.o: src/ImageSplit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageSplit.o src/ImageSplit.cpp

../build/linux/release/obj/ImageSwapDims.o: src/ImageSwapDims.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/ImageSwapDims.o src/ImageSwapDims.cpp

../build/linux/release/obj/InfBucketQueue.o: src/InfBucketQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/InfBucketQueue.o src/InfBucketQueue.cpp

../build/linux/release/obj/Integral.o: src/Integral.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Integral.o src/Integral.cpp

../build/linux/release/obj/IntensityGlobals.o: src/IntensityGlobals.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/IntensityGlobals.o src/IntensityGlobals.cpp

../build/linux/release/obj/IntensityLocals.o: src/IntensityLocals.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/IntensityLocals.o src/IntensityLocals.cpp

../build/linux/release/obj/Kernel.o: src/Kernel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Kernel.o src/Kernel.cpp

../build/linux/release/obj/KernelBox.o: src/KernelBox.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/KernelBox.o src/KernelBox.cpp

../build/linux/release/obj/KernelGabor.o: src/KernelGabor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/KernelGabor.o src/KernelGabor.cpp

../build/linux/release/obj/KernelGaussian.o: src/KernelGaussian.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/KernelGaussian.o src/KernelGaussian.cpp

../build/linux/release/obj/KernelRound.o: src/KernelRound.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/KernelRound.o src/KernelRound.cpp

../build/linux/release/obj/KernelSobel.o: src/KernelSobel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/KernelSobel.o src/KernelSobel.cpp

../build/linux/release/obj/KernelSquare.o: src/KernelSquare.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/KernelSquare.o src/KernelSquare.cpp

../build/linux/release/obj/KnnGraphAdjacency.o: src/KnnGraphAdjacency.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/KnnGraphAdjacency.o src/KnnGraphAdjacency.cpp

../build/linux/release/obj/LocalMaxPathFunction.o: src/LocalMaxPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/LocalMaxPathFunction.o src/LocalMaxPathFunction.cpp

../build/linux/release/obj/LSHGraphAdjacency.o: src/LSHGraphAdjacency.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/LSHGraphAdjacency.o src/LSHGraphAdjacency.cpp

../build/linux/release/obj/MarchingCubes.o: src/MarchingCubes.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MarchingCubes.o src/MarchingCubes.cpp

../build/linux/release/obj/MatrixDeterminant.o: src/MatrixDeterminant.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MatrixDeterminant.o src/MatrixDeterminant.cpp

../build/linux/release/obj/MatrixIdentity.o: src/MatrixIdentity.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MatrixIdentity.o src/MatrixIdentity.cpp

../build/linux/release/obj/MatrixInverse.o: src/MatrixInverse.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MatrixInverse.o src/MatrixInverse.cpp

../build/linux/release/obj/MatrixPolar.o: src/MatrixPolar.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MatrixPolar.o src/MatrixPolar.cpp

../build/linux/release/obj/MatrixResize.o: src/MatrixResize.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MatrixResize.o src/MatrixResize.cpp

../build/linux/release/obj/MatrixTranspose.o: src/MatrixTranspose.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MatrixTranspose.o src/MatrixTranspose.cpp

../build/linux/release/obj/MaxPathFunction.o: src/MaxPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MaxPathFunction.o src/MaxPathFunction.cpp

../build/linux/release/obj/MaxSumPathFunction.o: src/MaxSumPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MaxSumPathFunction.o src/MaxSumPathFunction.cpp

../build/linux/release/obj/MedianFeature.o: src/MedianFeature.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MedianFeature.o src/MedianFeature.cpp

../build/linux/release/obj/MinimumSpanningTree.o: src/MinimumSpanningTree.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MinimumSpanningTree.o src/MinimumSpanningTree.cpp

../build/linux/release/obj/MinPathFunction.o: src/MinPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MinPathFunction.o src/MinPathFunction.cpp

../build/linux/release/obj/MorphologyDilation.o: src/MorphologyDilation.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MorphologyDilation.o src/MorphologyDilation.cpp

../build/linux/release/obj/MorphologyErosion.o: src/MorphologyErosion.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MorphologyErosion.o src/MorphologyErosion.cpp

../build/linux/release/obj/MultiImage.o: src/MultiImage.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/MultiImage.o src/MultiImage.cpp

../build/linux/release/obj/NiftiHeader.o: src/NiftiHeader.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/NiftiHeader.o src/NiftiHeader.cpp

../build/linux/release/obj/OPFClusterMatching.o: src/OPFClusterMatching.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/OPFClusterMatching.o src/OPFClusterMatching.cpp

../build/linux/release/obj/OPFHierarchicalClustering.o: src/OPFHierarchicalClustering.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/OPFHierarchicalClustering.o src/OPFHierarchicalClustering.cpp

../build/linux/release/obj/OPFSpatialClustering.o: src/OPFSpatialClustering.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/OPFSpatialClustering.o src/OPFSpatialClustering.cpp

../build/linux/release/obj/OPFSpectralClustering.o: src/OPFSpectralClustering.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/OPFSpectralClustering.o src/OPFSpectralClustering.cpp

../build/linux/release/obj/OrientedExternPathFunction.o: src/OrientedExternPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/OrientedExternPathFunction.o src/OrientedExternPathFunction.cpp

../build/linux/release/obj/OrientedInternPathFunction.o: src/OrientedInternPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/OrientedInternPathFunction.o src/OrientedInternPathFunction.cpp

../build/linux/release/obj/PathFunction.o: src/PathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/PathFunction.o src/PathFunction.cpp

../build/linux/release/obj/PixelInterpolation.o: src/PixelInterpolation.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/PixelInterpolation.o src/PixelInterpolation.cpp

../build/linux/release/obj/Plotting.o: src/Plotting.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Plotting.o src/Plotting.cpp

../build/linux/release/obj/PNMHeader.o: src/PNMHeader.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/PNMHeader.o src/PNMHeader.cpp

../build/linux/release/obj/PreEuclideanDistanceFunction.o: src/PreEuclideanDistanceFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/PreEuclideanDistanceFunction.o src/PreEuclideanDistanceFunction.cpp

../build/linux/release/obj/RandomQueue.o: src/RandomQueue.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/RandomQueue.o src/RandomQueue.cpp

../build/linux/release/obj/RealColor.o: src/RealColor.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/RealColor.o src/RealColor.cpp

../build/linux/release/obj/Sample.o: src/Sample.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Sample.o src/Sample.cpp

../build/linux/release/obj/SampleRandom.o: src/SampleRandom.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SampleRandom.o src/SampleRandom.cpp

../build/linux/release/obj/SampleUniform.o: src/SampleUniform.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SampleUniform.o src/SampleUniform.cpp

../build/linux/release/obj/SegmentationBackground.o: src/SegmentationBackground.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationBackground.o src/SegmentationBackground.cpp

../build/linux/release/obj/SegmentationBorder.o: src/SegmentationBorder.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationBorder.o src/SegmentationBorder.cpp

../build/linux/release/obj/SegmentationConnectedComponents.o: src/SegmentationConnectedComponents.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationConnectedComponents.o src/SegmentationConnectedComponents.cpp

../build/linux/release/obj/SegmentationFSum.o: src/SegmentationFSum.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationFSum.o src/SegmentationFSum.cpp

../build/linux/release/obj/SegmentationGeoStar.o: src/SegmentationGeoStar.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationGeoStar.o src/SegmentationGeoStar.cpp

../build/linux/release/obj/SegmentationHysteresis.o: src/SegmentationHysteresis.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationHysteresis.o src/SegmentationHysteresis.cpp

../build/linux/release/obj/SegmentationIntensityThreshold.o: src/SegmentationIntensityThreshold.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationIntensityThreshold.o src/SegmentationIntensityThreshold.cpp

../build/linux/release/obj/SegmentationMST.o: src/SegmentationMST.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationMST.o src/SegmentationMST.cpp

../build/linux/release/obj/SegmentationOctants.o: src/SegmentationOctants.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationOctants.o src/SegmentationOctants.cpp

../build/linux/release/obj/SegmentationOtsuThreshold.o: src/SegmentationOtsuThreshold.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationOtsuThreshold.o src/SegmentationOtsuThreshold.cpp

../build/linux/release/obj/SegmentationQuadrants.o: src/SegmentationQuadrants.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationQuadrants.o src/SegmentationQuadrants.cpp

../build/linux/release/obj/SegmentationSurface.o: src/SegmentationSurface.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationSurface.o src/SegmentationSurface.cpp

../build/linux/release/obj/SegmentationWatershed.o: src/SegmentationWatershed.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SegmentationWatershed.o src/SegmentationWatershed.cpp

../build/linux/release/obj/Signal.o: src/Signal.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Signal.o src/Signal.cpp

../build/linux/release/obj/SignalAccumulate.o: src/SignalAccumulate.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SignalAccumulate.o src/SignalAccumulate.cpp

../build/linux/release/obj/SignalEmptyBins.o: src/SignalEmptyBins.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SignalEmptyBins.o src/SignalEmptyBins.cpp

../build/linux/release/obj/SignalEqualize.o: src/SignalEqualize.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SignalEqualize.o src/SignalEqualize.cpp

../build/linux/release/obj/SignalHysteresis.o: src/SignalHysteresis.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SignalHysteresis.o src/SignalHysteresis.cpp

../build/linux/release/obj/SignalKeyPoints.o: src/SignalKeyPoints.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SignalKeyPoints.o src/SignalKeyPoints.cpp

../build/linux/release/obj/SignalMedianFilter.o: src/SignalMedianFilter.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SignalMedianFilter.o src/SignalMedianFilter.cpp

../build/linux/release/obj/SignalNormalize.o: src/SignalNormalize.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SignalNormalize.o src/SignalNormalize.cpp

../build/linux/release/obj/SignalOtsuThreshold.o: src/SignalOtsuThreshold.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SignalOtsuThreshold.o src/SignalOtsuThreshold.cpp

../build/linux/release/obj/SortingBinarySearch.o: src/SortingBinarySearch.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SortingBinarySearch.o src/SortingBinarySearch.cpp

../build/linux/release/obj/SortingSort.o: src/SortingSort.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SortingSort.o src/SortingSort.cpp

../build/linux/release/obj/SpatialFeature.o: src/SpatialFeature.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SpatialFeature.o src/SpatialFeature.cpp

../build/linux/release/obj/SquareEuclideanDistanceFunction.o: src/SquareEuclideanDistanceFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SquareEuclideanDistanceFunction.o src/SquareEuclideanDistanceFunction.cpp

../build/linux/release/obj/StatisticsAverage.o: src/StatisticsAverage.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsAverage.o src/StatisticsAverage.cpp

../build/linux/release/obj/StatisticsBaddeley.o: src/StatisticsBaddeley.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsBaddeley.o src/StatisticsBaddeley.cpp

../build/linux/release/obj/StatisticsDice.o: src/StatisticsDice.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsDice.o src/StatisticsDice.cpp

../build/linux/release/obj/StatisticsJaccard.o: src/StatisticsJaccard.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsJaccard.o src/StatisticsJaccard.cpp

../build/linux/release/obj/StatisticsKappa.o: src/StatisticsKappa.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsKappa.o src/StatisticsKappa.cpp

../build/linux/release/obj/StatisticsMAD.o: src/StatisticsMAD.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsMAD.o src/StatisticsMAD.cpp

../build/linux/release/obj/StatisticsObsAgree.o: src/StatisticsObsAgree.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsObsAgree.o src/StatisticsObsAgree.cpp

../build/linux/release/obj/StatisticsPosNeg.o: src/StatisticsPosNeg.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsPosNeg.o src/StatisticsPosNeg.cpp

../build/linux/release/obj/StatisticsStdDev.o: src/StatisticsStdDev.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/StatisticsStdDev.o src/StatisticsStdDev.cpp

../build/linux/release/obj/SumPathFunction.o: src/SumPathFunction.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/SumPathFunction.o src/SumPathFunction.cpp

../build/linux/release/obj/Superpixel.o: src/Superpixel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Superpixel.o src/Superpixel.cpp

../build/linux/release/obj/Table.o: src/Table.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/Table.o src/Table.cpp

../build/linux/release/obj/TransformEuclDist.o: src/TransformEuclDist.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/TransformEuclDist.o src/TransformEuclDist.cpp

../build/linux/release/obj/TransformEuclDistInv.o: src/TransformEuclDistInv.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ../build/linux/release/obj/TransformEuclDistInv.o src/TransformEuclDistInv.cpp
