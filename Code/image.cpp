#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "base64.cpp"
#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <stdlib.h>

Image::Image() {
    
}

Image::Image(char* input) {
    std::string input_str = input;
    std::string decoded_data = base64_decode(input_str);
    std::vector<char> img_data(decoded_data.begin(), decoded_data.end());
    loadFromMemory(img_data);
}


// Image::Image(const char* filename) {
//      if (read(filename)){
//         printf("Read %s\n", filename);
//         size = w*h*channels;
//     }else {
//         printf("Failed to read %s\n", filename);
//     }

// }

// Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
//     size = w*h*channels;
//     data = new uint8_t[size];
// }

// Image:: Image(int w, int h, int channels, size_t size) {
//     this->size = size;
//     this->w = w;
//     this->h = h;
//     this->channels = channels;
// }

// Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
//     memcpy(data, img.data, size);
// }

Image::~Image() {
    stbi_image_free(data);
}

bool Image::read(const char* filename) {
    data = stbi_load(filename, &w, &h, &channels, 0);
    return data != NULL;

}
 
bool Image::write(const char* filename) {
    ImageType type = getFileType(filename);
    int success;
    switch (type)
    {
    case PNG:
        success = stbi_write_png(filename, w, h, channels, data, w*channels);
        break;
    case BMP:
        success = stbi_write_bmp(filename, w, h, channels, data);
        break;
    case JPG:
        success = stbi_write_jpg(filename, w, h, channels, data, 100);
        break;
    case TGA:
        success = stbi_write_tga(filename, w, h, channels, data);
        break;
    default:
        break;
    }

    return success != 0;

}

ImageType Image::getFileType(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (ext != nullptr) {
        if(strcmp(ext, ".png") == 0){
            return PNG;

        }
        else if(strcmp(ext, ".jpg") == 0){
            return JPG;

        }
        else if(strcmp(ext, ".bmp") == 0){
            return BMP;

        }
        else if(strcmp(ext, ".tga") == 0){
            return TGA;

        }else {
            return PNG;
        }
    }

    return PNG;
}

void Image::grayscale_avg() {
    if (channels < 3) {
        printf("Image has less than 3 channels and therefore assumed to be grayscaled");
    }else {
        for(int i = 0; i < size; i+=channels) {
            int gray = (data[i] + data[i+1] + data[i+2]) / 3;
            memset(data+i, gray, 3);
        }
        //return *this;
    }
    //return *this;

}
Image& Image::grayscale_lum() {

if (channels < 3) {
        printf("Image has less than 3 channels and therefore assumed to be grayscaled");
    }else {
        for(int i = 0; i < size; i+=channels) {
            int gray = 0.2126 * data[i] + 0.7152* data[i+1] + 0.0722 * data[i+2];
            memset(data+i, gray, 3);
        }
        return *this;
    }
    printf("I am about to return");
    return *this;
}

Image& Image::colorMask(float r, float g, float b) {
    if (channels < 3) {
        printf("\e[31m[ERROR] Color mask requires at leat 3 channels but this image has less");
    }
    else if (r > 1.0 || g > 1.0 || b > 1.0) {
        printf("\e[31m[ERROR] r g b values cannot be greater tha 1.0");
    }
    else {
        for (int i = 0; i < size; i+=channels) {
            data[i] *= r;
            data[i+1] *= g;
            data[i+2] *= b;
        }
    }
    return *this;
    

}

 Image& Image::encodeMessage(const char* message) {

    uint32_t len = strlen(message) * 8;

    if (len + STEG_HEADER_SIZE > size){
        printf("\e[31m[ERROR] Tis message is too large (%lu bits / %zu bits)\e[0m\n", len+STEG_HEADER_SIZE, size);
        return *this;
    }
    printf("LENGTHL %d\n", len);

    for(uint8_t i = 0; i < STEG_HEADER_SIZE;  ++i) {
        data[i] &= 0xFE;
        data[i] |= (len >> (STEG_HEADER_SIZE - 1 - i)) & 1UL;
    }

    for(uint32_t i = 0; i < len;  ++i) {
        data[i+STEG_HEADER_SIZE] &= 0xFE;
        data[i+STEG_HEADER_SIZE] |= (message[i/8] >> ((len-1-i)% 8)) & 1;
    }

    return *this;

 }

Image& Image::decodeMessage(char* buffer, size_t* messageLength) {
    uint32_t len = 0;
    for (uint8_t i = 0; i < STEG_HEADER_SIZE; ++i) {
        len = (len << 1) | (data[i] & 1);
    }
    printf("LENGTHL %d\n", len);
    *messageLength = len;

    for (uint32_t i = 0; i < len; ++i) {
        buffer[i/8] = (buffer[i/8] << 1) | (data[i+STEG_HEADER_SIZE] & 1);
    }
    return *this;

}

void Image::loadFromMemory(const std::vector<char> &img_data)
{
	data  = stbi_load_from_memory((const stbi_uc *)img_data.data(), img_data.size(), &w, &h, &channels, 0);
    size = w*h*channels;
}

std::string Image::decodeByte(const std::string input) {
    return base64_decode(input);
}

char* Image::encodeByte() {
    int len;
    unsigned char *imageData = stbi_write_png_to_mem((const unsigned char *) data, w*channels, w, h, channels, &len);
    //STBIW_FREE(png);
    std::string enc = base64_encode(imageData, len);
    char* buffer = (char*)malloc(sizeof(char) * len);
    strcpy(buffer, enc.c_str());
    return buffer;
    
}


//https://www.youtube.com/watch?v=ATkbOckoCZc&list=PLG5M8QIx5lkzdGkdYQeeCK__As6sI2tOY&index=4
//https://github.com/ReneNyffenegger/cpp-base64/blob/master/test.cpp

