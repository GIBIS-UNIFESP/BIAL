/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @file DicomHeader.hpp
 * @author Fábio Augusto Menocci Cappabianco 
 * @date 2012/Jun/21 
 * @version 1.0.00
 * @brief
 * Content: Dicom header reader 
 * <br> Description: Reads Dicom file and header containts. Based on Fabio Kawaoka Takase implementation. 
 * <br> Future add-on's: - Translate all comments to English. Reformat code to Bial standard. 
 *                  - Implement data writting. 
 *                  - Implement Nifti Header convertion. 
 */

/*****************************************************************************
*  File Name        :  TDicom.h
*  1st Version date :  Aug 2007
*  Author           :  Fabio Kawaoka Takase
*  ----------------------------------------------------------------------------
*  Description :
*  ^^^^^^^^^^^
*  TDicom class definition. This class extends the TDynMatrix class, providing
*  the support to open DICOM files.
*  ----------------------------------------------------------------------------
*  Updates:
*  ^^^^^^^^
*  2008-03-13:    Adicionadas classes TElementId e TGroupId
*  ----------------------------------------------------------------------------
*****************************************************************************/


#include "File.hpp"
#include "Image.hpp"

#ifndef BIALDICOMHEADER_H
#define BIALDICOMHEADER_H

namespace Bial {

  /** @brief  DICOM DATA ELEMENT */
  class TElementId {

  private:
    /** @brief  Element ID */
    int id;
    /** @brief  --> tipo == 1 ( std::string ), == 2 float, == 3 int */
    int valtype;
    /** @brief  string value */
    std::string sval;
    /** @brief  float value */
    float fval;
    /** @brief  int value */
    int ival;

  public:
    /** @brief  constructor */
    TElementId( ) try : id( -1 ), sval( std::string( ) ), fval( -1.0f ), ival( -1 ) {
    }
    catch( const std::out_of_range &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Out of range exception." ) );
      throw( std::out_of_range( msg ) );
    }
    catch( const std::logic_error &e ) {
      std::string msg( e.what( ) + std::string( "\n" ) + BIAL_ERROR( "Logic Error." ) );
      throw( std::logic_error( msg ) );
    }

    /** @brief  altera o valor ( std::string ) e o ID do Element */
    void set( int newId, std::string data ) {
      id = newId;
      sval = data;
      valtype = 1;
    }

    /** @brief  altera o valor ( float ) e o ID do Element */
    void set( int newId, float data ) {
      id = newId;
      fval = data;
      valtype = 2;
    }

    /** @brief  altera o valor ( int ) e o ID do Element */
    void set( int newId, int data ) {
      id = newId;
      ival = data;
      valtype = 3;
    }

    int getId( ) const {
      return( id );
    }

    int getValueType( ) const {
      return( valtype );
    }

    int getIntValue( ) const {
      return( ival );
    }

    float getFloatValue( ) const {
      return( fval );
    }

    std::string getStringValue( ) const {
      return( sval );
    }

    /* escreve o valor do Element na tela */
    void print( ) {
      if( valtype == 1 ) {
        std::cout << "Elem ID: " << id << " ; Elem Data: " << sval << std::endl;
      }
      else if( valtype == 2 ) {
        std::cout << "Elem ID: " << id << " ; Elem Data: " << fval << std::endl;
      }
      else if( valtype == 3 ) {
        std::cout << "Elem ID: " << id << " ; Elem Data: " << ival << std::endl;
      }
    }

  };

  /** @brief  DICOM DATA BLOCK */
  class TGroupId {

  private:
    /** @brief  group ID */
    int id; 
    std::deque< TElementId > lista; /* deque of Elements */

  public:
    typedef std::deque< TElementId >::iterator iterator;

    /** @brief  constructor ( ) */
    TGroupId( ) : id( -1 ), lista( std::deque< TElementId >( ) ) {
    }

    /** @brief  constructor ( int ID ) */
    TGroupId( int new_id ) : id( new_id ), lista( std::deque< TElementId >( ) ) {
    }

    /** @brief  muda o valor de ID do Group */
    void setId( int id ) {
      this->id = id;
    }

    /** @brief  adiciona um Element a list */
    bool push( int id, TElementId element ) {
      if( this->id == id ) {
        lista.push_back( element );
        return( true );
      }
      return( false );
    }

    /** @brief  posiciona no primeiro Element em list */
    iterator begin( ) {
      return( lista.begin( ) );
    }

    /** @brief  posiciona no ultimo Element em list */
    iterator end( ) {
      return( lista.end( ) );
    }

    /** @brief  retornar o tamanho de list */
    size_t size( ) const {
      return( lista.size( ) );
    }

    /** @brief  retorna o ID do Group */
    int getId( ) const {
      return( id );
    }

  };

  class DicomHeader {

  private:
    /** @brief  Block list */
    std::deque< TGroupId > lista; 

    /**
     * @date 2013/Aug/23 
     * @param none. 
     * @return Data according to element type. 
     * @brief Returns data read from element. 
     * @warning none. 
     */
    std::string readID( );
    long readInt32( );
    int readInt16( );

  public:
    typedef std::deque< TGroupId >::iterator iterator;

    /** @brief  posiciona no primeiro Element em list */
    iterator begin( ) {
      return( lista.begin( ) );
    }

    /** @brief  posiciona no ultimo Element em list */
    iterator end( ) {
      return( lista.end( ) );
    }

    IFile file_pointer;
    DicomHeader( );

    template< class D >
    Image< D > readFile( std::string filename );

    void readZeroPadding( );

    void readDICM( );

    template< class D >
    Image< D > readData( );

    template< class D >
    Image< D > readImageData( );

    void push( TGroupId group );

    TElementId getElement( int grupo, int elemento );
  };

}

#include "DicomHeader.cpp"

#endif
