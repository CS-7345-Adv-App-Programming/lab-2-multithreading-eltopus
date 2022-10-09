#pragma once

#ifndef IMAGE_H
#define IMAGE_H


#include <stdint.h>
#include <cstdio>
#include <string>
#include <vector>

const unsigned long STEG_HEADER_SIZE = sizeof(uint32_t) * 8;

enum ImageType {
    PNG, JPG, BMP, TGA
};



struct Image 
{
    uint8_t* data = NULL;
    size_t size = 0;
    int w;
    int h;
    int channels;
    
    // Image(const char* filename);
    Image();
    Image(char* input);
    Image(int w, int h, int channels);
    // Image(int w, int h, int channels, size_t size);
    // Image(const Image& img);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);
    void loadFromMemory(const std::vector<char> &img_data);

    ImageType getFileType(const char* filename);
    void grayscale_avg();
    Image& grayscale_lum();
    Image& colorMask(float r, float g, float b);
    Image& encodeMessage(const char* message);
    Image& decodeMessage(char* buffer, size_t* messageLength);
    std::string decodeByte(const std::string input); 
    char* encodeByte();

};

 #endif
