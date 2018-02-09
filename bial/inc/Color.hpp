/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date: 2015/Oct/06
 * @brief Colorspace convertion methods. For integer colors.
 */

#include "Array.hpp"

#ifndef BIALCOLOR_H
#define BIALCOLOR_H

namespace Bial {

  class RealColor;

  union Color {
    /* members */
    Array< uchar, 4 > channel;
    uint value;
    
  public:

    /**
     * @date 2015/Oct/06
     * @param none.
     * @return none.
     * @brief Constructor. All zero values.
     * @warning none.
     */
    explicit Color( );

    /**
     * @date 2015/Oct/06
     * @param val: The color value.
     * @return none.
     * @brief Constructor.
     * @warning none.
     */
    explicit Color( uint val );

    /**
     * @date 2015/Oct/06
     * @param chl: The color channels.
     * @return none.
     * @brief Constructor.
     * @warning none.
     */
    Color( uchar alpha, uchar red, uchar green, uchar blue );

    /**
     * @date 2015/Oct/06
     * @version 1.0.00
     * @param other: Other Color.
     * @return none.
     * @brief Copy Constructor.
     * @warning none.
     */
    Color( const Color &other );

    /**
     * @date 2015/Oct/06
     * @version 1.0.00
     * @param unamed: Other Color.
     * @return none.
     * @brief Move Constructor.
     * @warning none.
     */
    Color( Color &&other );


    /**
     * @date 2015/Oct/06
     * @version 1.0.00
     * @param unamed: Other Color.
     * @return none.
     * @brief Destructor.
     * @warning none.
     */
    ~Color( ) {
    }

    /**
     * @date 2015/Oct/27
     * @version 1.0.00
     * @param other: Other RealColor.
     * @return none.
     * @brief Copy Constructor.
     * @warning none.
     */
    Color( const RealColor &other );

    /**
     * @date 2015/Oct/06
     * @param unamed: Color to be assigned to this.
     * @return none.
     * @brief Copies color to this color.
     * @warning none.
     */
    Color &operator=( const Color &other );

    /**
     * @date 2015/Oct/06
     * @param unamed: Color to be assigned to this.
     * @return none.
     * @brief moves color to this color.
     * @warning none.
     */
    Color &operator=( Color &&other );

    /**
     * @date 2015/Oct/06
     * @param chl: a channel to be acessed.
     * @return Constant reference of data pointed by channel chl.
     * @brief Returns a constant reference of data pointed by channel chl. Same as operator()( chl ).
     * @warning Channel bounds are not verified.
     */
    const uchar &operator[]( size_t chl ) const;

    /**
     * @date 2015/Oct/06
     * @param chl: a channel to be acessed.
     * @return Reference of data pointed by channel chl.
     * @brief Returns the reference for data pointed by channle chl. Same as operator()( chl ).
     * @warning Channel bounds are not verified.
     */
    uchar &operator[]( size_t chl );

    /**
     * @date 2015/Oct/06
     * @param chl: a channel to be acessed.
     * @return Constant reference of data pointed by channel chl.
     * @brief Returns a constant reference of data pointed by channel chl. Same as operator[]( chl ).
     * @warning Channel bounds are not verified.
     */
    const uchar &operator()( size_t chl ) const;

    /**
     * @date 2015/Oct/06
     * @param chl: a channel to be acessed.
     * @return Reference of data pointed by channel chl.
     * @brief Returns the reference for data pointed by channle chl. Same as operator[]( chl ).
     * @warning Channel bounds are not verified.
     */
    uchar &operator()( size_t chl );

    /**
     * @date 2015/Oct/06
     * @param val: Color new value.
     * @return none.
     * @brief Updates color value with val.
     * @warning none.
     */
    void Value( uint val );

    /**
     * @date 2015/Oct/06
     * @param none.
     * @return Current color value.
     * @brief Returns current color value.
     * @warning none.
     */
    uint Value( ) const;

    /**
     * @date 2015/Oct/21
     * @param os: an output stream.
     * @return none.
     * @brief Prints color containts to output stream os.
     * @warning none.
     */
    template< class O >
    void Print( O &os ) const;

  };

  /**
   * @date 2015/Oct/06
   * @param clr1, clr2: Input colors.
   * @return Sum the input colors element by element.
   * @brief Computes the sum of the colors and returns the result.
   * @warning none.
   */
  Color operator+( const Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param clr1, clr2: Input colors.
   * @return Reference to clr1.
   * @brief Computes the sum of the colors, assigns the result to clr1, and returns the result.
   * @warning none.
   */
  Color &operator+=( Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param clr1, clr2: Input colors.
   * @return Difference of the input colors element by element.
   * @brief Computes the difference of the colors and returns the result.
   * @warning none.
   */
  Color operator-( const Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param other: Other color.
   * @return Reference to clr1.
   * @brief Computes the difference between input colors, assigns the result to clr1, and returns the result.
   * @warning none.
   */
  Color &operator-=( Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param clr1, clr2: Input colors.
   * @return Product of the input colors element by element.
   * @brief Computes the product of the colors and returns the result.
   * @warning none.
   */
  Color operator*( const Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param other: Other color.
   * @return Reference to clr1.
   * @brief Computes the product between input colors, assigns the result to clr1, and returns the result.
   * @warning none.
   */
  Color &operator*=( Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param clr1, clr2: Input colors.
   * @return Quotient of the input colors element by element.
   * @brief Computes the quotient of the colors and returns the result.
   * @warning none.
   */
  Color operator/( const Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param other: Other color.
   * @return Reference to clr1.
   * @brief Computes the quotient between input colors, assigns the result to clr1, and returns the result.
   * @warning none.
   */
  Color &operator/=( Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param other: Other color.
   * @return true if clr1 is equal to clr2.
   * @brief Compares two colors and returns true if they are equal.
   * @warning none.
   */
  bool operator==( const Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/06
   * @param other: Other color.
   * @return true if clr1 is different of clr2.
   * @brief Compares two colors and returns true if they are different.
   * @warning none.
   */
  bool operator!=( const Color &clr1, const Color &clr2 );

  /**
   * @date 2015/Oct/21
   * @param scalar: a scalar.
   * @param clr: Input color.
   * @return Sums scalar to input color channel values.
   * @brief Computes the sum of the scalar to input color channel values and returns the result.
   * @warning none.
   */
  Color operator+( const Color &clr, double scalar );
  Color operator+( double scalar, const Color &clr );

  /**
   * @date 2015/Oct/21
   * @param scalar: a scalar.
   * @param clr: Input color.
   * @return Reference to clr.
   * @brief Computes the sum of the scalar to input color channel values and assigns the result to clr.
   * @warning none.
   */
  Color &operator+=( Color &clr, double scalar );

  /**
   * @date 2015/Oct/21
   * @param scalar: a scalar.
   * @param clr: Input color.
   * @return Subtracts scalar from input color channel values, or vice-versa.
   * @brief Computes the differente between scalar and input color channel values and returns the result.
   * @warning none.
   */
  Color operator-( const Color &clr, double scalar );
  Color operator-( double scalar, const Color &clr );

  /**
   * @date 2015/Oct/21
   * @param scalar: a scalar.
   * @param clr: Input color.
   * @return Reference to clr.
   * @brief Computes the difference between the scalar and input color channel values and assigns the result to clr.
   * @warning none.
   */
  Color &operator-=( Color &clr, double scalar );

  /**
   * @date 2015/Oct/21
   * @param scalar: a scalar.
   * @param clr: Input color.
   * @return Multiplies the scalar to input color channel values.
   * @brief Computes the product of the scalar and input color channel values and returns the result.
   * @warning none.
   */
  Color operator*( const Color &clr, double scalar );
  Color operator*( double scalar, const Color &clr );

  /**
   * @date 2015/Oct/21
   * @param scalar: a scalar.
   * @param clr: Input color.
   * @return Reference to clr.
   * @brief Computes the product of the scalar and input color channel values and assigns the result to clr.
   * @warning none.
   */
  Color &operator*=( Color &clr, double scalar );

  /**
   * @date 2015/Oct/21
   * @param scalar: a scalar.
   * @param clr: Input color.
   * @return Divides the scalar by the input color channel values, or vice-versa.
   * @brief Computes the quotient between the scalar and the input color channel values and returns the result.
   * @warning none.
   */
  Color operator/( const Color &clr, double scalar );
  Color operator/( double scalar, const Color &clr );

  /**
   * @date 2015/Oct/21
   * @param scalar: a scalar.
   * @param clr: Input color.
   * @return Reference to clr.
   * @brief Computes the quotient between the scalar and the input color channel values and assigns the result to clr.
   * @warning none.
   */
  Color &operator/=( Color &clr, double scalar );

  /**
   * @date 2015/Oct/21
   * @param os: output stream.
   *        clr: A color.
   * @return Reference to os.
   * @brief Prints the color in a friendly way.
   * @warning none.
   */
  template< class O >
  O &operator<<( O &os, const Color &clr );

}

namespace Bial {

  template< class O >
  void Color::Print( O &os ) const {
    os << static_cast< int >( channel( 0 ) ) << ", " << static_cast< int >( channel( 1 ) ) << ", "
       << static_cast< int >( channel( 2 ) ) << ", " << static_cast< int >( channel( 3 ) );
  }

  template< class O >
  O &operator<<( O &os, const Color &clr ) {
    clr.Print( os );
    return( os );
  }

}

#include "Color.cpp"

#endif
