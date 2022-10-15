
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)



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

Image& Image::std_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	uint8_t new_data[w*h];
	uint64_t center = cr*ker_w + cc;
	for(uint64_t k=channel; k<size; k+=channels) {
		double c = 0;
		for(long i = -((long)cr); i<(long)ker_h-cr; ++i) {
			long row = ((long)k/channels)/w-i;
			if(row < 0 || row > h-1) {
				continue;
			}
			for(long j = -((long)cc); j<(long)ker_w-cc; ++j) {
				long col = ((long)k/channels)%w-j;
				if(col < 0 || col > w-1) {
					continue;
				}
				c += ker[center+i*(long)ker_w+j]*data[(row*w+col)*channels+channel];
			}
		}
		new_data[k/channels] = (uint8_t)BYTE_BOUND(round(c));
	}
	for(uint64_t k=channel; k<size; k+=channels) {
		data[k] = new_data[k/channels];
	}
    return *this;
}

void Image::std_convolve_clamp_to_0v(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	uint8_t new_data[w*h];
	uint64_t center = cr*ker_w + cc;
	for(uint64_t k=channel; k<size; k+=channels) {
		double c = 0;
		for(long i = -((long)cr); i<(long)ker_h-cr; ++i) {
			long row = ((long)k/channels)/w-i;
			if(row < 0 || row > h-1) {
				continue;
			}
			for(long j = -((long)cc); j<(long)ker_w-cc; ++j) {
				long col = ((long)k/channels)%w-j;
				if(col < 0 || col > w-1) {
					continue;
				}
				c += ker[center+i*(long)ker_w+j]*data[(row*w+col)*channels+channel];
			}
		}
		new_data[k/channels] = (uint8_t)BYTE_BOUND(round(c));
	}
	for(uint64_t k=channel; k<size; k+=channels) {
		data[k] = new_data[k/channels];
	}
}


Image& Image::std_convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	uint8_t new_data[w*h];
	uint64_t center = cr*ker_w + cc;
	for(uint64_t k=channel; k<size; k+=channels) {
		double c = 0;
		for(long i = -((long)cr); i<(long)ker_h-cr; ++i) {
			long row = ((long)k/channels)/w-i;
			if(row < 0) {
				row = 0;
			}
			else if(row > h-1) {
				row = h-1;
			}
			for(long j = -((long)cc); j<(long)ker_w-cc; ++j) {
				long col = ((long)k/channels)%w-j;
				if(col < 0) {
					col = 0;
				}
				else if(col > w-1) {
					col = w-1;
				}
				c += ker[center+i*(long)ker_w+j]*data[(row*w+col)*channels+channel];
			}
		}
		new_data[k/channels] = (uint8_t)BYTE_BOUND(round(c));
	}
	for(uint64_t k=channel; k<size; k+=channels) {
		data[k] = new_data[k/channels];
	}

    return *this;
	
}


Image& Image::std_convolve_cyclic(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	uint8_t new_data[w*h];
	uint64_t center = cr*ker_w + cc;
	for(uint64_t k=channel; k<size; k+=channels) {
		double c = 0;
		for(long i = -((long)cr); i<(long)ker_h-cr; ++i) {
			long row = ((long)k/channels)/w-i;
			if(row < 0) {
				row = row%h + h;
			}
			else if(row > h-1) {
				row %= h;
			}
			for(long j = -((long)cc); j<(long)ker_w-cc; ++j) {
				long col = ((long)k/channels)%w-j;
				if(col < 0) {
					col = col%w + w;
				}
				else if(col > w-1) {
					col %= w;
				}
				c += ker[center+i*(long)ker_w+j]*data[(row*w+col)*channels+channel];
			}
		}
		new_data[k/channels] = (uint8_t)BYTE_BOUND(round(c));
	}
	for(uint64_t k=channel; k<size; k+=channels) {
		data[k] = new_data[k/channels];
	}

    return *this;
}


uint32_t Image::rev(uint32_t n, uint32_t a) {
	uint8_t max_bits = (uint8_t)ceil(log2(n));
	uint32_t reversed_a = 0;
	for(uint8_t i=0; i<max_bits; ++i) {
		if(a & (1<<i)) {
			reversed_a |= (1<<(max_bits-1-i));
		}
	}
	return reversed_a;
}

void Image::bit_rev(uint32_t n, std::complex<double> a[], std::complex<double>* A) {
	for(uint32_t i=0; i<n; ++i) {
		A[rev(n,i)] = a[i];
	}
}

void Image::fft(uint32_t n, std::complex<double> x[], std::complex<double>* X) {
	//x in standard order
	if(x != X) {
		memcpy(X, x, n*sizeof(std::complex<double>));
	}

	//Gentleman-Sande butterfly
	uint32_t sub_probs = 1;
	uint32_t sub_prob_size = n;
	uint32_t half;
	uint32_t i;
	uint32_t j_begin;
	uint32_t j_end;
	uint32_t j;
	std::complex<double> w_step;
	std::complex<double> w;
	std::complex<double> tmp1, tmp2;
	while(sub_prob_size>1) {
		half = sub_prob_size>>1;
		w_step = std::complex<double>(cos(-2*M_PI/sub_prob_size), sin(-2*M_PI/sub_prob_size));
		for(i=0; i<sub_probs; ++i) {
			j_begin = i*sub_prob_size;
			j_end = j_begin+half;
			w = std::complex<double>(1,0);
			for(j=j_begin; j<j_end; ++j) {
				tmp1 = X[j];
				tmp2 = X[j+half];
				X[j] = tmp1+tmp2;
				X[j+half] = (tmp1-tmp2)*w;
				w *= w_step;
			}
		}
		sub_probs <<= 1;
		sub_prob_size = half;
	}
	//X in bit reversed order
}
void Image::ifft(uint32_t n, std::complex<double> X[], std::complex<double>* x) {
	//X in bit reversed order
	if(X != x) {
		memcpy(x, X, n*sizeof(std::complex<double>));
	}

	//Cooley-Tukey butterfly
	uint32_t sub_probs = n>>1;
	uint32_t sub_prob_size;
	uint32_t half = 1;
	uint32_t i;
	uint32_t j_begin;
	uint32_t j_end;
	uint32_t j;
	std::complex<double> w_step;
	std::complex<double> w;
	std::complex<double> tmp1, tmp2;
	while(half<n) {
		sub_prob_size = half<<1;
		w_step = std::complex<double>(cos(2*M_PI/sub_prob_size), sin(2*M_PI/sub_prob_size));
		for(i=0; i<sub_probs; ++i) {
			j_begin = i*sub_prob_size;
			j_end = j_begin+half;
			w = std::complex<double>(1,0);
			for(j=j_begin; j<j_end; ++j) {
				tmp1 = x[j];
				tmp2 = w*x[j+half];
				x[j] = tmp1+tmp2;
				x[j+half] = tmp1-tmp2;
				w *= w_step;
			}
		}
		sub_probs >>= 1;
		half = sub_prob_size;
	}
	for(uint32_t i=0; i<n; ++i) {
		x[i] /= n;
	}
	//x in standard order
}

void Image::dft_2D(uint32_t m, uint32_t n, std::complex<double> x[], std::complex<double>* X) {
	//x in row-major & standard order
	std::complex<double>* intermediate = new std::complex<double>[m*n];
	//rows
	for(uint32_t i=0; i<m; ++i) {
		fft(n, x+i*n, intermediate+i*n);
	}
	//cols
	for(uint32_t j=0; j<n; ++j) {
		for(uint32_t i=0; i<m; ++i) {
			X[j*m+i] = intermediate[i*n+j]; //row-major --> col-major
		}
		fft(m, X+j*m, X+j*m);
	}
	delete[] intermediate;
	//X in column-major & bit-reversed (in rows then columns)
}
void Image::idft_2D(uint32_t m, uint32_t n, std::complex<double> X[], std::complex<double>* x) {
	//X in column-major & bit-reversed (in rows then columns)
	std::complex<double>* intermediate = new std::complex<double>[m*n];
	//cols
	for(uint32_t j=0; j<n; ++j) {
		ifft(m, X+j*m, intermediate+j*m);
	}
	//rows
	for(uint32_t i=0; i<m; ++i) {
		for(uint32_t j=0; j<n; ++j) {
			x[i*n+j] = intermediate[j*m+i]; //row-major <-- col-major
		}
		ifft(n, x+i*n, x+i*n);
	}
	delete[] intermediate;
	//x in row-major & standard order
}

void Image::pad_kernel(uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc, uint32_t pw, uint32_t ph, std::complex<double>* pad_ker) {
	//padded so center of kernel is at top left
	for(long i=-((long)cr); i<(long)ker_h-cr; ++i) {
		uint32_t r = (i<0) ? i+ph : i;
		for(long j=-((long)cc); j<(long)ker_w-cc; ++j) {
			uint32_t c = (j<0) ? j+pw : j;
			pad_ker[r*pw+c] = std::complex<double>(ker[(i+cr)*ker_w+(j+cc)], 0);
		}
	}
}
void Image::pointwise_product(uint64_t l, std::complex<double> a[], std::complex<double> b[], std::complex<double>* p) {
	for(uint64_t k=0; k<l; ++k) {
		p[k] = a[k]*b[k];
	}
}

Image& Image::fd_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	//calculate padding
	uint32_t pw = 1<<((uint8_t)ceil(log2(w+ker_w-1)));
	uint32_t ph = 1<<((uint8_t)ceil(log2(h+ker_h-1)));
	uint64_t psize = pw*ph;

	//pad image
	std::complex<double>* pad_img = new std::complex<double>[psize];
	for(uint32_t i=0; i<h; ++i) {
		for(uint32_t j=0; j<w; ++j) {
			pad_img[i*pw+j] = std::complex<double>(data[(i*w+j)*channels+channel],0);
		}
	}

	//pad kernel
	std::complex<double>* pad_ker = new std::complex<double>[psize];
	pad_kernel(ker_w, ker_h, ker, cr, cc, pw, ph, pad_ker);

	//convolution
	dft_2D(ph, pw, pad_img, pad_img);
	dft_2D(ph, pw, pad_ker, pad_ker);
	pointwise_product(psize, pad_img, pad_ker, pad_img);
	idft_2D(ph, pw, pad_img, pad_img);

	//update pixel data
	for(uint32_t i=0; i<h; ++i) {
		for(uint32_t j=0; j<w; ++j) {
			data[(i*w+j)*channels+channel] = BYTE_BOUND((uint8_t)round(pad_img[i*pw+j].real()));
		}
	}

	return *this;
}

Image& Image::fd_convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	//calculate padding
	uint32_t pw = 1<<((uint8_t)ceil(log2(w+ker_w-1)));
	uint32_t ph = 1<<((uint8_t)ceil(log2(h+ker_h-1)));
	uint64_t psize = pw*ph;

	//pad image
	std::complex<double>* pad_img = new std::complex<double>[psize];
	for(uint32_t i=0; i<ph; ++i) {
		uint32_t r = (i<h) ? i : ((i<h+cr ? h-1 : 0));
		for(uint32_t j=0; j<pw; ++j) {
			uint32_t c = (j<w) ? j : ((j<w+cc ? w-1 : 0));
			pad_img[i*pw+j] = std::complex<double>(data[(r*w+c)*channels+channel],0);
		}
	}

	//pad kernel
	std::complex<double>* pad_ker = new std::complex<double>[psize];
	pad_kernel(ker_w, ker_h, ker, cr, cc, pw, ph, pad_ker);

	//convolution
	dft_2D(ph, pw, pad_img, pad_img);
	dft_2D(ph, pw, pad_ker, pad_ker);
	pointwise_product(psize, pad_img, pad_ker, pad_img);
	idft_2D(ph, pw, pad_img, pad_img);

	//update pixel data
	for(uint32_t i=0; i<h; ++i) {
		for(uint32_t j=0; j<w; ++j) {
			data[(i*w+j)*channels+channel] = BYTE_BOUND((uint8_t)round(pad_img[i*pw+j].real()));
		}
	}

	return *this;
}

Image& Image::fd_convolve_cyclic(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	//calculate padding
	uint32_t pw = 1<<((uint8_t)ceil(log2(w+ker_w-1)));
	uint32_t ph = 1<<((uint8_t)ceil(log2(h+ker_h-1)));
	uint64_t psize = pw*ph;

	//pad image
	std::complex<double>* pad_img = new std::complex<double>[psize];
	for(uint32_t i=0; i<ph; ++i) {
		uint32_t r = (i<h) ? i : ((i<h+cr ? i%h : h-ph+i));
		for(uint32_t j=0; j<pw; ++j) {
			uint32_t c = (j<w) ? j : ((j<w+cc ? j%w : w-pw+j));
			pad_img[i*pw+j] = std::complex<double>(data[(r*w+c)*channels+channel],0);
		}
	}

	//pad kernel
	std::complex<double>* pad_ker = new std::complex<double>[psize];
	pad_kernel(ker_w, ker_h, ker, cr, cc, pw, ph, pad_ker);

	//convolution
	dft_2D(ph, pw, pad_img, pad_img);
	dft_2D(ph, pw, pad_ker, pad_ker);
	pointwise_product(psize, pad_img, pad_ker, pad_img);
	idft_2D(ph, pw, pad_img, pad_img);

	//update pixel data
	for(uint32_t i=0; i<h; ++i) {
		for(uint32_t j=0; j<w; ++j) {
			data[(i*w+j)*channels+channel] = BYTE_BOUND((uint8_t)round(pad_img[i*pw+j].real()));
		}
	}

	return *this;
}


Image& Image::convolve_linear(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	if(ker_w*ker_h > 224) {
		return fd_convolve_clamp_to_0(channel, ker_w, ker_h, ker, cr, cc);
	}
	else {
		return std_convolve_clamp_to_0(channel, ker_w, ker_h, ker, cr, cc);
	}
}

Image& Image::convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	if(ker_w*ker_h > 224) {
		return fd_convolve_clamp_to_border(channel, ker_w, ker_h, ker, cr, cc);
	}
	else {
		return std_convolve_clamp_to_border(channel, ker_w, ker_h, ker, cr, cc);
	}
}

Image& Image::convolve_cyclic(uint8_t channel, uint32_t ker_w, uint32_t ker_h, double ker[], uint32_t cr, uint32_t cc) {
	if(ker_w*ker_h > 224) {
		return fd_convolve_cyclic(channel, ker_w, ker_h, ker, cr, cc);
	}
	else {
		return std_convolve_cyclic(channel, ker_w, ker_h, ker, cr, cc);
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



