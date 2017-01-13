/* Biomedical Image Analysis Library
 * See README file in the root instalation directory for more information.
 */

/**
 * @date 2012/Sep/19
 * @brief Path function to compute the initial path-values and propagated path-values in IFT algorithm.
 */

#ifndef BIALPATHFUNCTION_H
#define BIALPATHFUNCTION_H

#include "Common.hpp"

namespace Bial {

  class IFT;

  enum class BucketState : char;

  /**
   * @brief Path function to compute the initial path-values and propagated path-values.
   */
  template< template< class D > class C, class D >
  class PathFunction {

    friend class IFT;

    /** @brief  Remove functions of IFT. They will operate on valid maps among value, label, and predecessor. */
    typedef bool ( PathFunction< C, D >::*RemoveFn )( size_t index, BucketState state );

    /** @brief  Update functions of IFT. They will operate on valid maps among value, label, and predecessor. */
    typedef void ( PathFunction< C, D >::*UpdateFn )( size_t index, size_t adj_index );

  protected:

    /** @brief  Pointer to value container (Vector, Matrix, Image, etc). */
    C< D > *value;
    /** @brief  Pointer to label container (Vector, Matrix, Image, etc). */
    C< int > *label;
    /** @brief  Pointer to predecessor container (Vector, Matrix, Image, etc).  */
    C< int > *predecessor;
    int next_label;

  public:

    /** Removal and update functions. */
    typename PathFunction< C, D >::RemoveFn RemoveData;
    typename PathFunction< C, D >::UpdateFn UpdateData;

    /**
     * @date 2015/Mar/09
     * @param init_value: Reference for initial value container.
     * @param init_label: Reference for initial label container.
     * @param init_predecessor: Reference for predecessor container.
     * @param sequential_label: Sets labeling sequentially.
     * @return none.
     * @brief Basic constructor.
     * @warning none.
     */
    PathFunction( C< D > &init_value, C< int > *init_label, C< int > *init_predecessor, bool sequential_label );
    
    /**
     * @date 2015/Mar/09
     * @param pf: A queue.
     * @return none.
     * @brief Copy constructor.
     * @warning none.
     */
    PathFunction( const PathFunction< C, D > &pf );

    /*
     * / * @date 2015/Mar/09 * /
     * / * @param pf: A queue. * /
     * / * @return This. * /
     * / * @brief Assignement operator. * /
     * / * @warning none. * /
     * virtual PathFunction< C, D > operator=( const PathFunction< C, D > &pf ) = 0;
     */

    /**
     * @date 2013/Nov/19
     * @param none.
     * @return none.
     * @brief Virtual destructor required for virtual classes.
     * @warning none.
     */
    virtual ~PathFunction( ) {
      COMMENT( "Destructor.", 2 );
    }

    /**
     * @date 2013/Jun/28
     * @param index: Data index.
     * @param adj_index: Adjacent index;
     * @return none.
     * @brief Updates value, label, and predecessor images of index adj_index according to 'index' and
     * value. One of these functions will be used depending on the chosen resultant images.
     * @warning none.
     */
    void UpdateCompleteData( size_t index, size_t adj_index );
    void UpdatePredecessorData( size_t index, size_t adj_index );
    void UpdateLabelData( size_t index, size_t adj_index );
    void UpdateSimpleData( size_t index, size_t adj_index );

    /**
     * @date 2012/Sep/25
     * @param index: The index of the pixel to be initalized.
     * @return Whether this node can propagate or not.
     * @brief Sets initial value for root pixel of index 'index'.
     * @warning none.
     */
    virtual bool RemoveSimple( size_t index, BucketState state ) = 0;

    /**
     * @date 2014/Dec/05
     * @param index: The index of the pixel to be initalized.
     * @return Whether this node can propagate or not.
     * @brief Sets initial value for root pixel of index 'index'. Also sets its predecessor value.
     * @warning none.
     */
    virtual bool RemovePredecessor( size_t index, BucketState state ) = 0;

    /**
     * @date 2014/Dec/05
     * @param index: The index of the pixel to be initalized.
     * @return Whether this node can propagate or not.
     * @brief Sets initial value for root pixel of index 'index'. Also sets its label value.
     * @warning none.
     */
    virtual bool RemoveLabel( size_t index, BucketState state ) = 0;

    /**
     * @date 2012/Sep/25
     * @param index: The index of the pixel to be initalized.
     * @return Whether this node can propagate or not.
     * @brief Sets initial value for root pixel of index 'index'. Also sets its predecessor and label value.
     * @warning none.
     */
    virtual bool RemoveComplete( size_t index, BucketState state ) = 0;

    /**
     * @date 2013/Oct/14
     * @param index: Source pixel index.
     * @param adj_index: Adjacent pixel index.
     * @return True if it is possible to propagate.
     * @brief Checks if index can offer better value to adj_index, prior to computing it by Propagate
     * function. This function makes IFT faster.
     * @warning none.
     */
    virtual bool Capable( int index, int adj_index, BucketState adj_state ) = 0;

    /**
     * @date 2013/Jun/28
     * @param index: Source pixel index.
     * @param adj_index: Adjacent pixel index.
     * @return True if path-value is propagated.
     * @brief Updates adjacent pixel values and returns true if path_function is propagated.
     * @warning none.
     */
    virtual bool Propagate( int index, int adj_index ) = 0;

    /**
     * @date 2012/Sep/19
     * @param none.
     * @return True for increasing and false for decreasing values.
     * @brief Returns whether this path function uses increasing or decreasing values.
     * @warning none.
     */
    virtual bool Increasing( ) = 0;

    /**
     * @date 2016/Nov/17
     * @param index: Index of an element.
     * @return The best value that this element can achieve.
     * @brief Function used for IFT with backtracking. It usually returns the handicap of pathfunctions that contain
     *        this kind of map.
     * @warning none.
     */
    virtual D BestValue( int index );

    /**
     * @date 2013/Jun/28
     * @param none.
     * @return A pointer to one of root initialization functions.
     * @brief Returns a root initialization function that will be used depending on used maps.
     * @warning none.
     */
    typename PathFunction< C, D >::RemoveFn RemoveFunction( );

    /**
     * @date 2013/Jun/28
     * @param none.
     * @return A pointer to one of update functions.
     * @brief Returns an update function that will be used depending on used maps.
     * @warning none.
     */
    typename PathFunction< C, D >::UpdateFn UpdateFunction( );
  };

}

#include "PathFunction.cpp"

#endif
