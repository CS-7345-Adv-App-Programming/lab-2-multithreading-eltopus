
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "image_operations.cpp"



int main() {

    std::ifstream myfile ("../../Data/base64_images/jpg2.txt");
    std::string mystring;
    if ( myfile.is_open() ) { 
        myfile >> mystring;
    }


    size_t len = mystring.length();
    char* buffer = (char*)malloc(sizeof(char) * len);
    std::strcpy(buffer, mystring.c_str());

    ImageOperations* ops = new ImageOperations(buffer);
    
    ops->grayscale_avg();
    ops->write("../../Data/output/gray.png");
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

    myfile.close();
    delete ops;

}