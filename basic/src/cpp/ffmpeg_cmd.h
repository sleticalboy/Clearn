//
// Created by binlee on 23-12-12.
//

#ifndef BASIC_FFMPEG_CMD_H
#define BASIC_FFMPEG_CMD_H

#include "string"

void ffmpegVideoReverseTest(const std::string &input_file, const std::string &output_file);

void ffmpegAudioSplitTest(const std::string &input_file, const std::string &config_file);

void ffmpegAudioConvertTest(const std::string &input_file, const std::string &output_file);

void ffmpegVideoMergeTest(const std::vector<std::string> &input_files, const std::string &output_file);

std::string AudioConvert(const std::string &raw_file);

#endif //BASIC_FFMPEG_CMD_H
