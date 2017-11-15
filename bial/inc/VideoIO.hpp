#ifndef VIDEOIO_H
#define VIDEOIO_H

#include "Color.hpp"
#include "Errors.hpp"
#include "Image.hpp"
#include <iostream>
#include <string>

namespace Bial {

  class VideoIO {
public:
    virtual void Open( std::string file_name ) = 0;
    virtual void Close( ) = 0;
    virtual Image< Color >* GetFrame( ) = 0;
  };

}

#endif
