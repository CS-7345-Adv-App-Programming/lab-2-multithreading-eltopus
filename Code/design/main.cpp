
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
// #include "image_operations.cpp"
// #include "images.cpp"
// #include "concrete_image.cpp"
#include "originator.cpp"



int main() {

    std::ifstream myfile ("base64/jpg2.txt");
    std::string mystring;
    if ( myfile.is_open() ) { 
        myfile >> mystring;
    }

    size_t len = mystring.length();
    char* buffer = (char*)malloc(sizeof(char) * len);
    strcpy(buffer, mystring.c_str());

    Originator* state = new Originator(buffer);
    state->grayscale_avg();
    state->write("gray.png");
    state->undo();
    state->write("original.png");
  
   
    // std::cout << images->images_.size() << std::endl;
    

    delete state;

}