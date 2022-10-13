#include "image.h"


#include <iostream>
#include <vector>
#include <string>




class ImageOperations {
    private:
        Image* state_;
        std::vector<Image *> images_;
        

    public:

        ~ImageOperations() {
            for (auto i : images_) delete i;
        }

        ImageOperations (char* buffer){
            this->state_= new Image(buffer);
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
            Backup();
            this->state_->grayscale_avg();
        }

        void grayscale_lum() {
            Backup();
            this->state_->grayscale_lum();
              
        }
        
        void colorMask(float r, float g, float b) {
            Backup();
            this->state_->colorMask(r, g, b);

        }

        void encodeMessage(const char* message) {
            Backup();
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
            Image* newState = new Image(*this->state_);
            this->state_ = newState;
            std::cout << "new image address: " << &newState << std::endl;
            return newState;
        }

        void restore(Image* oldState) {
            this->state_ = oldState->state_;
            std::cout << "restored old image address: " << &this->state_ << std::endl;
            
        }

        char* getAddress () {
            return (char*)(&this->state_);
        }

        void Backup() {
            this->images_.push_back(this->save());
        }

        void Undo() {
            if (!this->images_.size()) {
                std::cout << "Size not good" << std::endl;
                return;
            }
            Image *image = this->images_.back();
            this->images_.pop_back();
            try {
                this->restore(image);
            } catch (...) {
                std::cout << "Exception occured" << std::endl;
                this->Undo();
            }
        }


        void printBuffer(char** buffer, int buffer_length, char** operations, int ops_length) {
            for (int i=0; i < buffer_length; i++) {
                printf("%s\n", buffer[i]);
            }

            for (int i=0; i < ops_length; i++) {
                printf("%s\n", operations[i]);
            }
        }

        



};