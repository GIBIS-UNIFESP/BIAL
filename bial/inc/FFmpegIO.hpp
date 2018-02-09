#ifndef FFMPEGIO_H
#define FFMPEGIO_H

#include "VideoIO.hpp"

#include <iostream>
#include <string>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace Bial {

  class FFmpegIO : public VideoIO {
private:
    bool is_open;

    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame;
    int videoStream;
    double video_clock;
    unsigned long frame_count;
    AVPacket packet;
    bool fFirstTime;

    Image< Color >* AVFrame2Image( );

public:
    FFmpegIO( std::string file_name );

    unsigned long GetFrameCount( );
    double GetVideoClock( );

    void Open( std::string file_name );
    void Close( );
    Image< Color >* GetFrame( );
  };

}

#endif
