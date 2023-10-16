
#include <cstdio>
#include <iostream>
#include <csignal>

extern "C" {
#include <libavutil/channel_layout.h> // 用户音频声道布局操作
#include <libavutil/opt.h> // 设置操作选项操作
#include <libavutil/mathematics.h> // 用于数学相关操作
#include <libavutil/timestamp.h> // 用于时间戳操作
#include <libavformat/avformat.h> // 用 于 封装与解封装操作
#include <libswscale/swscale.h> // 用于缩放、转换颜色格式操作
#include <libswresample/swresample.h> // 用于进行音频采样率操作
}

int main(int argc, char **argv) {
  std::cout << "main() argc: " << argc << ", argv: " << argv[0] << std::endl;
  av_register_all();

  // 程序运行当前目录
  // std::cout << get_current_dir_name() << std::endl;

  auto root_ = std::string("/home/binlee/code/Clearn/ffmpeger/testdata/");
  auto input_ = root_ + "dance-4k.mp4";


  AVFormatContext *ctx;
  avformat_alloc_output_context2(&ctx, nullptr, "flv", input_.c_str());
  if (ctx == nullptr) {
    std::cerr << "can't alloc flv format" << std::endl;
    return 1;
  }
  AVOutputFormat *fmt = ctx->oformat;

  AVStream *st;
  AVCodecContext *c;
  st = avformat_new_stream(ctx, nullptr);
  if (st == nullptr) {
    std::cerr << "can't alloc flv stream" << std::endl;
    return 1;
  }
  st->id = (int)(ctx->nb_streams - 1);
  c->bit_rate = 400000;
  c->codec_id = AV_CODEC_ID_012V;
  c->width = 352;
  c->height = 288;
  st->time_base = {1, 25};
  c->time_base = st->time_base;
  c->gop_size = 12;
  c->pix_fmt = AV_PIX_FMT_YUV420P;
  return 0;
}