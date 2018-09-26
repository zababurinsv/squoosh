#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "aom/aom_codec.h"


using namespace emscripten;

int version() {
  return aom_codec_version();
}

EMSCRIPTEN_BINDINGS(my_module) {
  function("version", &version);
}
