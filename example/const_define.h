#pragma once
#include "libavcodec/avcodec.h"

#define VIDEO_FPS 25

#define BASE_WIDTH 1920
#define BASE_HEIGHT 1080

//目前默认配置了6 个通道
enum SOURCE_CHANNELS
{
    SOURCE_CHANNEL_TRANSITION = 0,
    SOURCE_CHANNEL_AUDIO_OUTPUT, // 1
    SOURCE_CHANNEL_AUDIO_OUTPUT_2, // 2
    SOURCE_CHANNEL_AUDIO_INPUT, // 3
    SOURCE_CHANNEL_AUDIO_INPUT_2, // 4
    SOURCE_CHANNEL_AUDIO_INPUT_3, // 5
};

#define VIDEO_ENCODER_ID AV_CODEC_ID_H264
#define VIDEO_ENCODER_NAME "libx264"
#define RECORD_OUTPUT_FORMAT "mp4"
#define RECORD_OUTPUT_FORMAT_MIME "video/mp4"
#define VIDEO_FPS 25
#define AUDIO_BITRATE 128
#define VIDEO_BITRATE 2000
#define BASE_WIDTH 1280
#define BASE_HEIGHT 720