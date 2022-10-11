#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "image_operations.cpp"
#include "images.cpp"
#include "concrete_image.cpp"


class Originator {

    private:
        ImageOperations* ops_;
        Images* states_;

    public:

        Originator(char* buffer) {
            ops_ = new ImageOperations(buffer);
            states_= new Images(ops_);
        }

        char* encodeByte(){
            return this->ops_->encodeByte();
        }

        void grayscale_avg()  {
            this->states_->Backup();
            this->ops_->grayscale_avg();
        }

        void grayscale_lum() {
            this->states_->Backup();
            this->ops_->grayscale_lum();
              
        }
        
        void colorMask(float r, float g, float b) {
            this->states_->Backup();
            this->ops_->colorMask(r, g, b);

        }

        void encodeMessage(const char* message) {
            this->states_->Backup();
            this->ops_->encodeMessage(message);
        }

        void decodeMessage(char* buffer, size_t* messageLength) {
            this->ops_->decodeMessage(buffer, messageLength);
        }

        char* undo () {
            this->states_->undo();
            return this->ops_->encodeByte();
        }

        bool write(const char* filename)  {
            return this->ops_->write(filename);
        }






};