
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

    Image* img = new ConcreteImage(buffer);
    //img->grayscale_avg();
    // std::cout << img->encodeByte();
    ImageOperations* ops = new ImageOperations(*img);
    Images* images = new Images(ops);
    images->Backup();
    std::cout << images->images_.size() << std::endl;
    ops->grayscale_avg();
    ops->write("gray.png");
    images->undo();
    std::cout << images->images_.size() << std::endl;
    ops->write("original.png");

    delete ops;
    delete images;
    delete img;

}