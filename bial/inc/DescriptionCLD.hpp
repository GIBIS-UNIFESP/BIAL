#include "DescriptionFeatureExtractor.hpp"

#ifndef CLD_H
#define CLD_H

namespace Bial {

  typedef Vector < Features < int >> CLDfeature;


/*
 * This file contains a slow-but-accurate integer implementation of the
 * forward DCT (Discrete Cosine Transform).
 *
 * A 2-D DCT can be done by 1-D DCT on each row followed by 1-D DCT
 * on each column.  Direct algorithms are also available, but they are
 * much more complex and seem not to be any faster when reduced to code.
 *
 * This implementation is based on an algorithm described in
 *   C. Loeffler, A. Ligtenberg and G. Moschytz, "Practical Fast 1-D DCT
 *   Algorithms with 11 Multiplications", Proc. Int'l. Conf. on Acoustics,
 *   Speech, and Signal Processing 1989 (ICASSP '89), pp. 988-991.
 * The primary algorithm described there uses 11 multiplies and 29 adds.
 * We use their alternate method with 12 multiplies and 32 adds.
 * The advantage of this method is that no data path contains more than one
 * multiplication; this allows a very simple and accurate implementation in
 * scaled fixed-point arithmetic, with a minimal number of shifts.
 */

/*
 * A forward DCT routine is given a pointer to a work area of type DCTELEM[];
 * the DCT is to be performed in-place in that buffer.  Type DCTELEM is int
 * for 8-bit samples. The DCT inputs are expected to be signed. The DCT
 * outputs are returned scaled up by a factor of 8; they therefore have a
 * range of +-8K for 8-bit data.  This convention improves accuracy in
 * integer implementations and saves some work in floating-point ones.
 */
  typedef int DCTELEM; /* 16 or 32 bits is fine */
  typedef long INT32;

/* Various constants determining the sizes of things.
 */
#define DCTSIZE 8 /* The basic DCT block is 8x8 samples */
#define DCTSIZE2 64 /* DCTSIZE squared; # of elements in a block */

/*
 * Each 1-D DCT step produces outputs which are a factor of sqrt(N)
 * larger than the true DCT outputs.  The final outputs are therefore
 * a factor of N larger than desired; since N=8 this can be cured by
 * a simple right shift at the end of the algorithm.  The advantage of
 * this arrangement is that we save two multiplications per 1-D DCT,
 * because the y0 and y4 outputs need not be divided by sqrt(N).
 * This factor of 8 is removed by the quantization step.
 *
 * We have to do addition and subtraction of the integer inputs, which
 * is no problem, and multiplication by fractional constants, which is
 * a problem to do in integer arithmetic.  We multiply all the constants
 * by CONST_SCALE and convert them to integer constants (thus retaining
 * CONST_BITS bits of precision in the constants).  After doing a
 * multiplication we have to divide the product by CONST_SCALE, with proper
 * rounding, to produce the correct output.  This division can be done
 * cheaply as a right shift of CONST_BITS bits.  We postpone shifting
 * as long as possible so that partial sums can be added together with
 * full fractional precision.
 *
 * The outputs of the first pass are scaled up by PASS1_BITS bits so that
 * they are represented to better-than-integral precision.  These outputs
 * require 8 + PASS1_BITS + 3 bits; this fits in a 16-bit word with the
 * recommended scaling.
 *
 * To avoid overflow of the 32-bit intermediate results in pass 2, we must
 * have 8 + CONST_BITS + PASS1_BITS <= 26.  Error analysis shows that the
 * values given below are the most effective.
 */
#define CONST_BITS 13
#define PASS1_BITS 2

/*
 * Macros for handling fixed-point arithmetic.
 *
 * All values are expected to be of type INT32.
 */
#define ONE ( ( INT32 ) 1 )

#define FIX_0_298631336 ( ( INT32 ) 2446 ) /* FIX(0.298631336) */
#define FIX_0_390180644 ( ( INT32 ) 3196 ) /* FIX(0.390180644) */
#define FIX_0_541196100 ( ( INT32 ) 4433 ) /* FIX(0.541196100) */
#define FIX_0_765366865 ( ( INT32 ) 6270 ) /* FIX(0.765366865) */
#define FIX_0_899976223 ( ( INT32 ) 7373 ) /* FIX(0.899976223) */
#define FIX_1_175875602 ( ( INT32 ) 9633 ) /* FIX(1.175875602) */
#define FIX_1_501321110 ( ( INT32 ) 12299 ) /* FIX(1.501321110) */
#define FIX_1_847759065 ( ( INT32 ) 15137 ) /* FIX(1.847759065) */
#define FIX_1_961570560 ( ( INT32 ) 16069 ) /* FIX(1.961570560) */
#define FIX_2_053119869 ( ( INT32 ) 16819 ) /* FIX(2.053119869) */
#define FIX_2_562915447 ( ( INT32 ) 20995 ) /* FIX(2.562915447) */
#define FIX_3_072711026 ( ( INT32 ) 25172 ) /* FIX(3.072711026) */

/* LEFT_SHIFT provides a proper signed left shift of an INT32 quantity.
 * It is only applied with constant shift counts.
 */
#define LEFT_SHIFT( x, shft ) ( ( x ) << ( shft ) )

/* RIGHT_SHIFT provides a proper signed right shift of an INT32 quantity.
 * It is only applied with constant shift counts.
 */
#define RIGHT_SHIFT( x, shft ) ( ( x ) >> ( shft ) )

/* Descale and correctly round an INT32 value that's scaled by N bits.
 * We assume RIGHT_SHIFT rounds towards minus infinity, so adding
 * the fudge factor is correct for either sign of X.
 */
#define DESCALE( x, n ) RIGHT_SHIFT( ( x ) + LEFT_SHIFT( ONE, ( n ) - 1 ), n )

/* Multiply an INT32 variable by an INT32 constant to yield an INT32 result.
 * For 8-bit samples with the recommended scaling, all the variable
 * and constant values involved are no more than 16 bits wide, so a
 * 16x16->32 bit multiply can be used instead of a full 32x32 multiply.
 */
#define MULTIPLY( var, const ) ( ( var ) * ( const ) )


  class CLD : public FeatureExtractor< Color, int > {
private:

    static const unsigned short int quantbits[ DCTSIZE2 ];
    static const unsigned short int zigzag[ DCTSIZE2 ];

    static uchar LinearNormalize( double value, double n );
    Vector< int > DiscreteCosineTransform( Vector< int > data );
    Vector< int > QuantizeCoefficients( Vector< int > data );

public:
    CLD( FeatureDetector< Color > *Fd );

    CLD( Vector < std::tuple < Image< Color >, Image< int >> > detected );


    void SetParameters( ParameterInterpreter *interpreter );

    std::string GetParameters( ParameterInterpreter *interpreter );

    CLDfeature Run( );
  };

}
#endif
