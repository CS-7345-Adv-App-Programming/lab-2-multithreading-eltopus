
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION



#include "base64.cpp"
#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>


void Image::loadFromMemory(const std::vector<char> &img_data) {
    data  = stbi_load_from_memory((const stbi_uc *)img_data.data(), img_data.size(), &w, &h, &channels, 0);
    size = w*h*channels;
}

char* Image::encodeByte() {
    int len;
    unsigned char *imageData = stbi_write_png_to_mem((const unsigned char *) data, w*channels, w, h, channels, &len);
    //STBIW_FREE(png);
    std::string enc = base64_encode(imageData, len);
    char* buffer = (char*)malloc(sizeof(char) * len);
    // std:: cout << len << std::endl;
    // strncpy(buffer, enc.c_str(), len-1);
    strcpy(buffer, enc.c_str());
    // buffer[len-1] = '\0';
    
    return buffer;
}

std::string Image::encodeByteString() {
    int len;
    unsigned char *imageData = stbi_write_png_to_mem((const unsigned char *) data, w*channels, w, h, channels, &len);
    //STBIW_FREE(png);
    std::string enc = base64_encode(imageData, len);
    return enc;
}

void Image::grayscale_avg() {
    if (channels < 3) {
        printf("Image has less than 3 channels and therefore assumed to be grayscaled");
    }else {
        for(int i = 0; i < size; i+=channels) {
            int gray = (data[i] + data[i+1] + data[i+2]) / 3;
            memset(data+i, gray, 3);
        }
    }
}

void Image::grayscale_lum()  {
    if (channels < 3) {
            printf("Image has less than 3 channels and therefore assumed to be grayscaled");
        }else {
            for(int i = 0; i < size; i+=channels) {
                int gray = 0.2126 * data[i] + 0.7152* data[i+1] + 0.0722 * data[i+2];
                memset(data+i, gray, 3);
            }
        }
        
}
    
void Image::colorMask(float r, float g, float b) {
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

}

void Image::encodeMessage(const char* message) {
    uint32_t len = strlen(message) * 8;
    if (len + STEG_HEADER_SIZE > size){
        printf("\e[31m[ERROR] Tis message is too large (%lu bits / %zu bits)\e[0m\n", len+STEG_HEADER_SIZE, size);
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
}

void Image::decodeMessage(char* buffer, size_t* messageLength){
    uint32_t len = 0;
    for (uint8_t i = 0; i < STEG_HEADER_SIZE; ++i) {
        len = (len << 1) | (data[i] & 1);
    }
    printf("LENGTHL %d\n", len);
    *messageLength = len;

    for (uint32_t i = 0; i < len; ++i) {
        buffer[i/8] = (buffer[i/8] << 1) | (data[i+STEG_HEADER_SIZE] & 1);
    }

}

Type Image::getFileType(const char* filename)  {
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


bool Image::write(const char* filename) {
    Type type = getFileType(filename);
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

Image::Image(char* input){
    std::string input_str = input;
    std::string decoded_data = base64_decode(input_str);
    std::vector<char> img_data(decoded_data.begin(), decoded_data.end());
    loadFromMemory(img_data);
    this->state_ = this;

}

Image::Image(const char* input){
    std::string input_str = input;
    std::string decoded_data = base64_decode(input_str);
    std::vector<char> img_data(decoded_data.begin(), decoded_data.end());
    loadFromMemory(img_data);
    this->state_ = this;

}

Image::Image(Image& img) : Image(img.w, img.h, img.channels) {
    this->state_ = img.state_;
    memcpy(data, img.data, size);
}


Image::Image(int w, int h, int channels) {
    this->w = w;
    this->h = h;
    this->channels = channels;
    this->size = w*h*channels;
    this->data = new uint8_t[size];
}


Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
    this->state_ = img.state_;
    memcpy(data, img.data, size);
}

Image::~Image() {
    stbi_image_free(data);
}



