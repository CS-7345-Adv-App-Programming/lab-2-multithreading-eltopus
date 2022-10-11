#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdint.h>
#include <cstdio>
#include <string>
#include <vector>
#include "stb_image.h"
#include "stb_image_write.h"
#include "base64.cpp"
#include<cstdlib>



const unsigned long STEG_HEADER_SIZE = sizeof(uint32_t) * 8;

enum Type {
    PNG, JPG, BMP, TGA
};



class Image 
{
        
    public:

        Image(int w, int h, int channels) : w(w), h(h), channels(channels) {}

        Image(const Image& img) : Image(img.w, img.h, img.channels) {}

        Image(char* input) {}

        Image() {

        }

        Image(Image& img) {}

        virtual ~Image() {
            stbi_image_free(data);
        }
        
        

        virtual Image* state() const = 0;

        virtual void grayscale_avg() const = 0;

        virtual void grayscale_lum() const = 0;
        
        virtual void colorMask(float r, float g, float b) const = 0;

        virtual void encodeMessage(const char* message) const = 0;

        virtual void decodeMessage(char* buffer, size_t* messageLength) const = 0;

        virtual char* encodeByte() const = 0;

        virtual bool write(const char* filename) = 0;

        virtual Type getFileType(const char* filename) = 0;



    // protected:
        uint8_t* data = NULL;
        size_t size = 0;
        int w;
        int h;
        int channels;
        int random;
        

};