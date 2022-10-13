
#pragma once

#ifndef IMAGE_H
#define IMAGE_H


#include <stdint.h>
#include <cstdio>
#include <string>
#include <vector>


const unsigned long STEG_HEADER_SIZE = sizeof(uint32_t) * 8;

enum Type {
    PNG, JPG, BMP, TGA
};

extern bool write(const char* filename);


struct Image 
{

    Image(int w, int h, int channels);

    Image(const Image& img);

    Image(char* input);

    Image() {

    }

    Image(Image& img);

    Image(Image* imageState);

     virtual ~Image();
    

    void grayscale_avg();

    void grayscale_lum();
    
    void colorMask(float r, float g, float b);

    void encodeMessage(const char* message);

    void decodeMessage(char* buffer, size_t* messageLength);

    bool write(const char* filename);

    Type getFileType(const char* filename);

    char* encodeByte();

    void loadFromMemory(const std::vector<char> &img_data);
    
    


// protected:
    uint8_t* data = NULL;
    size_t size = 0;
    int w;
    int h;
    int channels;
    int random; 
    Image* state_;  

};

 #endif