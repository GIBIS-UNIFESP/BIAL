#ifndef VIDEOIO_H
#define VIDEOIO_H

#include "Color.hpp"
#include "Image.hpp"
#include "errors.h"
#include <iostream>
#include <string>

using namespace std;
using namespace Bial;

class VideoIO
{
  public:
    virtual void Open(string file_name)=0;
    virtual void Close( )=0;
    virtual Image<Color> * GetFrame( )=0;
};

#endif
