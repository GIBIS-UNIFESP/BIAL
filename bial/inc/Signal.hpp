/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Sep/11 
 * @brief Signal handeling. 
 */

#ifndef BIALSIGNAL_H
#define BIALSIGNAL_H

#include "Common.hpp"
#include "Vector.hpp"

namespace Bial {
  /**
   * @brief Signal handeling declaration. 
   */
  class Signal {

  private:
    /** @brief  frequency of the bins. */
    Vector< double > frequency; 
    /** @brief  data value of the bins. */
    Vector< double > _data; 
    /** @brief  quick access to data value of the bins. */
    double *qk_data; 

  public:

    /**
     * @date 2012/Sep/11 
     * @param none. 
     * @return none. 
     * @brief Basic constructor. 
     * @warning none. 
     */
    Signal( );

    /**
     * @date 2012/Sep/11 
     * @param data_size: Number of samples. 
     * @param data_minimum: Minimum data value. 
     * @param data_step: Data step between two consecutive samples. 
     * @return none. 
     * @brief Basic constructor. All frequencies are initiated with 0.0.
     * @warning none. 
     */
    Signal( size_t data_size, double data_minimum = 0.0, double data_step = 1.0 );

    /**
     * @date 2012/Sep/11 
     * @param new_frequency: Frequency to be assigned to the signal. 
     * @param data_minimum: Minimum data value. 
     * @param data_step: Data step between two consecutive samples. 
     * @return none. 
     * @brief Basic constructor. 
     * @warning none. 
     */
    template< class D > Signal( Vector< D > &new_frequency, double data_minimum = 0.0, double data_step = 1.0 );

    /**
     * @date 2013/Oct/04 
     * @param new_frequency: Frequency to be assigned to the signal. 
     * @param new_data: Data to be assigned to the signal. 
     * @return none. 
     * @brief Basic constructor. 
     * @warning none. 
     */
    template< class D > Signal( Vector< D > &new_frequency, Vector< D > &new_data );

    /**
     * @date 2015/May/08 
     * @version 1.0.00 
     * @param filename: File name. 
     * @return none. 
     * @brief Copy Constructor. 
     * @warning none. 
     */
    Signal( const Signal &sgn );

    /**
     * @date 2015/May/08 
     * @version 1.0.00 
     * @param sgn: Other signal. 
     * @return none. 
     * @brief Move Constructor. 
     * @warning none. 
     */
    Signal( Signal && ) = default;

    /**
     * @date 2015/May/08 
     * @param other: Signal to be assigned to this. 
     * @return none. 
     * @brief Assigns 'other'signal to this signal. 
     * @warning none. 
     */
    Signal &operator=( const Signal &other );

    /**
     * @date 2015/May/08 
     * @param other: Signal to be assigned to this. 
     * @return none. 
     * @brief Assigns 'other'signal to this signal, moving the content. 
     * @warning none. 
     */
    Signal &operator=( Signal && ) = default;

    /**
     * @date 2012/Sep/13 
     * @param bin: Bin to be acessed. 
     * @return Frequency at bin 'bin'. 
     * @brief Returns frequency at bin 'bin'. 
     * @warning Signal bounds are not verified. 
     */
    const double &operator[]( size_t bin ) const;

    /**
     * @date 2012/Sep/13 
     * @param bin: Bin to be acessed. 
     * @return Reference to bin 'bin'. 
     * @brief Returns a reference to bin 'bin'. 
     * @warning Signal bounds are not verified. Minimum sampling frequency is NOT automatically updated. <br>
     * Signal range does NOT increase automatically either. 
     */
    double &operator[]( size_t p );

    /**
     * @date 2012/Sep/13 
     * @param none. 
     * @return The number of bins. 
     * @brief Returns the number of bins. 
     * @warning none. 
     */
    size_t Size( ) const;

    /**
     * @date 2013/Oct/04 
     * @param none. 
     * @return The number of bins. 
     * @brief Returns the number of bins. 
     * @warning none. 
     */
    size_t size( ) const;

    /**
     * @date 2012/Sep/13 
     * @param none. 
     * @return The number of bins. 
     * @brief Returns the number of bins. Same as Size( ) method.
     * @warning none. 
     */
    size_t Bins( ) const;

    /**
     * @date 2012/Sep/13 
     * @param none. 
     * @return Signal data. 
     * @brief Returns signal data. 
     * @warning none. 
     */
    const double* Data( ) const;

    /**
     * @date 2015/Mar/05 
     * @param none. 
     * @return Signal data. 
     * @brief Returns signal data. 
     * @warning none. 
     */
    double* Data( );

    /**
     * @date 2015/Mar/05 
     * @param none. 
     * @return Signal data. 
     * @brief Returns signal data. 
     * @warning none. 
     */
    const double* data( ) const;

    /**
     * @date 2015/Mar/05 
     * @param none. 
     * @return Signal data. 
     * @brief Returns signal data. 
     * @warning none. 
     */
    double* data( );

    /**
     * @date 2012/Sep/13 
     * @param val: A Vector to be assigned. 
     * @return none. 
     * @brief Assigns a copy of val to data. Frequency vector remains unchanged.
     * @warning Frequency vector size must match. 
     */
    template< class D >
    void Data( Vector< D > val );

    /**
     * @date 2012/Sep/13 
     * @param none. 
     * @return A pointer to the signal data. 
     * @brief Returns a pointer to the signal data. 
     * @warning none. 
     */
    const double* Frequency( ) const;

    /**
     * @date 2015/Mar/15 
     * @param none. 
     * @return A pointer to the signal data. 
     * @brief Returns a pointer to the signal data. 
     * @warning none. 
     */
    double* Frequency( );

    /**
     * @date 2012/Sep/13 
     * @param val: A Vector to be assigned. 
     * @return none. 
     * @brief Assigns a copy of val to frequency. Data vector remains unchanged. 
     * @warning Data vector must match. 
     */
    void Frequency( Vector< double > val );

    /**
     * @date 2012/Sep/13 
     * @param min_bin: left most bin. 
     * @return The minimum frequency contained from min_bin to the last bin (including the extremities). 
     * @brief Returns the minimum frequency contained in bins from min_bin to the last bin (including the
     * extremities). 
     * @warning none. 
     */
    double MinimumFrequency( size_t min_bin = 0 ) const;

    /**
     * @date 2012/Sep/13 
     * @param min_bin: left most bin. 
     * @param max_bin: right most bin. 
     * @return The minimum frequency contained in bins from min_bin to max_bin (including the extremities). 
     * @brief Returns the minimum frequency contained in bins from min_bin to max_bin (including the
     * extremities). 
     * @warning none. 
     */
    double MinimumFrequency( size_t min_bin, size_t max_bin ) const;

    /**
     * @date 2012/Sep/13 
     * @param min_bin: left most bin. 
     * @return The maximum frequency contained in bins from min_bin to the last bin (including the extremities). 
     * @brief Returns the maximum frequency contained in bins from min_bin to the last bin (including the
     * extremities). 
     * @warning none. 
     */
    double MaximumFrequency( size_t min_bin = 0 ) const;

    /**
     * @date 2012/Sep/13 
     * @param min_bin: left most bin. 
     * @param max_bin: right most bin. 
     * @return The maximum frequency contained in bins from min_bin to max_bin (including the extremities). 
     * @brief Returns the maximum frequency contained in bins from min_bin to max_bin (including the
     * extremities). 
     * @warning none. 
     */
    double MaximumFrequency( size_t min_bin, size_t max_bin ) const;

    /**
     * @date 2012/Sep/13 
     * @param min_bin: left most bin. 
     * @return The bin of the minimum frequency contained in bins from min_bin to the last bin (including the
     * extremities). 
     * @brief Returns the bin of the minimum frequency contained in bins from min_bin to the last bin (including
     * the extremities). 
     * @warning none. 
     */
    size_t MinimumFrequencyBin( size_t min_bin = 0 ) const;

    /**
     * @date 2012/Sep/13 
     * @param min_bin: left most bin. 
     * @param max_bin: right most bin. 
     * @return The bin of the minimum frequency contained in bins from min_bin to max_bin (including the
     * extremities). 
     * @brief Returns the bin of the minimum frequency contained in bins from min_bin to max_bin (including the
     * extremities). 
     * @warning none. 
     */
    size_t MinimumFrequencyBin( size_t min_bin, size_t max_bin ) const;

    /**
     * @date 2012/Sep/13 
     * @param min_bin: left most bin. 
     * @return The bin of the maximum frequency contained in bins from min_bin to the last bin (including the
     * extremities). 
     * @brief Returns the bin of the maximum frequency contained in bins from min_bin to the last bin (including
     * the extremities). 
     * @warning none. 
     */
    size_t MaximumFrequencyBin( size_t min_bin = 0 ) const;

    /**
     * @date 2012/Sep/13 
     * @param min_bin: left most bin. 
     * @param max_bin: right most bin. 
     * @return The bin of the maximum frequency contained in bins from min_bin to max_bin (including the
     * extremities). 
     * @brief Returns the bin of the maximum frequency contained in bins from min_bin to max_bin (including the
     * extremities). 
     * @warning none. 
     */
    size_t MaximumFrequencyBin( size_t min_bin, size_t max_bin ) const;

    /**
     * @date 2012/Sep/11 
     * @param val: Data value. 
     * @return The closest bin into which 'val' would fall. 
     * @brief Returns the closest bin into which 'val' would fall. 
     * @warning none. 
     */
    size_t Bin( double val ) const;

    /**
     * @date 2012/Sep/11 
     * @param bin: the bin to be set. 
     * @param val: Data value. 
     * @return none. 
     * @brief Sets value "val" to histogram bin "bin". 
     * @warning none. 
     */
    void Bin( size_t bin, double val );

    /**
     * @date 2012/Sep/11 
     * @param bin: Data bin. 
     * @return Data corresponding to the input bin. 
     * @brief Returns data corresponding to the input bin. 
     * @warning Does not verify data vector bounds. 
     */
    const double &Data( size_t bin ) const;

    /**
     * @date 2012/Sep/11 
     * @param bin: Data bin. 
     * @param val: Data value. 
     * @return none. 
     * @brief Assigns value to data bin. 
     * @warning Does not verify data vector bounds. 
     */
    double &Data( size_t bin );

    /**
     * @date 2012/Sep/14 
     * @param os: an output stream. 
     * @return none. 
     * @brief Prints signal containts to output stream os. It prints a pair composed by frequency and value per
     * line. 
     * @warning none. 
     */
    template< class O >
    void Print( O &os ) const;

  };

  /* @param os: output stream. 
   *             sig: a signal. 
   * @return Reference to os. 
   * @brief Prints the signal in a friendly way. 
   * @warning none. 
   * template< class O > O& operator<<( O& os, const Signal& mat ); 
   */
  template< class O >
  O &operator<<( O &os, const Signal &sig ) {
    sig.Print( os );
    return( os );
  }

}

#include "Signal.cpp"

#endif
