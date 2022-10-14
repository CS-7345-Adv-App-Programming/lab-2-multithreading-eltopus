
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>   
#include <string.h>
#include "image_operations.cpp"



int main() {

    std::vector<std::string> files{ "../../Data/base64_images/tiger.txt", "../../Data/base64_images/bird.txt"};
    // std::vector<std::string> files{ "../../Data/base64_images/tiger.txt"};
    std::string base64Images;
    
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



    size_t len = base64Images.length();
    char* buffer = (char*)malloc(sizeof(char) * len);
    std::strcpy(buffer, base64Images.c_str());

    ImageOperations* ops = new ImageOperations();
    ops->createImages(buffer);
    ops->grayscale_avgs();
    ops->writes("../../Data/progOutput/gray1.png");
    ops->Undos();
 
    char* b = ops->encodeBytes();
    std::ofstream mynewfile;
    mynewfile.open("../../Data/base64_images/combinedImages.txt");
    mynewfile << b;
    mynewfile.close();

    std::ifstream mynewerfile ("../../Data/base64_images/combinedImages.txt");
    std::string newbase64Images;
    if ( mynewerfile.is_open() ) { 
        mynewerfile >> newbase64Images;
    }
    
    

    size_t len1 = newbase64Images.length();
    char* buffer1 = (char*)malloc(sizeof(char) * len1);
    std::strcpy(buffer1, newbase64Images.c_str());

    ImageOperations* ops2 = new ImageOperations();
    ops2->createImages(buffer1);
    ops2->grayscale_avgs();
    ops2->writes("../../Data/progOutput/gray2.png");

    //delete ops;
    delete ops2;
    mynewerfile.close();

    // ops->Undos();
    // ops->writes("../../Data/progOutput/gray.png");

    // std::cout << base64Images << std::endl;
    
    // std::ifstream myfile ("../../Data/base64_images/tiger.txt");
    // std::string mystring;
    // if ( myfile.is_open() ) { 
    //     myfile >> mystring;
    // }


    // size_t len = mystring.length();
    // char* buffer = (char*)malloc(sizeof(char) * len);
    // std::strcpy(buffer, mystring.c_str());

    // ImageOperations* ops = new ImageOperations(buffer);

    // ImageOperations* ops = new ImageOperations();
    
    // std::cout << std::string(ops->encodeByte()).length();

    // const char* bytes = "1372883,1372889,1372882";
    // ops->createImage(buffer, bytes);
    
    // ops->grayscale_avg();
    // ops->write("../../Data/output/gray.png");
    // ops->Undo();
    // ops->write("../../Data/output/original.png");

    // Originator* state = new Originator(buffer);
    // state->grayscale_avg();
    // state->write("../../Data/output/gray.png");
    // state->undo();
    // state->write("../../Data/output/original.png");
    // state->grayscale_avg();
    // state->write("../../Data/output/gray2.png");
    // state->undo();
    // state->write("../../Data/output/original2.png");

  
   
    // std::cout << images->images_.size() << std::endl;

    // myfile.close();
    
}