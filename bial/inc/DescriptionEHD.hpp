#ifndef EHD_H
#define EHD_H

#include "FeatureExtractor.h"

using namespace std;
using namespace Bial;

/*
 * This convention improves accuracy in integer implementations and saves
 * some work in floating-point ones.
 */
typedef short INT16;
typedef long  INT32;

/*
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
#define CONST_BITS  13
#define PASS1_BITS  2

/*
 * Macros for handling fixed-point arithmetic.
 *
 * All values are expected to be of type INT32.
 */
#define ONE		((INT32) 1)

#define CONST_SCALE	(ONE << CONST_BITS)

#define FIX_1_4142135	((INT32) 11585)		/* FIX(1.4142135) */

/* Convert a positive real constant to an integer scaled by CONST_SCALE */
#define FIX(x)			((INT32) ((x) * CONST_SCALE + 0.5))

/* LEFT_SHIFT provides a proper signed left shift of an INT32 quantity.
 * It is only applied with constant shift counts.
 */
#define LEFT_SHIFT(x,shft)	((x) << (shft))

/* RIGHT_SHIFT provides a proper signed right shift of an INT32 quantity.
 * It is only applied with constant shift counts.
 */
#define RIGHT_SHIFT(x,shft)	((x) >> (shft))

/* Descale and correctly round an INT32 value that's scaled by N bits.
 * We assume RIGHT_SHIFT rounds towards minus infinity, so adding
 * the fudge factor is correct for either sign of X.
 */
#define DESCALE(x,n)		RIGHT_SHIFT((x) + LEFT_SHIFT(ONE, (n)-1), n)

/* Multiply an INT32 variable by an INT32 constant to yield an INT32 result.
 * For 8-bit samples with the recommended scaling, all the variable
 * and constant values involved are no more than 16 bits wide, so a
 * 16x16->32 bit multiply can be used instead of a full 32x32 multiply.
 */
#define MULTIPLY(var,const)  	((var) * (const))


typedef Vector<Features<int>> EHDfeature;

class EHD : public FeatureExtractor<int, int>
{
  private:

    //parametros----------------------------------------------------------------
    size_t GRID;
    size_t BINS;
    size_t BLCK;
    //--------------------------------------------------------------------------

    static int Log( double value , double n );
    static size_t FindMaxPosition(Vector<int> vet);
    Vector<int> ConvolveFilter( int mask[2][2] );

  public:
    EHD(FeatureDetector<int>* Fd);

    EHD(Vector<tuple<Image<int>,Image<int>>> detected);

    void SetParameters( ParameterInterpreter* interpreter );

    string GetParameters( ParameterInterpreter* interpreter );

    EHDfeature Run();
};

#endif
