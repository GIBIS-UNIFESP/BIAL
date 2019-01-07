/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @file NiftiHeader.hpp
 * @author Fábio Augusto Menocci Cappabianco
 * @date 2012/Jul/02
 * @version 1.0.00
 * @brief
 * Content: NiftiHeader class
 * <br> Description: Magnetic Resonance Image class.
 * <br> Future add-on's: Complete nifti header extension support and analyze 7.5 support.
 */

/* This implementation was based on Robert W Cox, SSCC/DIRP/NIMH/NIH/DHHS/USA/EARTH c code.
 * Source codes: nifti1_io.h, nifti1.h, and nifti1_io.c
 */

#include "Array.hpp"
#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#ifndef BIALNIFTIHEADER_H
#define BIALNIFTIHEADER_H

namespace Bial {

  class OFile;
  template< class D >
  class Image;
  union Color;
  class RealColor;

  /* @brief
   * NIFTI1 constants. Meaning extracted from nifti1.h header file
   * from the Official definition of the nifti1 header. Written by Bob
   * Cox, SSCC, NIMH..
   */
  enum class NiftiType : short { /* NIFTI1_DATATYPES: nifti1 datatype codes  */
    UINT8 = 2,
      INT16 = 4,
      INT32 = 8,
      FLOAT32 = 16,
      FLOAT64 = 64,
      INT8 = 256,
      UINT16 = 512,
      UINT32 = 768,
      INT64 = 1024,
      UINT64 = 1280
      };

  /** @brief  NIFTI1_INTENT: nifti1 intent codes, to describe intended meaning of dataset contents. */
  enum class NiftiIntent : short {
    NONE = 0,
      CORREL = 2,
      TTEST = 3, /* Student t statistic (1 param): p1 = DOF. */
      FTEST = 4, /* Fisher F statistic (2 params): p1 = numerator DOF, p2 = denominator DOF. */
      ZSCORE = 5, /* Standard normal (0 params): Density = N(0,1). */
      CHISQ = 6, /* Chi-squared (1 param): p1 = DOF. Density(x) proportional to exp(-x/2) * x^(p1/2-1). */
      BETA = 7, /* Beta distribution (2 params): p1=a, p2=b. Density(x) proportional to x^(a-1) * (1-x)^(b-1).
     * Binomial distribution (2 params): p1 = number of trials, p2 = probability per trial.  Prob(x) = (p1 choose x) *
     * p2^x * (1-p2)^(p1-x), for x=0,1,...,p1.
     */
      BINOM = 8,
    /* Gamma distribution (2 params): p1 = shape, p2 = scale. Density(x) proportional to x^(p1-1) * exp(-p2*x). */
      GAMMA = 9,
      POISSON = 10, /* Poisson distribution (1 param): p1 = mean. Prob(x) = exp(-p1) * p1^x / x!, for x=0,1,2,....
         *
         */
      NORMAL = 11, /* Normal distribution (2 params): p1 = mean, p2 = standard deviation.
        * Noncentral F statistic (3 params): p1 = numerator DOF, p2 = denominator DOF, p3 = numerator noncentrality
        * parameter.
        */
      FTEST_NONC = 12,
      CHISQ_NONC = 13, /* Noncentral chi-squared statistic (2 params): p1 = DOF, p2 = noncentrality parameter.
      * Logistic distribution (2 params): p1 = location, p2 = scale. Density(x) proportional to sech^2((x-p1)/(2*p2)).
      */
      LOGISTIC = 14,
    /* Laplace distribution (2 params): p1 = location, p2 = scale. Density(x) proportional to exp(-abs(x-p1)/p2). */
      LAPLACE = 15,
      UNIFORM = 16, /* Uniform distribution: p1 = lower end, p2 = upper end. */
      TTEST_NONC = 17, /* Noncentral t statistic (2 params): p1 = DOF, p2 = noncentrality parameter.
      * Weibull distribution (3 params): p1 = location, p2 = scale, p3 = power. Density(x) proportional to
      * ((x-p1)/p2)^(p3-1) exp(-((x-p1)/p2)^p3) for x > p1.
      */
      WEIBULL = 18,
    /* Chi distribution (1 param): p1 = DOF. Density(x) proportional to x^(p1-1) * exp(-x^2/2) for x > 0. p1 = 1 = 'half
     * normal'.  distribution p1 = 2 = Rayleigh distribution p1 = 3 = Maxwell-Boltzmann
     * distribution.
     */
      CHI = 19,
    /* Inverse Gaussian (2 params): p1 = mu, p2 = lambda Density(x) proportional to exp(-p2*(x-p1)^2/(2*p1^2*x)) / x^3
     * for x > 0.
     */
      INVGAUSS = 20,
      EXTVAL = 21, /* Extreme value type I (2 params): p1 = location, p2 = scale cdf(x) = exp(-exp(-(x-p1)/p2)). */
      PVAL = 22, /* Data is a 'p-value' (no params).
      * Data is ln(p-value) (no params). To be safe, a program should compute p = exp(-abs(this_value)). The
      * nifti_stats.c library returns this_value as positive, so that this_value = -log(p).
      */
      LOGPVAL = 23,
    /* Data is log10(p-value) (no params). To be safe, a program should compute p = pow(10.,-abs(this_value)). The
     * nifti_stats.c library returns this_value as positive, so that this_value = -log10(p).
     */
      LOG10PVAL = 24,
      FIRST_STAT = 2, /* Smallest intent_code that indicates a statistic. */
      LAST_STAT = 24, /* Largest intent_code that indicates a statistic.
           * To signify that the value at each voxel is an estimate of some parameter, set intent_code = ESTIMATE. The name of
           * the parameter may be stored in intent_name.
           */
      ESTIMATE = 1001,
    /* To signify that the value at each voxel is an index into some set of labels, set intent_code = LABEL. The
     * filename with the labels may stored in aux_file.
     */
      LABEL = 1002,
    /* To signify that the value at each voxel is an index into the NeuroNames labels set, set intent_code =
     * NEURONAME.
     */
      NEURONAME = 1003,
      GENMATRIX = 1004, /* To store an M x N matrix at each voxel:
       * - dataset must have a 5th dimension (dim[0]=5 and dim[5]>1)
       * - intent_code must be GENMATRIX - dim[5] must be M*N
       * - intent_p1 must be M (in float format)
       * - intent_p2 must be N (ditto)
       * - the matrix values A[i][[j] are stored in row-order:
       * - A[0][0] A[0][1] ... A[0][N-1]
       * - A[1][0] A[1][1] ... A[1][N-1]
       * - etc., until
       * - A[M-1][0] A[M-1][1] ... A[M-1][N-1]
       */
      SYMMATRIX = 1005, /* To store an NxN symmetric matrix at each voxel:
       * - dataset must have a 5th dimension
       * - intent_code must be SYMMATRIX
       * - dim[5] must be N*(N+1)/2
       * - intent_p1 must be N (in float format)
       * - the matrix values A[i][[j] are stored in row-order:
       * - A[0][0]
       * - A[1][0] A[1][1]
       * - A[2][0] A[2][1] A[2][2]
       * - etc.: row-by-row
       */
      DISPVECT = 1006, /* To signify that the vector value at each voxel is to be taken as a displacement field
      * or vector:
      * - dataset must have a 5th dimension_code must be DISPVECT
      * - dim[5] must be the dimensionality of the displacment vector (e.g., 3 for spatial
      * displacement, 2 for in-plane). Specifically for displacements
      */
      VECTOR = 1007, /* To signify that the vector value at each voxel is to be taken as a displacement field
          * or vector:
          * - dataset must have a 5th dimension_code must be DISPVECT
          * - dim[5] must be the dimensionality of the displacment vector (e.g., 3 for spatial
          * displacement, 2 for in-plane). For any other type of vector
          */
      POINTSET = 1008, /* To signify that the vector value at each voxel is really a spatial coordinate
      * (e.g., the vertices or nodes of a surface mesh):
      * - dataset must have a 5th dimension
      * - intent_code must be POINTSET
      * - dim[0] = 5
      * - dim[1] = number of points
      * - dim[2] = dim[3] = dim[4] = 1
      * - dim[5] must be the dimensionality of space (e.g., 3 => 3D space).
      * - intent_name may describe the object these points come from  (e.g., "pial", "gray/white",
      * "EEG", "MEG").
      */
      TRIANGLE = 1009, /* To signify that the vector value at each voxel is really a triple of indexes (e.g., forming
      * a triangle) from a pointset dataset:
      * - dataset must have a 5th dimension
      * - intent_code must be TRIANGLE
      * - dim[0] = 5
      * - dim[1] = number of triangles
      * - dim[2] = dim[3] = dim[4] = 1
      * - dim[5] = 3 should be an integer type (preferably DT_INT32)
      * - the data values are indexes (0,1,...) into a pointset dataset.
      */
      QUATERNION = 1010, /* To signify that the vector value at each voxel is a quaternion:
        * - dataset must have a 5th dimension
        * - intent_code must be QUATERNION
        * - dim[0] = 5
        * - dim[5] = 4
        * - datatype should be a floating point type.
        */
      DIMLESS = 1011, /* Dimensionless value: no params although, as in _ESTIMATE the name of the parameter may be
           * stored in intent_name.
           */
      TIME_SERIES = 2001, /* GIFTI datasets: To signify that the value at each location is from a time series. */
      NODE_INDEX = 2002, /* GIFTI datasets: To signify that the value at each location is a node index, from a complete
        * surface dataset.
        */
      RGB_VECTOR = 2003, /* GIFTI datasets: To signify that the vector value at each location is an RGB triplet, of
        * whatever type.
        * - dataset must have a 5th dimension
        * - dim[0] = 5
        * - dim[1] = number of nodes
        * - dim[2] = dim[3] = dim[4] = 1
        * - dim[5] = 3
        */
      RGBA_VECTOR = 2004, /* GIFTI datasets: To signify that the vector value at each location is a 4 valued RGBA
         * vector, of whatever type.
         * - dataset must have a 5th dimension
         * - dim[0] = 5
         * - dim[1] = number of nodes
         * - dim[2] = dim[3] = dim[4] = 1
         * - dim[5] = 4
         */
      SHAPE = 2005 /* GIFTI datasets: To signify that the value at each location is a shape value, such as
        * the curvature.
        */
      };

  /** @brief  NIFTI1_XFORM_CODES: nifti1 xform codes to describe the "standard" x,y,z coordinate system. */
  enum class NiftiXForm : short {
    UNKNOWN = 0, /* Arbitrary coordinates. */
      SCANNER_ANAT = 1, /* Scanner-based anatomical coordinates */
      ALIGNED_ANAT = 2, /* Coordinates aligned to another file's, or to anatomical "truth". */
      TALAIRACH = 3, /* Coordinates aligned to Talairach-Tournoux Atlas; (0,0,0)=AC, etc. */
      MNI_152 = 4 /* MNI 152 normalized coordinates. */
      };

  /** @brief  NIFTI1_UNITS: nifti1 units codes to describe the unit of measurement for each dimension of the dataset. */
  enum class NiftiUnit : char {
    UNKNOWN = 0, /* NIFTI code for unspecified units. */
      METER = 1, /* NIFTI code for meters. */
      MM = 2, /* NIFTI code for millimeters. */
      MICRON = 3, /* NIFTI code for micrometers. */
      SEC = 8, /* NIFTI code for seconds. */
      MSEC = 16, /* NIFTI code for milliseconds. */
      USEC = 24, /* NIFTI code for microseconds. */
      HZ = 32, /* NIFTI code for Hertz. */
      PPM = 40, /* NIFTI code for ppm. */
      RADS = 48 /* NIFTI code for radians per second. */
      };

  /** @brief  NIFTI1_SLICE_ORDER: nifti1 slice order codes, describing the acquisition order of the slices. */
  enum class NiftiSlice : char {
    UNKNOWN = 0,
      SEQ_INC = 1,
      SEQ_DEC = 2,
      ALT_INC = 3,
      ALT_DEC = 4,
      ALT_INC2 = 5,
      ALT_DEC2 = 6
      };

  /**
   * @brief Magnetic Resonance Image class.
   */
  class NiftiHeader {
    template< class D >
    friend class Image;
  public:
    static const short NIFTI_HEADER_SIZE = 348;
    static const short ANALYZE_EXTENT = 16384;
  private:
    /** @brief  Analyze 7.5: data_type[ 10 ]                                            */
    std::string data_type;
    /** @brief  Analyze 7.5: char db_name[ 18 ]                                         */
    std::string db_name;
    /** @brief  Analyze 7.5: extents.                                                   */
    int extents;
    /** @brief  Analyze 7.5: session_error.                                             */
    short session_error;
    /** @brief  Analyze 7.5: regular.                                                   */
    char regular;
    /** @brief  Nifti1: indexes (1,2,3, or 0) for MRI                                   */
    char freq_dim;
    /** @brief  Nifti1: directions in dim[ ] / pixdim[ ]                                */
    char phase_dim;
    /** @brief  Nifti1: directions in dim[ ] / pixdim[ ]                                */
    char slice_dim;
    /** @brief  Nifti1: Data array dimensions dim[ 8 ]                                  */
    Vector< size_t > dim;
    /** @brief  Nifti1: 1st intent parameter.                                           */
    float intent_p1;
    /** @brief  Nifti1: 2nd intent parameter.                                           */
    float intent_p2;
    /** @brief  Nifti1: 3rd intent parameter.                                           */
    float intent_p3;
    /** @brief  Nifti1: NiftiIntent::* code.                                            */
    NiftiIntent intent_code;
    /** @brief  Both:   Defines data type.                                              */
    NiftiType datatype;
    /** @brief  Both:   bits/voxel.                                                     */
    short bitpix;
    /** @brief  Nifti1: First slice index.                                              */
    short slice_start;
    /** @brief  Both:   Grid spacings. pixdim[ 8 ]                                      */
    Vector< float > pixdim;
    /** @brief  Both:   Offset into .nii file.                                          */
    float vox_offset;
    /** @brief  Nifti1: Data scaling slope.                                             */
    float scl_slope;
    /** @brief  Nifti1: Data scaling offset.                                            */
    float scl_inter;
    /** @brief  Nifti1: Last slice index.                                               */
    short slice_end;
    /** @brief  Nifti1: Slice timing order.                                             */
    NiftiSlice slice_code;
    /** @brief  Nifti1: dx,dy,dz units: NiftiUnit::* code                               */
    NiftiUnit xyz_units;
    /** @brief  Nifti1: dt       units: NiftiUnit::* code                               */
    NiftiUnit time_units;
    /** @brief  Both:   Max display intensity.                                          */
    float cal_max;
    /** @brief  Both:   Min display intensity.                                          */
    float cal_min;
    /** @brief  Nifti1: Time for 1 slice. Analyze 7.5: compressed.                      */
    float slice_duration;
    /** @brief  Nifti1: Time axis shift.  Analyze 7.5: verified.                        */
    float toffset;
    /** @brief  Analyze 7.5: glmax.                                                     */
    int glmax;
    /** @brief  Analyze 7.5: glmin.                                                     */
    int glmin;
    /** @brief  Nifti1: Anything.         Analyze 7.5: descrip[ 80 ]                    */
    std::string descrip;
    /** @brief  Both:   Auxiliary filename. aux_file[24]                                */
    std::string aux_file;
    /** @brief  NIFTI1: NiftiXForm::* code.                                             */
    NiftiXForm qform_code;
    /** @brief  NIFTI1: NiftiXForm::* code.                                             */
    NiftiXForm sform_code;
    /** @brief  NIFTI1: Quaternion b param.                                             */
    float quatern_b;
    /** @brief  NIFTI1: Quaternion c param.                                             */
    float quatern_c;
    /** @brief  NIFTI1: Quaternion d param.                                             */
    float quatern_d;
    /** @brief  NIFTI1: Quaternion x shift.                                             */
    float qoffset_x;
    /** @brief  NIFTI1: Quaternion y shift.                                             */
    float qoffset_y;
    /** @brief  NIFTI1: Quaternion z shift.                                             */
    float qoffset_z;
    /** @brief  NIFTI1: Auaternion transform parameters [used for qform, NOT qto_xyz]   */
    float qfac;
    /** @brief  NIFTI1: qform: transform (i, j, k) to (x, y, z) 4 x 4                   */
    Matrix< float > qtm;
    /** @brief  NIFTI1: sform: transform (i, j, k) to (x, y, z) 4 x 4                   */
    Matrix< float > stm;
    /** @brief  NIFTI1: 'name' or meaning of data. intent_name[ 16 ]                    */
    std::string intent_name;

    /**
     * @date 2012/jul/31
     * @param none.
     * @return none.
     * @brief Sets bitpix field according to datatype.
     * @warning none.
     */
    void SetBitPix( );

    /**
     * @date 2012/jul/31
     * @param num: A number with the data type to be decoded.
     * @return NIFTI code for the data type of num.
     * @brief Returns NIFTI code for the data type of num. This function and all other overloaded are used by
     * WriteNifti1 to identify the type of the data to be written.
     * @warning none.
     */
    static NiftiType DataTypeDecode( const int );
    static NiftiType DataTypeDecode( const llint );
    static NiftiType DataTypeDecode( const float );
    static NiftiType DataTypeDecode( const double );
    static NiftiType DataTypeDecode( const Color );
    static NiftiType DataTypeDecode( const RealColor );

    /**
     * @date 2012/jul/31
     * @param size: number of elements to be swapped.
     * @param data: data to be swapped data.
     * @return none.
     * @brief Swaps bytes of data pointed by 'data'. Used to read data written by different architectures.
     * @warning none.
     */
    static void Swap2Bytes( size_t size, void *data );
    static void Swap4Bytes( size_t size, void *data );
    static void Swap8Bytes( size_t size, void *data );
    static void Swap16Bytes( size_t size, void *data );

    /**
     * @date 2012/jul/31
     * @param none.
     * @return none.
     * @brief Swaps bytes of 'this' header. Used to read data written by different architectures.
     * @warning none.
     */
    void SwapHeader( );

  public:

    /**
     * @date 2013/Fev/01
     * @param none.
     * @return none.
     * @brief Basic Constructor.
     * @warning All nifti fields are uninitialized or undefined.
     */
    NiftiHeader( );

    /**
     * @date 2012/Aug/02
     * @param new_dim: Vector with MR image dimensions.
     * @param new_pixel_dim: array with pixel dimensions.
     * @param new_datatype: nifti header data type.
     * @param one_file: defines weather it is a header from one or two files image.
     * @return none.
     * @brief Basic Constructor.
     * @warning All nifti fields, except dim, pixdim, and datatype, are uninitialized.
     */
    NiftiHeader( const Vector< size_t > &new_dim, const Vector< float > &new_pixdim, 
                 NiftiType new_datatype = NiftiType::INT8, bool one_file = true );

    /**
     * @date 2012/Aug/20
     * @param img: base image to extract dimensions and datatype.
     * @param one_file: defines weather it is a header from one or two files image.
     * @return none.
     * @brief Basic Constructor.
     * @warning All nifti fields, except dim, pixdim, and datatype, are uninitialized.
     */
    template< class D > 
    NiftiHeader( const Image< D > &img, bool one_file = true );

    /**
     * @date 2013/Aug/10
     * @param filename: File name.
     * @return none.
     * @brief Basic Constructor.
     * @warning none.
     */
    NiftiHeader( const std::string &filename );

    /**
     * @date 2015/Oct/30
     * @param none.
     * @return Reference to dimension vector.
     * @brief Returns a reference to dimension vector.
     * @warning none.
     */
    const Vector< size_t > &Dim( ) const;

    /**
     * @date 2016/Apr/14
     * @param dms: The dimension of desired size.
     * @return The size of a given image dimension.
     * @brief Returns the size of a given image dimension.
     * @warning none.
     */
    size_t Size( size_t dms ) const;

    /**
     * @date 2016/Apr/14
     * @param dms: The dimension of desired size.
     * @param val: New size of the dimension.
     * @return none.
     * @brief Updates image dimension size.
     * @warning none.
     */
    void Size( size_t dms, size_t val );

    /**
     * @date 2015/Oct/30
     * @param none.
     * @return Reference to pixel dimension vector.
     * @brief Returns a reference to pixel dimension vector.
     * @warning none.
     */
    const Vector< float > &PixelSize( ) const;

    /**
     * @date 2016/Apr/14
     * @param dms: The dimension of desired pixel size.
     * @return The pixel size of a given image dimension.
     * @brief Returns the pixel size of a given image dimension.
     * @warning none.
     */
    float PixelSize( size_t dms ) const;

    /**
     * @date 2016/Apr/14
     * @param dms: The dimension of desired size.
     * @param val: New size of the pixel dimension.
     * @return none.
     * @brief Updates Pixel dimension size.
     * @warning none.
     */
    void PixelSize( size_t dms, float val );

    /**
     * @date 2015/Oct/30
     * @param none.
     * @return Number of bits per pixel.
     * @brief Returns the number of bits per pixel.
     * @warning none.
     */
    short BitPix( ) const;

    /**
     * @date 2015/Oct/30
     * @param none.
     * @return The data type code.
     * @brief Returns data type code.
     * @warning none.
     */
    NiftiType DataType( ) const;

    /**
     * @date 2013/Aug/20
     * @param filename: A nifti extension file name.
     * @return Nifti header object.
     * @brief Returns a nifti header read from file with filename 'filename'.
     * @warning none.
     */
    static NiftiHeader Read( const std::string &filename );

    /**
     * @date 2013/Aug/20
     * @param file: Opened file for outputting.
     * @param one_file: If it is nii or hdr+img file.
     * @return none.
     * @brief Writes this nifti header to file.
     * @warning 'file' is not closed. This function is necessary because gzstream does not have random access.
     */
    void Write( OFile &file, bool one_file ) const;

    /**
     * @date 2013/Aug/20
     * @param filename: A nifti extension file name.
     * @return none.
     * @brief Writes this nifti header to file with filename 'filename'.
     * @warning none.
     */
    void Write( const std::string &filename ) const;

    /**
     * @date 2014/May/27
     * @param filename: A file name.
     * @return True if given file has nifti extension.
     * @brief Returns true if given file has nifti extension.
     * @warning none.
     */
    static bool IsNiftiFile( const std::string &filename );

    /**
     * @date 2013/Sep/25
     * @param filename: A nifti extension file name.
     * @return Nifti header file name associated to filename.
     * @brief Returns nifti header file name associated to filename.
     * @warning none.
     */
    static std::string HeaderFileName( const std::string &filename );

    /**
     * @date 2013/Sep/25
     * @param filename: A nifti extension file name.
     * @return Nifti data file name associated to filename.
     * @brief Returns nifti data file name associated to filename.
     * @warning none.
     */
    static std::string DataFileName( const std::string &filename );

    /**
     * @date 2013/Aug/11
     * @param filename: A nifti extension file name.
     * @return Existing nifti header file name associated to filename.
     * @brief Returns existing nifti header file name associated to filename.
     * @warning none.
     */
    static std::string ExistingHeaderFileName( const std::string &filename );

    /**
     * @date 2013/Aug/11
     * @param filename: A nifti extension file name.
     * @return Existing nifti data file name associated to filename.
     * @brief Returns existing nifti data file name associated to filename.
     * @warning none.
     */
    static std::string ExistingDataFileName( const std::string &filename );


    /**
     * @date 2013/Aug/27
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     * or PA. Look for NiftiHeader::Orientation() function for examples.
     * @return The number of the image dimension that corresponds to axial direction.
     * @brief Returns the number of the image dimension that corresponds to axial direction.
     * @warning none.
     */
    static size_t AxialDimension( const std::string &orientation );

    /**
     * @date 2013/Aug/27
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     * or PA. Look for NiftiHeader::Orientation() function for examples.
     * @return The number of the image dimension that corresponds to coronal direction.
     * @brief Returns the number of the image dimension that corresponds to coronal direction.
     * @warning none.
     */
    static size_t CoronalDimension( const std::string &orientation );

    /**
     * @date 2013/Aug/27
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     * or PA. Look for NiftiHeader::Orientation() function for examples.
     * @return The number of the image dimension that corresponds to sagittal direction.
     * @brief Returns the number of the image dimension that corresponds to sagittal direction.
     * @warning none.
     */
    static size_t SagittalDimension( const std::string &orientation );

    /**
     * @date 2013/Aug/27
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     * or PA. Look for NiftiHeader::Orientation() function for examples.
     * @return True if axial dimension direction is superior-inferior.
     * @brief Returns true if axial dimension direction is superior-inferior.
     * @warning none.
     */
    static bool SuperiorInferior( const std::string &orientation );

    /**
     * @date 2014/Mar/14
     * @param orientation: Image orientation. Must be a composition of three values containing: LR, RL, SI, IS, AP,
     * or PA. Look for NiftiHeader::Orientation() function for examples.
     * @return True if coronal dimension direction is anterior-posterior.
     * @brief Returns true if coronal dimension direction is anterior-posterior.
     * @warning none.
     */
    static bool AnteriorPosterior( const std::string &orientation );

    /**
     * @date 2012/Jul/31
     * @param size: size of the swapped data.
     * @param bytes: number of bytes to be swapped.
     * @param data: pointer to data.
     * @return none.
     * @brief Swaps bytes of data pointed by data. Used to read data written by different architectures.
     * @warning none.
     */
    static void SwapNBytes( size_t size, size_t bytes, void *data );

    /**
     * @date 2013/Aug/21
     * @param filename: File name.
     * @return If header nifti file corresponding to filename must swap bytes order.
     * @brief Verifies if header nifti file corresponding to filename must swap bytes order.
     * @warning none.
     */
    static bool IsSwapped( const std::string filename );

    /**
     * @date 2013/Aug/20
     * @param type: data type.
     * @return none.
     * @brief Changes header information to support data of type 'type'.
     * @warning none.
     */
    void DataType( NiftiType type );

    /**
     * @date 2013/Aug/20
     * @param filename: A nifti extension file name.
     * @return Type of data in given file.
     * @brief Returns the type of the data in given file.
     * @warning none.
     */
    static NiftiType DataType( const std::string &filename );

    /**
     * @date 2012/jul/31
     * @param pixel_dim: dimensions of the pixel.
     * @return none.
     * @brief Translates quaterns to 4x4 matrix used to convert from xyz to ijk space.
     * @warning Quatern parameters must be set prior to this function call.
     */
    void QuaternToMat( const Vector< float > &pixel_dim );

    /**
     * @date 2012/Aug/03
     * @param none.
     * @return none.
     * @brief Translates 4x4 matrix used to convert from xyz to ijk space to quaterns.
     * @warning Quatern parameters must be set prior to this function call.
     */
    void MatToQuatern( );

    /**
     * @date 2012/Sep/08
     * @param row: The row of orientation.
     * @return A std::string with the orientation of the given row.
     * @brief Returned std::string can be: LR, RL, SI, IS, AP, PA, or Undefined.
     * @warning Looks for quatern form first. If it is undefined, searches in affine form.
     */
    std::string Orientation( size_t row ) const;

    /**
     * @date 2012/Sep/08
     * @param none.
     * @return A std::string with the orientation of the MR image.
     * @brief Returned std::string will be a composition of three values containing: LR, RL, SI, IS, AP, or PA, or
     * Undefined std::string.
     * @warning Looks for quatern form first. If it is undefined, searches in affine form.
     */
    std::string Orientation( ) const;

    /**
     * @date 2017/Apr/08
     * @param none.
     * @return scl_slope and scl_inter.
     * @brief Functions to access scl_slope and scl_inter parameters.
     * @warning none.
     */
    float SclSlope( ) const;
    float SclInter( ) const;

    /**
     * @date 2012/Sep/08
     * @param none.
     * @return Boolean value indicating if this MR image uses quaternion orientation form or not.
     * @brief Returns a boolean value indicating if this MR image uses quaternion orientation form or not.
     * @warning none.
     */
    bool UseQuatern( ) const;

    /**
     * @date 2012/Sep/08
     * @param none.
     * @return Boolean value indicating if this MR image uses affine orientation form or not.
     * @brief Returns a boolean value indicating if this MR image uses affine orientation form or not.
     * @warning none.
     */
    bool UseAffine( ) const;

    /**
     * @date 2012/Aug/03
     * @param mat: a matrix to be set to quatern matrix.
     * @return none.
     * @brief Assigns mat data to qtm.
     * @warning This method will change data orientation.
     */
    void QuaternMatrix( const Matrix< float > mat );

    /**
     * @date 2012/Aug/03
     * @param none.
     * @return A copy of qtm.
     * @brief returns a copy of qtm.
     * @warning none.
     */
    Matrix< float > QuaternMatrix( ) const;

    /**
     * @date 2012/Aug/03
     * @param x: x quatern offset.
     * @param y: y quatern offset.
     * @param z: z quatern offset.
     * @return none.
     * @brief Sets quatern offset.
     * @warning none.
     */
    void QuaternOffset( float x, float y, float z );

    /**
     * @date 2012/Aug/03
     * @param mat: a matrix to be set to affine transformation matrix.
     * @return none.
     * @brief Assigns mat data to stm.
     * @warning This method will change data orientation.
     */
    void AffineMatrix( const Matrix< float > mat );

    /**
     * @date 2012/Aug/03
     * @param none.
     * @return A copy of stm.
     * @brief returns a copy of stm.
     * @warning none.
     */
    Matrix< float > AffineMatrix( ) const;

    /* nifti_io based functions. */

    /**
     * @date 2012/jul/31
     * @param datatype: type of image data.
     * @param nbyper: bytes by data element.
     * @param swapsize: size of datas that may be swapped.
     * @return none.
     * @brief Computes the number of bytes of each element and the bytes to be swapped.
     * @warning none.
     */
    static void DataTypeSizes( int datatype, size_t &nbyper, size_t &swapsize );

    /**
     * @date 2012/Aug/06
     * @param os: An output stream.
     * @return A reference to the given output stream.
     * @brief Prints nifti orientation information to output stream 'os'.
     * @warning none.
     */
    template< class O >
    O &PrintOrientation( O &os ) const;

    /**
     * @date 2012/Aug/06
     * @param os: An output stream.
     * @return A reference to the given output stream.
     * @brief Prints nifti header information to output stream 'os'.
     * @warning none.
     */
    template< class O >
    O &Print( O &os ) const;

  };

  /**
   * @param os: output stream.
   *        hdr: a nifti header.
   * @return Reference to os.
   * @brief Prints the header in a friendly way.
   * @warning none.
   */
  template< class O >
  O &operator<<( O &os, const NiftiHeader &hdr );

}

namespace Bial {

  template< class O >
  O &operator<<( O &os, NiftiHeader &hdr ) {
    hdr.Print( os );
    return( os );
  }

}

#endif
