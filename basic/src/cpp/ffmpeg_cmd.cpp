//
// Created by binlee on 23-12-12.
//

#include "ffmpeg_cmd.h"
#include "cmd_runner.h"
#include "nlohmann/json.hpp"

#include "iostream"
#include "fstream"
#include "sstream"
#include <sys/stat.h>

std::string timeFormatTest(float time) {
    // 时间格式为： 00:01:30.500
    int h = 0, m = 0;
    if (time > 3600) {
        h = (int) time / 3600;
        time = time - h * 3600;
    }
    if (time > 60) {
        m = (int) time / 60;
        time = time - m * 60;
    }
    float s = time;
    std::stringstream r;
    if (h < 10) {
        r << "0" << h << ":";
    } else {
        r << h << ":";
    }
    if (m < 10) {
        r << "0" << m << ":";
    } else {
        r << m << ":";
    }
    if (s < 10) {
        r << "0" << s;
    } else {
        r << s;
    }
    return r.str();
}

void ffmpegVideoReverseTest(const std::string &input_file, const std::string &output_file) {
    auto tmp_dir = std::string("/tmp/") + std::to_string(random()) + "x" + std::to_string(random());
    std::cout << "tmp dir: " << tmp_dir << std::endl;
    run_pipe("mkdir -p " + tmp_dir);
    // 1、切割视频
    // ffmpeg -i input.mp4 -c copy -map 0 -segment_time 300 -f segment output%d.mp4
    run_pipe("ffmpeg -v error -i " + input_file + " -c copy -map 0 -segment_time 5 -f segment -y " +
             tmp_dir + "/part-%d.mp4");
    // 2、 获取视频时长并计算文件分片数
    auto _command = std::string("ffprobe -v error -show_format -show_streams -of json ") + input_file;
    auto probe = nlohmann::json::parse(run_pipe(_command).second);
    assert(probe.is_object());
    auto dj = probe["format"]["duration"];
    auto duration = dj.is_number_float() ? dj.get<float>() * 1000 : std::stof(dj.get<std::string>()) * 1000;
    std::cout << "duration is: " << duration << "ms" << ", segments: " << ceilf(duration / 5000) << std::endl;

    // 检测切割是否成功
    auto last_part = tmp_dir + "/part-" + std::to_string(((int) ceilf(duration / 5000)) - 1) + ".mp4";
    struct stat s{};
    if (stat(last_part.c_str(), &s) != 0) {
        std::cerr << "split file failed" << std::endl;
        std::cout << run_pipe("ls -alh " + tmp_dir).second << std::endl;
        return;
    }

    auto part_files = std::vector<std::string>((int) ceilf(duration / 5000));
    auto part_rfiles = std::vector<std::string>(part_files.capacity());
    for (int i = 0; i < part_files.capacity(); ++i) {
        part_files[i] = tmp_dir + "/part-" + std::to_string(i) + ".mp4";
        part_rfiles[i] = tmp_dir + "/part-r-" + std::to_string(i) + ".mp4";
    }
    // 3、分段倒放
    for (int i = 0; i < part_files.size(); ++i) {
        // ffmpeg -i output1.mp4 -vf reverse reversed_output1.mp4
        run_pipe("ffmpeg -v error -i " + part_files[i] + " -vf reverse -y " + part_rfiles[i]);
    }
    // 4.1、倒放后的视频逆序后合并
    std::reverse(part_rfiles.begin(), part_rfiles.end());
    auto merge_file = tmp_dir + "/all-r-files.txt";
    auto fs = std::ofstream(merge_file);
    if (!fs.is_open()) return;
    for (const auto &prf: part_rfiles) {
        fs << "file '" << prf << "'" << std::endl;
    }
    fs.close();
    // 4.2、合并视频
    // ffmpeg -i "concat:reversed_output1.mp4|reversed_output2.mp4|reversed_output3.mp4" -c copy reversed_full.mp4
    run_pipe("ffmpeg -v error -f concat -safe 0 -i " + merge_file + " -c copy -y " + output_file);

    // 播放倒放后的视频
    std::cout << run_pipe("ls -alh " + tmp_dir).second << std::endl;
    // pipeTest("ffplay -v error " + input_file);
    run_pipe("ffplay -v error " + output_file);

    // std::__fs::filesystem::remove_all(tmp_dir);
}

void ffmpegAudioSplitTest(const std::string &input_file, const std::string &config_file) {
    // // ffmpeg -i input.mp3 -ss 00:01:30 -to 00:02:00 output.mp3
    std::ifstream ifs(config_file);
    auto json = nlohmann::json::parse(ifs);
    ifs.close();

    nlohmann::json split_result(json["data"]);
    for (auto &item: split_result) {
        std::cout << item.dump() << std::endl;
        auto start = timeFormatTest(item["start"].get<float>());
        auto end = timeFormatTest(item["end"].get<float>());
        std::stringstream sub_wav;
        sub_wav << "/home/binlee/code/Clearn/basic/testdata/split/" << start << "-" << end << ".wav";
        std::cout << sub_wav.str() << std::endl;
        // 根据起始和结束时间来分割音频
        // ffmpeg -i input.mp3 -ss 00:01:30 -to 00:02:00 output.mp3
        std::stringstream cmd_;
        cmd_ << "ffmpeg -v error -i " << input_file << " -ss " << start << " -to " << end << " -y " << sub_wav.str();
        run_pipe(cmd_.str());
        item.erase("start");
        item.erase("end");
        item["url"] = sub_wav.str();
    }

    // 结果写入文件中
    std::ofstream out("/home/binlee/code/Clearn/basic/testdata/split_result.json");
    out << split_result << std::endl;
    out.close();
}

void ffmpegAudioConvertTest(const std::string &input_file, const std::string &output_file) {
    run_pipe("ffmpeg -v error -i " + input_file + " -acodec pcm_s16le -y " + output_file);
    run_pipe("ffprobe " + output_file + "; ffplay -v error " + output_file);
}

void ffmpegVideoMergeTest(const std::vector<std::string> &input_files, const std::string &output_file) {
    // ffmpeg -i input1.mp4 -i input2.mp4 -filter_complex "[0:v][0:a][1:v][1:a]concat=n=2:v=1:a=1" -strict -2 -y output.mp4
    auto cmd = std::string("ffmpeg -v error");
    for (const auto &item: input_files) {
        cmd += (" -i " + item);
    }
    cmd += " -filter_complex \"";
    for (int i = 0; i < input_files.size(); ++i) {
        cmd += ("[" + std::to_string(i) + ":v][" + std::to_string(i) + ":a]");
    }
    cmd += ("concat=n=" + std::to_string(input_files.size()) + ":v=1:a=1");
    cmd += ("\" -strict -2 -y " + output_file);
    std::cout << "cmd is: " << cmd << std::endl;
    auto r = run_pipe(cmd);
    if (r.second == PIPE_ERROR) return;

    std::cout << run_pipe("ls -alh " + output_file).second << std::endl;
    run_pipe("vlc " + output_file);
}

std::string AudioConvert(const std::string &raw_file) {
    auto pos = raw_file.rfind(".wav");
    if (pos != std::string::npos && raw_file.substr(pos) == ".wav") {
        auto converted_file = "/tmp/hhh.wav";
        if (run_pipe("ffmpeg -v error -i " + raw_file + " -ar 16000 -y " + converted_file).first != 0) abort();
        return converted_file;
    }
    return raw_file;
}
