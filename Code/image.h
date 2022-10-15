
#pragma once

#ifndef IMAGE_H
#define IMAGE_H


#include <stdint.h>
#include <cstdio>
#include <string>
#include <vector>
#include <iomanip>
#include <complex>


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

    Image(const char* input);

     virtual ~Image();
    

    void grayscale_avg();

    void grayscale_lum();
    
    void colorMask(float r, float g, float b);

    void encodeMessage(const char* message);

    void decodeMessage(char* buffer, size_t* messageLength);

    bool write(const char* filename);

    Type getFileType(const char* filename);

    char* encodeByte();

    std::string encodeByteString();

    Image& std_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

    void std_convolve_clamp_to_0v(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

    Image& std_convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) ;

    void std_convolve_clamp_to_borderv(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) ;

    Image& std_convolve_cyclic(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

    uint32_t rev(uint32_t n, uint32_t a);

    void bit_rev(uint32_t n, std::complex<double> a[], std::complex<double>* A);

    void fft(uint32_t n, std::complex<double> x[], std::complex<double>* X);

    void ifft(uint32_t n, std::complex<double> X[], std::complex<double>* x);

    void dft_2D(uint32_t m, uint32_t n, std::complex<double> x[], std::complex<double>* X);

    void idft_2D(uint32_t m, uint32_t n, std::complex<double> X[], std::complex<double>* x);

    void loadFromMemory(const std::vector<char> &img_data);

    void pad_kernel(uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc, uint32_t pw, uint32_t ph, std::complex<double>* pad_ker);

    void pointwise_product(uint64_t l, std::complex<double> a[], std::complex<double> b[], std::complex<double>* p);

    Image& fd_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

    Image& fd_convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

    Image& fd_convolve_cyclic(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

    Image& convolve_linear(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

    Image& convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);

    Image& convolve_cyclic(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc);
    
    


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