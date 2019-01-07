/* Biomedical Image Analysis Library 
 * See README file in the root instalation directory for more information. 
 */

/**
 * @date 2013/Aug/08 
 * @brief File and Directory Handling. 
 */

#include "Common.hpp"
#include "Vector.hpp"

#ifndef BIALFILE_H
#define BIALFILE_H

namespace Bial {

  class NiftiHeader;
  template< class D >
  class Image;
  template< class D >
  class Matrix;

  class Terminal {

  public:

    /**
     * @date 2013/Oct/30 
     * @param none. 
     * @return true if terminal is accessable. 
     * @brief Verifies if terminal is accessable. 
     * @warning none. 
     */
    static bool Exists( );

  };

  class Directory {

  public:

    /**
     * @date 2013/Oct/30 
     * @param dir_name: directory name. 
     * @return true if directory exists. 
     * @brief Verifies if directory exists. 
     * @warning none. 
     */
    static bool Exists( const std::string &dir_name );

    /**
     * @date 2013/Oct/30 
     * @param dir_name: directory name. 
     * @param recursive: true for recursive search. 
     * @return Vector of strings containing subdirectories in given
     * directory. 
     * @brief Returns a vector of strings containing the
     * subdirectories in given directory. 
     * @warning none. 
     */
    static Vector< std::string > ListDirectories( const std::string &dir_name, bool recursive = false );

    /**
     * @date 2013/Oct/30 
     * @param dir_name: directory name. 
     * @param recursive: true for recursive search in subdirectories. 
     * @return Vector of strings containing files in given directory. 
     * @brief Returns a vector of strings containing files in given directory. 
     * @warning none. 
     */
    static Vector< std::string > ListFiles( const std::string &dir_name, bool recursive = false );

    /**
     * @date 2013/Oct/30 
     * @param dir_name: directory name. 
     * @param extension: a file extension. Leave it blank for all files. 
     * @param recursive: true for recursive search in subdirectories. 
     * @return Vector of strings containing files in given directory. 
     * @brief Returns a vector of strings containing files in given directory. 
     * @warning none. 
     */
    static Vector< std::string > ListFiles( const std::string &dir_name,
                                            const std::string &extension,
                                            bool recursive = false );

    /**
     * @date 2013/Oct/29 
     * @param dir_name: directory name. 
     * @param list_subdirs: true for including sub directories to the returning string. 
     * @param recursive: true for recursive search. 
     * @return Vector of strings containing files in given directory. 
     * @brief Returns a vector of strings containing files in
     * given directory. 
     * @warning none. 
     */
    static Vector< std::string > List( const std::string &dir_name, bool recursive = false );

  };

  class IFile {

    friend IFile &getline( IFile &file, std::string &str, char delim );
    friend IFile &getline( IFile && file, std::string & str, char delim );
    friend IFile &getline( IFile &file, std::string &str );
    friend IFile &getline( IFile && file, std::string & str );
    friend IFile &operator>>( IFile &file, char &c );
    friend IFile &operator>>( IFile &file, signed char &c );
    friend IFile &operator>>( IFile &file, unsigned char &c );
    friend IFile &operator>>( IFile &file, char *s );
    friend IFile &operator>>( IFile &file, signed char *s );
    friend IFile &operator>>( IFile &file, unsigned char *s );

  private:

    std::ifstream std_file;

  public:

    /**
     * @date 2013/Aug/19 
     * @param many. 
     * @return Depends. 
     * @brief Interface to call std::ifstream
     * functions. 
     * @warning none. 
     */
    IFile( );
    explicit IFile( std::ifstream && if_stream );
    explicit IFile( const char *filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary );
    explicit IFile( const std::string &filename,
                    std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary );
    IFile( const IFile &other ) = delete;
    ~IFile( );
    void open( const char *filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary );
    void open( const std::string &filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::binary );
    bool is_open( ) const;
    bool good( ) const;
    bool eof( ) const;
    bool fail( ) const;
    bool bad( ) const;
    void close( );
    int peek( );
    IFile &operator>>( bool &val );
    IFile &operator>>( short &val );
    IFile &operator>>( unsigned short &val );
    IFile &operator>>( int &val );
    IFile &operator>>( unsigned int &val );
    IFile &operator>>( long &val );
    IFile &operator>>( unsigned long &val );
    IFile &operator>>( long long &val );
    IFile &operator>>( unsigned long long &val );
    IFile &operator>>( float &val );
    IFile &operator>>( double &val );
    IFile &operator>>( long double &val );
    IFile &operator>>( void* &val );
    IFile &operator>>( std::streambuf *sb );
    /* IFile &operator>> ( IFile &( *pf )( IFile &) ); */
    IFile &operator>>( std::istream & ( *pf )( std::istream & ) );
    IFile &operator>>( std::ios & ( *pf )( std::ios & ) );
    IFile &operator>>( std::ios_base & ( *pf )( std::ios_base & ) );
    std::streamsize gcount( );
    int get( );
    IFile &get( char &c );
    IFile &get( char *s, std::streamsize n );
    IFile &get( char *s, std::streamsize n, char delim );
    IFile &get( std::streambuf &sb );
    IFile &get( std::streambuf &sb, char delim );
    IFile &getline( char *s, std::streamsize n );
    IFile &getline( char *s, std::streamsize n, char delim );
    IFile &ignore( std::streamsize n = 1, int delim = EOF );
    IFile &read( char *s, std::streamsize n );
    IFile &seekg( std::streampos pos );
    IFile &seekg( std::streamoff off, std::ios_base::seekdir way );
    std::ios_base::iostate exceptions( ) const;
    void exceptions( std::ios_base::iostate except );
  };

  /* Functions: std::istream non-member functions for IFile 
   * @date 2013/Aug/21 
   * @version 1.0.00 
   * @param file: An IFile object. 
   * @param str: A string. 
   * @param delim: String delineator. 
   * @param c: A character. 
   * @param s: A string. 
   * @return Same as file. 
   * @brief Same behavior as in std::istream class. 
   * @warning none. 
   */
  IFile &getline( IFile &file, std::string &str, char delim );
  IFile &getline( IFile && file, std::string & str, char delim );
  IFile &getline( IFile &file, std::string &str );
  IFile &getline( IFile && file, std::string & str );
  IFile &operator>>( IFile &file, char &c );
  IFile &operator>>( IFile &file, signed char &c );
  IFile &operator>>( IFile &file, unsigned char &c );
  IFile &operator>>( IFile &file, char *s );
  IFile &operator>>( IFile &file, signed char *s );
  IFile &operator>>( IFile &file, unsigned char *s );

  class OFile {

    friend OFile &operator<<( OFile &file, char c );
    friend OFile &operator<<( OFile &file, signed char c );
    friend OFile &operator<<( OFile &file, unsigned char c );
    friend OFile &operator<<( OFile &file, const char *s );
    friend OFile &operator<<( OFile &file, const signed char *s );
    friend OFile &operator<<( OFile &file, const unsigned char *s );

  private:

    std::ofstream std_file;

  public:

    OFile( );
    explicit OFile( const char *filename, std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary );
    explicit OFile( const std::string &filename,
                    std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary );
    OFile( const OFile &other ) = delete;
    ~OFile( );
    void open( const char *filename, std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary );
    void open( const std::string &filename, std::ios_base::openmode mode = std::ios_base::out | std::ios_base::binary );
    bool is_open( ) const;
    bool eof( ) const;
    void close( );
    OFile &operator<<( bool val );
    OFile &operator<<( short val );
    OFile &operator<<( unsigned short val );
    OFile &operator<<( int val );
    OFile &operator<<( unsigned int val );
    OFile &operator<<( long val );
    OFile &operator<<( unsigned long val );
    OFile &operator<<( long long val );
    OFile &operator<<( unsigned long long val );
    OFile &operator<<( float val );
    OFile &operator<<( double val );
    OFile &operator<<( long double val );
    OFile &operator<<( void *val );
    OFile &operator<<( std::streambuf *sb );
    OFile &operator<<( OFile & ( *pf )( OFile & ) );
    OFile &operator<<( std::ostream & ( *pf )( std::ostream & ) );
    OFile &operator<<( std::ios & ( *pf )( std::ios & ) );
    OFile &operator<<( std::ios_base & ( *pf )( std::ios_base & ) );
    OFile &put( char c );
    OFile &write( const char *s, std::streamsize n );
    std::streampos tellp( );
    OFile &seekp( std::streampos pos );
    OFile &seekp( std::streamoff off, std::ios_base::seekdir way );
    std::ios_base::iostate exceptions( ) const;
    void exceptions( std::ios_base::iostate except );
  };

  /* Non-member std::ostream functions definition for OFile */
  OFile &operator<<( OFile &file, char c );
  OFile &operator<<( OFile &file, signed char c );
  OFile &operator<<( OFile &file, unsigned char c );
  OFile &operator<<( OFile &file, const char *s );
  OFile &operator<<( OFile &file, const signed char *s );
  OFile &operator<<( OFile &file, const unsigned char *s );

  class File {
  public:

    /**
     * @date 2013/Aug/08 
     * @param filename: A filename. 
     * @param pos: Position of the first character in the filename to be considered in the search. 
     * @return The extension of the filename. 
     * @brief Returns the extension of the filename. The extension is the string composed by the first occurrence
     * of character '.' to the end of the filename. If no occurrence of '.' is found, empty string is
     * returned. 
     * @warning none. 
     */
    static std::string Extension( const std::string &filename, size_t pos = 0 );

    /**
     * @date 2013/Aug/08 
     * @param filename: A filename. 
     * @param pos: Position of the first character in the filename to be considered in the search. 
     * @return The extension of the filename. 
     * @brief Returns the extension of the filename in lower case characters. The extension is the string
     * composed by the first occurrence of character '.' to the end of the filename. If no occurrence of
     * '.' is found, empty string is returned. 
     * @warning none. 
     */
    static std::string ToLowerExtension( const std::string &filename, size_t pos = 0 );

    /**
     * @date 2013/Aug/08 
     * @param filename: A filename. 
     * @param pos: Position of the last character in the filename to be considered in the search. 
     * @return The directory of the filename. 
     * @brief Returns the directory of the filename. The directory is the leftmost part of the filename up to the
     * last occurrence of character '/' for Linux ana '\' for Windows. If no occurrence of '/' or '\' is
     * found, empty string is returned. 
     * @warning none. 
     */
    static std::string Directory( const std::string &filename, size_t pos = std::string::npos );

    /**
     * @date 2013/Aug/20 
     * @param filename: A filename. 
     * @param with_directory: Whether or not to to keep directory. 
     * @return Filename basename. 
     * @brief Returns the file basename of the filename. It may keep the directory name optionaly. 
     * @warning none. 
     */
    static std::string Basename( const std::string &filename, bool keep_directory = false );

    /**
     * @date 2013/Aug/20 
     * @param filename: A filename. 
     * @return A string with located case sensitive extension of input filename. 
     * @brief Searches base on filename directory/basename for a filename that exists independently of extension
     * characters case. 
     * @warning Must be optimized to change only letters. 
     */
    static std::string LocateCaseSensitiveFileName( const std::string &filename );

  };

}

#endif
