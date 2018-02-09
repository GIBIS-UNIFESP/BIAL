/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2012/Jun/26 
 * @brief Feature vector class. It is essentialy a matrix where, the first dimention refers to the feature and
 * the other dimension to the element.
 * Example: Using constructor Feature( 4, 4 ) the vector will be allocated as follows: f_0_0, f_0_1, f_0_2, f_0_3,
 * f_1_0, f_1_1, ..., where f_i_j is the feature i of sample j. 
 */

#include "Common.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

#ifndef BIALFEATURE_H
#define BIALFEATURE_H

namespace Bial {

  union Color;
  class Sample;

  template< class D >
  class Image;
  template< class D >
  class Feature;
  /**
   * @brief Feature vector class. It is essentialy a matrix where, the first dimention refers to the feature and
   * the other dimension to the element.
   */
  template< class D >
  class Feature {

    template< class D2 >
    friend class Feature;

  private:
    /** @brief  The feature vectors. Access with Feature::operator( sample, feature ). In memory, matrix is featureple.  */
    Matrix< D > feature;
    /** @brief  An index assigned to each feature vector. May be used, for example, to point to a pixel position in an image. */
    Vector< size_t > index; 
    /** @brief  Feature label.*/
    Vector< int > label; 
    /** @brief  Number of different labels. */
    size_t nlabels;

  public:

    /**
     * @date 2012/Jun/26 
     * @param elements: Number of elements. 
     * @param features: Number of features. 
     * @return none. 
     * @brief Basic constructor. 
     * @warning none. 
     */
    Feature( size_t elements, size_t features );

    /**
     * @date 2014/Oct/22 
     * @param feat: Base feature vector for sampling. 
     * @param sample: Vector with sample indexes. 
     * @return none. 
     * @brief Constructor of sub sample of given feature vector with desired samples. 
     * @warning Does not check if requested samples are in original feature vector range. 
     */
    template< class D2 > Feature( const Feature< D2 > &feat, const Sample &sample );

    /**
     * @date 2012/Jul/14 
     * @param elm: The element to be accessed. 
     * @return Reference to feature vector of element elm. 
     * @brief Returns reference to feature vector of element elm.
     * @warning none. 
     */
    Vector< D > operator[]( size_t elm ) const;

    /**
     * @date 2012/Jul/14 
     * @param elm: The element to be accessed. 
     * @return Reference to feature vector of element elm. 
     * @brief Returns reference to feature vector of element elm.
     * @warning none. 
     */
    Vector< D > operator()( size_t elm ) const;

    /**
     * @date 2014/Oct/14 
     * @param elm: The element to be accessed. 
     * @param ftr_vct: A feature vector. 
     * @return none. 
     * @brief Copies ftr_vct data to element elm. 
     * @warning none. 
     */
    void operator()( size_t elm, Vector< D > ftr_vct );

    /**
     * @date 2012/Jun/29 
     * @param elm: The element to be accessed. 
     * @param ftr: The feature to be accessed. 
     * @return Feature ftr of element elm. 
     * @brief Returns feature ftr of element elm.
     * @warning none. 
     */
    const D &operator()( size_t elm, size_t ftr ) const;

    /**
     * @date 2012/Jun/29 
     * @param elm: The element to be accessed. 
     * @param ftr: The feature to be accessed. 
     * @return Reference to feature ftr of element elm. 
     * @brief Returns reference to feature ftr of element elm.
     * @warning none. 
     */
    D &operator()( size_t elm, size_t ftr );

    /**
     * @date 2012/Jun/29 
     * @param none. 
     * @return The number of elements. 
     * @brief Returns the number of elements.
     * @warning none. 
     */
    size_t Elements( ) const;

    /**
     * @date 2015/Mar/12 
     * @param none. 
     * @return The number of vectors multiplied by the number of features. 
     * @brief Returns the number of vectors multiplied by the number of features. 
     * @warning none. 
     */
    size_t size( ) const;

    /**
     * @date 2015/Mar/12 
     * @param none. 
     * @return The number of vectors multiplied by the number of features. 
     * @brief Returns the number of vectors multiplied by the number of features. 
     * @warning none. 
     */
    size_t Size( ) const;

    /**
     * @date 2012/Jun/29 
     * @param none. 
     * @return A constant reference to the index vector. 
     * @brief Returns a constant reference to the index vector. 
     * @warning none. 
     */
    const Vector< size_t > &Index( ) const;

    /**
     * @date 2012/Aug/13 
     * @param none. 
     * @return A reference to the index vector. 
     * @brief Returns a reference to the index vector. 
     * @warning Input vector size must be compatible. 
     */
    Vector< size_t > &Index( );

    /**
     * @date 2012/Jun/29 
     * @param elm: The index to be accessed. 
     * @return Feature index element elm. 
     * @brief Returns index of element elm.
     * @warning none. 
     */
    const size_t &Index( size_t elm ) const;

    /**
     * @date 2012/Jun/29 
     * @param elm: The index to be accessed. 
     * @return Reference to feature index element elm. 
     * @brief Returns a reference to feature index element elm.
     * @warning none. 
     */
    size_t &Index( size_t elm );

    /**
     * @date 2012/Jun/29 
     * @param none. 
     * @return A constant reference to the label vector. 
     * @brief Returns a constant reference to the label vector. 
     * @warning none. 
     */
    const Vector< int > &Label( ) const;

    /**
     * @date 2012/Aug/13 
     * @param none. 
     * @return A reference to the label vector. 
     * @brief Returns a reference to the label vector. 
     * @warning Input vector size must be compatible. 
     */
    Vector< int > &Label( );

    /**
     * @date 2012/Jun/29 
     * @param elm: The label to be accessed. 
     * @return Feature label element elm. 
     * @brief Returns label of element elm.
     * @warning none. 
     */
    const int &Label( size_t elm ) const;

    /**
     * @date 2012/Jun/29 
     * @param elm: The label to be accessed. 
     * @return Reference to feature label element elm. 
     * @brief Returns a reference to feature label element elm.
     * @warning none. 
     */
    int &Label( size_t elm );

    /**
     * @date 2012/Jun/29 
     * @param none. 
     * @return Number of labels in the feature vector. 
     * @brief Returns the number of labels in the feature vector.
     * @warning none. 
     */
    size_t Labels( ) const;

    /**
     * @date 2012/Jun/29 
     * @param max: Maximum label value in feature vector. 
     * @return none. 
     * @brief Sets the maximum label value in feature vector.
     * @warning none. 
     */
    void Labels( size_t max );

    /**
     * @date 2012/Jun/29 
     * @param none. 
     * @return The number of features. 
     * @brief Returns the number of features. 
     * @warning none. 
     */
    size_t Features( ) const;

    /**
     * @date 2012/Aug/13 
     * @param none. 
     * @return A reference from this feature vectors. 
     * @brief Returns a reference from this feature vectors. 
     * @warning none. 
     */
    const Matrix< D > &FeatureVector( ) const;

    /**
     * @date 2015/Mar/10 
     * @param none. 
     * @return A reference from this feature vectors. 
     * @brief Returns a reference from this feature vectors. 
     * @warning none. 
     */
    Matrix< D > &FeatureVector( );

    /**
     * @date 2012/Aug/13 
     * @param none. 
     * @return A reference from this feature vectors. 
     * @brief Returns a reference from this feature vectors. 
     * @warning none. 
     */
    const D* data( ) const;

    /**
     * @date 2015/Mar/10 
     * @param none. 
     * @return A reference from this feature vectors. 
     * @brief Returns a reference from this feature vectors. 
     * @warning none. 
     */
    D* data( );

    /**
     * @date 2012/Aug/07 
     * @param os: an output stream. 
     * @return The output stream. 
     * @brief Prints feature vector containts to output stream os. 
     * @warning Only enebled features are printed. 
     */
    template< class O >
    O &Print( O &os ) const;

  };

  template< class D, class O >
  O &operator<<( O &os, const Feature< D > &ftr ) {
    ftr.Print( os );
    return( os );
  }

}

#include "Feature.cpp"

#endif
