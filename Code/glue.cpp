
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

// Image

Image* EMSCRIPTEN_KEEPALIVE emscripten_bind_Image_Image_1(char* base46Str) {
  return new Image(base46Str);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Image_grayscale_avg_0(Image* self) {
  self->grayscale_avg();
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_Image_encodeByte_0(Image* self) {
  return self->encodeByte();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Image___destroy___0(Image* self) {
  delete self;
}

}

