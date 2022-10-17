
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>   
#include <string.h>
#include "image_operations.cpp"


void writeMetrics(char* metrics) {
    std::ofstream myfile ("cPlusMetrics.csv");
    if (myfile.is_open())
    {
        myfile << metrics;
        myfile.close();
    }
    else std::cout << "Unable to open file";
};

void createTestImages(ImageOperations* ops, int n) {

    for (int i =0; i < n; i++){

        // Perform operations
        ops->grayscale_avgs();
        ops->writes("../../Data/cPlusOutput/tiger_gray_avg.png") ;
        ops->Undos();
        ops->grayscale_lums();
        ops->writes("../../Data/cPlusOutput/tiger_gray_lum.png") ;
        ops->Undos();
        ops->colorMasks(1, 0, 0);
        ops->writes("../../Data/cPlusOutput/tiger_color_mask.png") ;
        ops->Undos();
        ops->encodeMessages("Some looooooooooooooooooooooooooooooooooooooooooooooooooooon strinnnnnnnnnnnnnnnnnnnnnnnnnnng");
        std::string decodedString = ops->decodeMessagesLib();
        std::cout << "decodedMessage: " << decodedString << std::endl;
        ops->Undos();
        // Apply to all three channels
        ops->std_convolve_clamp_to_0(0, 3, 3, 1, 1);
        ops->std_convolve_clamp_to_0(1, 3, 3, 1, 1);
        ops->std_convolve_clamp_to_0(2, 3, 3, 1, 1);
        ops->writes("../../Data/cPlusOutput/tiger_blur1.png");
        ops->Undos();
        ops->Undos();
        ops->Undos();

        // Apply to all three channels
        ops->std_convolve_clamp_to_border(0, 3, 3, 1, 1);
        ops->std_convolve_clamp_to_border(1, 3, 3, 1, 1);
        ops->std_convolve_clamp_to_border(2, 3, 3, 1, 1);
        ops->writes("../../Data/cPlusOutput/tiger_blur2.png");
        ops->Undos();
        ops->Undos();
        ops->Undos();

        ops->flipX();
        ops->writes("../../Data/cPlusOutput/tiger_flipX.png");
        ops->Undos();
        ops->flipY();
        ops->writes("../../Data/cPlusOutput/tiger_flipY.png");
        ops->Undos();
    }
};

void createMetrics(ImageOperations* ops, int n) {

    for (int i =0; i < n; i++){

        // Perform operations
        ops->grayscale_avgs();
        ops->Undos();
        ops->grayscale_lums();
        ops->Undos();
        ops->colorMasks(1, 0, 0);
        ops->Undos();
        ops->encodeMessages("Some looooooooooooooooooooooooooooooooooooooooooooooooooooon strinnnnnnnnnnnnnnnnnnnnnnnnnnng");
        std::string decodedString = ops->decodeMessagesLib();
        // std::cout << "decodedMessage: " << decodedString << std::endl;
        ops->Undos();
        ops->std_convolve_clamp_to_0(0, 3, 3, 1, 1);
        ops->Undos();
        ops->std_convolve_clamp_to_border(0, 3, 3, 1, 1);
        ops->Undos();
        ops->flipX();
        ops->Undos();
        ops->flipY();
        char* base64Response = ops->encodeBytes();
    }
    char* metrics = ops->getMetrics();
    writeMetrics(metrics);
};


void runOperations(int n, std::vector<std::string> files) {

    if (n < 1){
        n = 1;
    }
    std::string base64Images;

    //Generate combined comma seperated base64 image encoded string 
    
    for (int i = 0; i < files.size(); i++) {
        std::ifstream myfile (files[i]);
        std::string mystring;
        if ( myfile.is_open() ) { 
            myfile >> mystring;
        }
        size_t len = mystring.length();
        base64Images.append(mystring + ",");
        myfile.close();
    
    }

    // Create char* buffer for the encoded string
    size_t len = base64Images.length();
    char* buffer = (char*)malloc(sizeof(char) * len);
    std::strcpy(buffer, base64Images.c_str());

    // Create instance of Image Operations
    ImageOperations* ops1 = new ImageOperations();
    ops1->createImages(buffer);
    createTestImages(ops1, 1);

    ImageOperations* ops2 = new ImageOperations();
    ops2->createImages(buffer);
    createMetrics(ops2, n);
    delete ops1;
    delete ops2;
}


int main() {
    std::vector<std::string> files{ "../../Data/base64_images/tiger.txt", "../../Data/base64_images/tiger.txt"};
    runOperations(50, files);
}