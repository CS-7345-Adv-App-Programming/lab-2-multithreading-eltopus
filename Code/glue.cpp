
#include <emscripten.h>

extern "C" {

// Not using size_t for array indices as the values used by the javascript code are signed.

EM_JS(void, array_bounds_check_error, (size_t idx, size_t size), {
  throw 'Array index ' + idx + ' out of bounds: [0,' + size + ')';
});

void array_bounds_check(const int array_size, const int array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    array_bounds_check_error(array_idx, array_size);
  }
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// ImageOperations

ImageOperations* EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_ImageOperations_0() {
  return new ImageOperations();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_createImages_1(ImageOperations* self, char* base46Str) {
  self->createImages(base46Str);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_grayscale_avgs_0(ImageOperations* self) {
  self->grayscale_avgs();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_grayscale_lums_0(ImageOperations* self) {
  self->grayscale_lums();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_colorMasks_3(ImageOperations* self, float r, float g, float b) {
  self->colorMasks(r, g, b);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_encodeMessages_1(ImageOperations* self, char* message) {
  self->encodeMessages(message);
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_encodeBytes_0(ImageOperations* self) {
  return self->encodeBytes();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_std_convolve_clamp_to_0_5(ImageOperations* self, unsigned char channel, unsigned int ker_w, unsigned int ker_h, unsigned int cr, unsigned int cc) {
  self->std_convolve_clamp_to_0(channel, ker_w, ker_h, cr, cc);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_std_convolve_clamp_to_border_5(ImageOperations* self, unsigned char channel, unsigned int ker_w, unsigned int ker_h, unsigned int cr, unsigned int cc) {
  self->std_convolve_clamp_to_border(channel, ker_w, ker_h, cr, cc);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_Undos_0(ImageOperations* self) {
  self->Undos();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations___destroy___0(ImageOperations* self) {
  delete self;
}

}

