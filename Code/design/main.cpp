
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "image_operations.cpp"
#include "images.cpp"
#include "concrete_image.cpp"



int main() {

    std::ifstream myfile ("base64/jpg2.txt");
    std::string mystring;
    if ( myfile.is_open() ) { 
        myfile >> mystring;
    }

    size_t len = mystring.length();
    char* buffer = (char*)malloc(sizeof(char) * len);
    strcpy(buffer, mystring.c_str());


    ImageOperations* ops = new ImageOperations(buffer);
    Images* images = new Images(ops);
    
    images->Backup();
    ops->grayscale_avg();
    ops->write("gray.png");
    images->undo();
    ops->write("original.png");
    images->Backup();
    ops->grayscale_avg();
    ops->write("gray2.png");
    images->undo();
    ops->write("original2.png");
    // images->Backup();
   
   
    // images->undo();
    // std::cout << ops->getRandom() << std::endl;
    
    // std::cout << ops->getRandom() << std::endl;
    std::cout << images->images_.size() << std::endl;
    // std::cout << images->images_[0]->encodeByte() << std::endl;
    // images->images_[0]->grayscale_lum();
    
    // ops->grayscale_avg();
    
    // ops->write("gray.png");


    delete ops;
    delete images;

}