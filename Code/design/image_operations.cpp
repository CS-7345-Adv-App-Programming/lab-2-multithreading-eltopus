#pragma once
#include "concrete_image.cpp"
#include<cstdlib>
#include <ctime>
#include <iostream>
#include <vector>


class ImageOperations {
    private:
        Image* state_;

    public:

        ImageOperations (char* buffer){
            this->state_= new ConcreteImage(buffer);
        }

        ImageOperations (Image* state){
            this->state_= state;
        }
      
        Image* getState() {
            return this->state_;
        }
        char* encodeByte(){
            return this->state_->encodeByte();
        }

        void grayscale_avg()  {
            this->state_->grayscale_avg();
        }

        void grayscale_lum() {
            this->state_->grayscale_lum();
              
        }
        
        void colorMask(float r, float g, float b) {
            this->state_->colorMask(r, g, b);

        }

        void encodeMessage(const char* message) {
            this->state_->encodeMessage(message);
        }

        void decodeMessage(char* buffer, size_t* messageLength) {
            this->state_->decodeMessage(buffer, messageLength);
        }

        bool write(const char* filename)  {
            return this->state_->write(filename);
        }

        Image *save() {
            std::cout << "old image address: " << &this->state_ << std::endl;
            Image* newState = new ConcreteImage(*this->state_);
            this->state_ = newState;
            std::cout << "new image address: " << &newState << std::endl;
            return newState;
        }

        void restore(Image* oldState) {
            this->state_ = oldState->state();
            std::cout << "restored old image address: " << &this->state_ << std::endl;
            
        }

        char* getAddress () {
            return (char*)(&this->state_);
        }

        



};