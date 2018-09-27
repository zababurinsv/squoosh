#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "common/video_common.h"


using namespace emscripten;

int version() {
  return aom_codec_version();
}

void encode(std::string image_in, int width, int height) {
  AvxVideoInfo info;
  aom_image_t raw;
  aom_codec_ctx_t codec;
  aom_codec_enc_cfg_t cfg;
  const AvxInterface *encoder = get_aom_encoder_by_name("av1");

  info.codec_fourcc = encoder->fourcc;
  info.frame_width = width;
  info.frame_height = height;

  if (!aom_img_alloc(&raw, AOM_IMG_FMT_I420, info.frame_width, info.frame_height, 1)) {
    die("Failed to allocate image.");
  }

  aom_codec_enc_config_default(encoder->codec_interface(), &cfg, 0);
  cfg.g_w = info.frame_width;
  cfg.g_h = info.frame_height;

  if (aom_codec_enc_init(&codec, encoder->codec_interface(), &cfg, 0)) {
    die_codec(&codec, "Failed to initialize encoder");
  }
}

EMSCRIPTEN_BINDINGS(my_module) {
  function("version", &version);
  function("encode", &encode);
}
