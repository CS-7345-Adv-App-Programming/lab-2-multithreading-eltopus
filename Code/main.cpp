

#include <string>
#include <vector>
#include "image.cpp"
#include "images.cpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdio.h>
#define THRESHOLD 128
#define WHITE 255
#define BLACK 0
#include <vector>

using namespace std;


// namespace fs = std::filesystem;
// namespace fs = std::__fs::filesystem;


// Images *Images::instance = 0;
std::ifstream myfile;
std::string mystring;

static vector<char> ReadAllBytes(char const* filename)
{
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  result(pos);

    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);

    return result;
}
 


int main(int argc, char** argv)
{
    // Images *img = img->getInstance();

    // Image image("input/z.bmp");
    // std::cout << image.getImageData() << std::endl;

    // std::vector<char> byte = ReadAllBytes("input/spider.png");


    std::ifstream myfile ("bmp.txt");
    std::string mystring;
    if ( myfile.is_open() ) { 
        myfile >> mystring; 
        // std::cout << mystring; 
    }

    Image image(mystring);
    image.grayscale_avg();
    image.write("test1.jpeg");

    // const uint8_t* p = reinterpret_cast<const uint8_t*>(str.c_str());


    //Image image(width, height, bitDepth);
    //image.write("test.bmp");

    // std::string path = "./input";
    // for (auto & p : fs::directory_iterator(path)) {
    //     const char* filepath = p.path().c_str();
    //     img->put(filepath, new Image(filepath));
    // }



    // // test.grayscale_avg();
    // // test.write("gray.png");
    
    // Image gray_avg = test;
    // gray_avg.colorMask(0, 1, 1);
    // // // gray_avg.write("red.png");

    // Image t = *img->get("./input/a.jpg");
    // std::string key = img->getKey("./input/a.jpg");
    // std::cout << key << std::endl;
    // t.encodeMessage("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum");
    // t.grayscale_avg();
    // t.write("./output/encoded.png");
    // char buffer[4103] = {0};
    // size_t len = 0;
    // t.decodeMessage(buffer, &len);
    // printf("Message: %s (%zu)\n", buffer, len); 
 
 return 0;
}