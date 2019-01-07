TARGET = bial
TEMPLATE = lib
#CONFIG += shared_and_static build_all
CONFIG += c++11

CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-function -Wno-unused-parameter \
-I$$PWD/inc -I$$PWD/src -I$$PWD/lsh/inc -I$$PWD/SLIC/inc -I$$PWD/description/inc\
-I$$PWD/cpplex/inc -I$$PWD/zlib -DREAL_FLOAT -DBIAL_$(basename $(notdir $(@)))

CONFIG(release, release|debug|profile) : message( "Release 1" )

CONFIG(profile, profile|release|debug) : CONFIG+=static
CONFIG(profile, profile|release|debug) : message( "Profile 1" )

CONFIG(debug, debug|release|profile) : QMAKE_CXXFLAGS_DEBUG += -DBIAL_DEBUG=1
CONFIG(debug, debug|release|profile) : message( "Debug 1" )

#-I$$PWD/SLIC/inc

win32{
QMAKE_CXXFLAGS += -fopenmp
}

unix: !macx{
QMAKE_CXXFLAGS += -fopenmp -std=c++11
QMAKE_LFLAGS += -std=c++11
}

#QMAKE_CXXFLAGS += -DBIAL_IMPLICIT_BIN
QMAKE_CXXFLAGS += -DBIAL_EXPLICIT_LIB

#DEFINES += BIAL_IMPLICIT_BIN
DEFINES += BIAL_EXPLICIT_LIB

QMAKE_CXXFLAGS_RELEASE -= -O2 -O0
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE -= -pg

QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_DEBUG -= -O2 -O3
QMAKE_CXXFLAGS_DEBUG += -pg
QMAKE_LFLAGS += -pg
macx{
QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++17
QMAKE_LFLAGS += -stdlib=libc++
}


QMAKE_CXXFLAGS -= -pipe


LIBS += -lm
QMAKE_LFLAGS += -lm


win32{
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

Release:DESTDIR = $$PWD/../build/win/release/lib/
Release:OBJECTS_DIR = $$PWD/../build/win/release/obj

Debug:DESTDIR = $$PWD/../build/win/debug/lib/
Debug:OBJECTS_DIR = $$PWD/../build/win/debug/obj
}

unix: !macx{
QMAKE_CXXFLAGS += -fopenmp -std=c++11
QMAKE_LFLAGS += -fopenmp -std=c++11
}

macx{
QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
QMAKE_LFLAGS += -stdlib=libc++

Release:DESTDIR = $$PWD/../build/mac/release/lib/
Release:OBJECTS_DIR = $$PWD/../build/mac/release/obj

Debug:DESTDIR = $$PWD/../build/mac/debug/lib/
Debug:OBJECTS_DIR = $$PWD/../build/mac/debug/obj
}

unix{

CONFIG(release, release|debug|profile):message( "Release Dir" )
CONFIG(release, release|debug|profile):DESTDIR = $$PWD/../build/linux/release/lib/
CONFIG(release, release|debug|profile):OBJECTS_DIR = $$PWD/../build/linux/release/obj

CONFIG(debug, debug|release|profile):message( "Debug Dir" )
CONFIG(debug, debug|release|profile):DESTDIR = $$PWD/../build/linux/debug/lib/
CONFIG(debug, debug|release|profile):OBJECTS_DIR = $$PWD/../build/linux/debug/obj

CONFIG(profile, profile|debug|release):message( "Profile Dir" )
CONFIG(profile, profile|debug|release):DESTDIR = $$PWD/../build/linux/profile/lib/
CONFIG(profile, profile|debug|release):OBJECTS_DIR = $$PWD/../build/linux/profile/obj

}

QMAKE_EXTRA_TARGETS += createDirs

createDirs.commands = $(QMAKE_MKDIR) $DESTDIR $OBJECTS_DIR
bial.depends += createDirs



include(cpplex/cpplex.pri)
include(lsh/lsh.pri)
include(SLIC/SLIC.pri)
include(zlib/zlib.pri)
include(description/description.pri)

HEADERS += \
    inc/Adjacency.hpp \
    inc/AdjacencyGrayCode.hpp \
    inc/AdjacencyIterator.hpp \
    inc/AdjacencyMarchingCube.hpp \
    inc/AdjacencyRound.hpp \
    inc/AdjacencySquare.hpp \
    inc/Array.hpp \
    inc/BinaryCOG.hpp \
    inc/BinaryComplement.hpp \
    inc/Bit.hpp \
    inc/BrainCOG.hpp \
    inc/BrainInhomogeneity.hpp \
    inc/BrainIntensities.hpp \
    inc/BrainSkullStripping.hpp \
    inc/BrainTissueSegmentation.hpp \
    inc/BSpline.hpp \
    inc/BucketQueue.hpp \
    inc/BucketQueueElements.hpp \
    inc/ChessBoardDistanceFunction.hpp \
    inc/CityBlockDistanceFunction.hpp \
    inc/ClusteringIFT.hpp \
    inc/Color.hpp \
    inc/ColorChannel.hpp \
    inc/ColorFeature.hpp \
    inc/ColorHSI.hpp \
    inc/ColorLab.hpp \
    inc/ColorMedianFeature.hpp \
    inc/ColorRGB.hpp \
    inc/Common.hpp \
    inc/ConnPathFunction.hpp \
    inc/Correlation.hpp \
    inc/DegeneratedIFT.hpp \
    inc/DFIDE.hpp \
    inc/DicomHeader.hpp \
    inc/DiffPathFunction.hpp \
    inc/DiffusionFunction.hpp \
    inc/Display.hpp \
    inc/DistanceFunction.hpp \
    inc/DrawBox.hpp \
    inc/DrawCircle.hpp \
    inc/DrawFigure.hpp \
    inc/DrawIntersection.hpp \
    inc/DrawLine.hpp \
    inc/DrawPoint.hpp \
    inc/DrawSequence.hpp \
    inc/DrawShape.hpp \
    inc/DrawSVGGraph.hpp \
    inc/EdgeLazyWalk.hpp \
    inc/EdgeLiveWire.hpp \
    inc/EdgeMaxPathFunction.hpp \
    inc/EdgeRiverBed.hpp \
    inc/Errors.hpp \
    inc/EuclideanDistanceFunction.hpp \
    inc/FastBucketQueue.hpp \
    inc/FastDecreasingFifoBucketQueue.hpp \
    inc/FastDecreasingLifoBucketQueue.hpp \
    inc/FastIncreasingFifoBucketQueue.hpp \
    inc/FastIncreasingLifoBucketQueue.hpp \
    inc/Feature.hpp \
    inc/FeaturePathFunction.hpp \
    inc/FeatureResize.hpp \
    inc/File.hpp \
    inc/FileBMP.hpp \
    inc/FileDicom.hpp \
    inc/FileFeature.hpp \
    inc/FileImage.hpp \
    inc/FileMatrix.hpp \
    inc/FileNifti.hpp \
    inc/FilePNM.hpp \
    inc/FileScene.hpp \
    inc/FileSignal.hpp \
    inc/FilteringAnisotropicDiffusion.hpp \
    inc/FilteringGaussian.hpp \
    inc/FilteringMean.hpp \
    inc/FilteringMedian.hpp \
    inc/FilteringOptimalAnisotropicDiffusion.hpp \
    inc/Fourier.hpp \
    inc/FuzzyCMeans.hpp \
    inc/GDCM.hpp \
    inc/GeodesicPathFunction.hpp \
    inc/Geometrics.hpp \
    inc/GeometricsAffine.hpp \
    inc/GeometricsRotate.hpp \
    inc/GeometricsScale.hpp \
    inc/GeometricsShear.hpp \
    inc/GeometricsTranslate.hpp \
    inc/GeoSumPathFunction.hpp \
    inc/GradientCanny.hpp \
    inc/GradientDirectional.hpp \
    inc/GradientGabor.hpp \
    inc/GradientMorphological.hpp \
    inc/GradientPosOperations.hpp \
    inc/GradientScaleCanny.hpp \
    inc/GradientSobel.hpp \
    inc/Graph.hpp \
    inc/GraphAdjacency.hpp \
    inc/GrowingBucketQueue.hpp \
    inc/gzstream.hpp \
    inc/HeartCOG.hpp \
    inc/HeartSegmentation.hpp \
    inc/HierarchicalGraph.hpp \
    inc/HierarchicalPathFunction.hpp \
    inc/Histogram.hpp \
    inc/HoughCircle.hpp \
    inc/Image.hpp \
    inc/ImageEquals.hpp \
    inc/ImageFrame.hpp \
    inc/ImageIFT.hpp \
    inc/ImageInterpolation.hpp \
    inc/ImageMerge.hpp \
    inc/ImageResize.hpp \
    inc/ImageROI.hpp \
    inc/ImageSplit.hpp \
    inc/ImageSwapDims.hpp \
    inc/InfBucketQueue.hpp \
    inc/Integral.hpp \
    inc/IntensityGlobals.hpp \
    inc/IntensityLocals.hpp \
    inc/Kernel.hpp \
    inc/KernelBox.hpp \
    inc/KernelGabor.hpp \
    inc/KernelGaussian.hpp \
    inc/KernelRound.hpp \
    inc/KernelSobel.hpp \
    inc/KernelSquare.hpp \
    inc/KnnGraphAdjacency.hpp \
    inc/LocalMaxPathFunction.hpp \
    inc/LSHGraphAdjacency.hpp \
    inc/MarchingCubes.hpp \
    inc/Matrix.hpp \
    inc/MatrixDeterminant.hpp \
    inc/MatrixIdentity.hpp \
    inc/MatrixInverse.hpp \
    inc/MatrixPolar.hpp \
    inc/MatrixResize.hpp \
    inc/MatrixTranspose.hpp \
    inc/MaxPathFunction.hpp \
    inc/MaxSumPathFunction.hpp \
    inc/MedianFeature.hpp \
    inc/MinimumSpanningTree.hpp \
    inc/MinPathFunction.hpp \
    inc/MorphologyDilation.hpp \
    inc/MorphologyErosion.hpp \
    inc/MRIModality.hpp \
    inc/MultiImage.hpp \
    inc/NiftiHeader.hpp \
    inc/OPFClusterMatching.hpp \
    inc/OPFHierarchicalClustering.hpp \
    inc/OPFSpatialClustering.hpp \
    inc/OPFSpectralClustering.hpp \
    inc/OrientedExternPathFunction.hpp \
    inc/OrientedInternPathFunction.hpp \
    inc/PathFunction.hpp \
    inc/PixelInterpolation.hpp \
    inc/Plotting.hpp \
    inc/PNMHeader.hpp \
    inc/PreEuclideanDistanceFunction.hpp \
    inc/RandomQueue.hpp \
    inc/RealColor.hpp \
    inc/Sample.hpp \
    inc/SampleRandom.hpp \
    inc/SampleUniform.hpp \
    inc/SegmentationBackground.hpp \
    inc/SegmentationBorder.hpp \
    inc/SegmentationConnectedComponents.hpp \
    inc/SegmentationFSum.hpp \
    inc/SegmentationGeoStar.hpp \
    inc/SegmentationHysteresis.hpp \
    inc/SegmentationIntensityThreshold.hpp \
    inc/SegmentationMST.hpp \
    inc/SegmentationOctants.hpp \
    inc/SegmentationOtsuThreshold.hpp \
    inc/SegmentationQuadrants.hpp \
    inc/SegmentationSurface.hpp \
    inc/SegmentationWatershed.hpp \
    inc/Signal.hpp \
    inc/SignalAccumulate.hpp \
    inc/SignalEmptyBins.hpp \
    inc/SignalEqualize.hpp \
    inc/SignalHysteresis.hpp \
    inc/SignalKeyPoints.hpp \
    inc/SignalMedianFilter.hpp \
    inc/SignalNormalize.hpp \
    inc/SignalOtsuThreshold.hpp \
    inc/SortingBinarySearch.hpp \
    inc/SortingSort.hpp \
    inc/SpatialFeature.hpp \
    inc/SquareEuclideanDistanceFunction.hpp \
    inc/StatisticsAverage.hpp \
    inc/StatisticsBaddeley.hpp \
    inc/StatisticsDice.hpp \
    inc/StatisticsJaccard.hpp \
    inc/StatisticsKappa.hpp \
    inc/StatisticsMAD.hpp \
    inc/StatisticsObsAgree.hpp \
    inc/StatisticsPosNeg.hpp \
    inc/StatisticsStdDev.hpp \
    inc/SumPathFunction.hpp \
    inc/Superpixel.hpp \
    inc/Table.hpp \
    inc/TransformEuclDist.hpp \
    inc/TransformEuclDistInv.hpp \
    inc/Vector.hpp \
    inc/VideoIO.hpp \
    inc/SimpleBucketQueue.hpp \
    inc/RotatingBucketQueue.hpp \
    inc/StatisticsSSIM.hpp \
    inc/StatisticsPSNR.hpp \
    inc/StatisticsCovariance.hpp \
    inc/StatisticsMeanSquareError.hpp \
    inc/StatisticsQILV.hpp \
    inc/StatisticsIQI.hpp \
    inc/StatisticsEntropyDifference.hpp \
    inc/AdjacencySequential.hpp \
    inc/StatisticsGRBAC.hpp

SOURCES += \
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
    src/TransformEuclDistInv.cpp \
    src/SimpleBucketQueue.cpp \
    src/RotatingBucketQueue.cpp \
    src/StatisticsSSIM.cpp \
    src/StatisticsCovariance.cpp \
    src/StatisticsPSNR.cpp \
    src/StatisticsMeanSquareError.cpp \
    src/StatisticsQILV.cpp \
    src/StatisticsEntropyDifference.cpp \
    src/StatisticsIQI.cpp \
    src/AdjacencySequential.cpp \
    src/StatisticsGRBAC.cpp

DISTFILES += \
    uncrustify.cfg

#TODO: Verify if FFMPEG is available

#HEADERS += inc/FFmpegIO.hpp
#SOURCES += src/FFmpegIO.cpp
#LIBS += -lavutil -lavformat -lavcodec -lswscale
#QMAKE_LFLAGS += -lavutil -lavformat -lavcodec -lswscale

## INSTALL LIBRARY
headersDataFiles.path = $$[QT_INSTALL_HEADERS]/bial/
headersDataFiles.files = $$PWD/inc/*.hpp
INSTALLS += headersDataFiles

libraryFiles.path = $$[QT_INSTALL_LIBS]
Debug:libraryFiles.files = $$OUT_PWD/debug/*.a $$OUT_PWD/debug/*.prl
Release:libraryFiles.files = $$OUT_PWD/release/*.a $$OUT_PWD/release/*.prl
INSTALLS += libraryFiles
