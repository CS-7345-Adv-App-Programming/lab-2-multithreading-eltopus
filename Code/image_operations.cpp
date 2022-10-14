#include "image.h"


#include <iostream>
#include <vector>
#include <string>
#include <sstream>




class ImageOperations {
    private:
        Image* state_;
        std::vector<std::vector<Image*>> states;
        std::vector<Image *> currentStates;
        std::vector<Image *> images_;

        std::vector<std::string> splitBase64 (const std::string &s, char delim) {
            std::vector<std::string> result;
            std::stringstream ss (s);
            std::string item;
            while (getline (ss, item, delim)) {
                result.push_back(item);
            }

            return result;
        }

        Image *save() {
            std::cout << "old image address: " << &this->state_ << std::endl;
            Image* newState = new Image(*this->state_);
            this->state_ = newState;
            std::cout << "new image address: " << &newState << std::endl;
            return newState;
        }

        std::vector<Image* > saves() {
            std::vector<Image* > new_states;
            for (auto i : this->currentStates) {
                std::cout << "old image address: " << &i << std::endl;
                Image* newState = new Image(*i->state_);
                std::cout << "new image address: " << &newState << std::endl;
                new_states.push_back(newState);
            }
            return new_states;
        }
        void restore(Image* oldState) {
            this->state_ = oldState->state_;
            std::cout << "restored old image address: " << &this->state_ << std::endl;
            
        }

        void restores(std::vector<Image *> oldStates) {
            this->currentStates.clear();
            this->currentStates = oldStates;
            for (auto i : this->currentStates) {
                std::cout << "restored old image address: " << &i << std::endl;
            }
            
            
        }
        
        void Backup() {
            this->images_.push_back(this->save());
        }
        
        void Backups() {
            this->states.push_back(this->saves());
        }

        

    public:

        ~ImageOperations() {
            for (auto i : images_) delete i;
        }

        ImageOperations (char* buffer){
            this->state_= new Image(buffer);
        }

        ImageOperations (){
            this->state_= NULL;
        }

        ImageOperations (Image* state){
            this->state_= state;
        }

        void createImages(char* buffer) {
            std::string base64(buffer);
            std::vector<std::string> base64Tokens = splitBase64(base64, ',');
            for (auto i : base64Tokens) {
                Image* image = new Image(i.c_str());
                this->state_= image;
                this->currentStates.push_back(image);
            }
            std::cout << this->currentStates.size() << std::endl;
           
        }
      
        Image* getState() {
            return this->state_;
        }

        char* encodeByte(){
            return this->state_->encodeByte();
        }

        char* encodeBytes(){
            std::string base64Images_s;
            for (auto i : this->currentStates) {
                base64Images_s.append(i->encodeByteString() + ",");
            }
            size_t len = base64Images_s.length();
            char* buffer = (char*)malloc(sizeof(char) * len);
            strncpy(buffer, base64Images_s.c_str(), len-1);
            buffer[len-1] = '\0';
            return buffer;
        }

        void grayscale_avgs()  {
            Backups();
            for (auto i : this->currentStates) {
                i->grayscale_avg();
            }
        }


        void grayscale_avg()  {
            Backup();
            this->state_->grayscale_avg();
        }

        void grayscale_lum() {
            Backup();
            this->state_->grayscale_lum();
              
        }

        void grayscale_lums() {
            Backups();
            for (auto i : this->currentStates) {
                i->grayscale_lum();
            }
              
        }
        
        void colorMask(float r, float g, float b) {
            Backup();
            this->state_->colorMask(r, g, b);

        }

        void colorMasks(float r, float g, float b) {
            Backups();
            for (auto i : this->currentStates) {
                i->colorMask(r, g, b);
            }

        }

        void encodeMessage(const char* message) {
            Backup();
            this->state_->encodeMessage(message);
        }

         void encodeMessages(const char* message) {
            Backups();
            for (auto i : this->currentStates) {
                i->encodeMessage(message);
            }
        }

        void decodeMessage(char* buffer, size_t* messageLength) {
            this->state_->decodeMessage(buffer, messageLength);
        }

        void decodeMessages(char* buffer, size_t* messageLength) {
            for (auto i : this->currentStates) {
                i->decodeMessage(buffer, messageLength);
            }
        }

        bool write(const char* filename)  {
            return this->state_->write(filename);
        }

         bool writes(const char* filename)  {
            // std::cout << this->currentStates.size() << std::endl;
            this->currentStates[1]->write(filename);
            // for (int i=0; i < this->currentStates.size(); i++) {
            //     const char* newFilename = std::string(filename + std::to_string(i) + ".png").c_str();
            //     std::cout << newFilename << std::endl;
            //     // this->currentStates[i]->write(newFilename);
            // }
            return true;
        }



        char* getAddress () {
            return (char*)(&this->state_);
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

        void Undos() {
            if (!this->states.size()) {
                std::cout << "Size not good" << std::endl;
                return;
            }
            std::vector<Image *> images = states.back();
            this->states.pop_back();
            try {
                this->restores(images);
            } catch (...) {
                std::cout << "Exception occured" << std::endl;
                this->Undos();
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