/*
 * File:   errors.h
 * Author: Samuel Felipe dos Santos
 *
 * Created on 11 de Setembro de 2015, 09:53
 */

#ifndef ERRORS_H
#define	ERRORS_H

//from Image, ImageIO and ImageOperators
#define OPEN_ERROR "error: could not open file"
#define FORMAT_ERROR "error: image format not supported"
#define IMAGE_SIZE_ERROR "error: image size doesn't match"
#define IMAGE_ERROR "error: empty image"
#define IMAGE_DEPTH_ERROR "error: image doesn't have a three-dimensional color space"

//from VideoIO
#define FFMPEG_COVERSION_ERROR "error: Cannot initialize the conversion context";
#define FFMPEG_STREAM_ERROR "error: Could not find stream information";
#define FFMPEG_VIDEO_STREAM_ERROR "error: Could not find a video stream";
#define FFMPEG_CODEC_ERROR "error: Codec not found";
#define FFMPEG_CODEC_OPEN_ERROR "error: Could not open codec";
#define VIDEO_OPEN_ERROR "error: there is no video open";
#define FFMEPEG_DECODING_ERROR "Error while decoding frame";


#define VIDEO_OPEN_ERROR "error: there is no video open";

#endif	/* ERRORS_H */
