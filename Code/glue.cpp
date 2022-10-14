
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

ImageOperations* EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_ImageOperations_1(char* base46Str) {
  return new ImageOperations(base46Str);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_createImage_1(ImageOperations* self, char* base46Str) {
  self->createImage(base46Str);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_grayscale_avg_0(ImageOperations* self) {
  self->grayscale_avg();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_grayscale_lum_0(ImageOperations* self) {
  self->grayscale_lum();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_colorMask_3(ImageOperations* self, float r, float g, float b) {
  self->colorMask(r, g, b);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_encodeMessage_1(ImageOperations* self, char* message) {
  self->encodeMessage(message);
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_encodeByte_0(ImageOperations* self) {
  return self->encodeByte();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_printBuffer_4(ImageOperations* self, char** buffer, int buffer_length, char** operations, int ops_length) {
  self->printBuffer(buffer, buffer_length, operations, ops_length);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations_Undo_0(ImageOperations* self) {
  self->Undo();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ImageOperations___destroy___0(ImageOperations* self) {
  delete self;
}

}

