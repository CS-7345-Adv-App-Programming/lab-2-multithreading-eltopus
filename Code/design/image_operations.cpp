#pragma once
#include "concrete_image.cpp"

class ImageOperations {
    private:
        Image* state_;

    public:

        ImageOperations (Image& image){
            this->state_ = &image;
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
            return new ConcreteImage(*this->state_);
        }

        void restore(Image* image) {
            this->state_ = &image->state();
        }



};