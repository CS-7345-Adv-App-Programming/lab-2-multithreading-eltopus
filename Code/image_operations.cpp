#include "image.h"
#include "metrics.cpp"


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>

static Metrics* metric;

class ImageOperations {
    private:
        std::vector<std::vector<Image*>> backupStates;
        std::vector<Image *> currentStates;
        std::vector<std::string> performaceResult;
        

        std::vector<std::string> splitBase64 (const std::string &s, char delim) {
            std::vector<std::string> result;
            std::stringstream ss (s);
            std::string item;
            while (getline (ss, item, delim)) {
                result.push_back(item);
            }

            return result;
        }


        std::vector<Image* > saves() {
            std::vector<Image* > new_states;
            for (auto i : this->currentStates) {
                Image* newState = new Image(*i);
                new_states.push_back(newState);
            }
            return new_states;
        }
       

        void restores(std::vector<Image *> oldStates) {
            this->currentStates = oldStates;
            
        }
        
        
        void Backups() {
            this->backupStates.push_back(this->saves());
        }

    public:

        ~ImageOperations() {
            for (auto i : this->currentStates) delete i;
        }


        ImageOperations (){
            // this->currentStates = NULL;
        }

        ImageOperations (Image* state){
            //this->state_= state;
        }

        void createImages(char* buffer) {
            std::string base64(buffer);
            std::vector<std::string> base64Tokens = splitBase64(base64, ',');
            for (auto i : base64Tokens) {
                Image* image = new Image(i.c_str());
                this->currentStates.push_back(image);
            }
            std::cout << "Current Image Size: " << this->currentStates.size() << std::endl;
           
        }
      

         //---------------------------------------------------------------------------------------------Operations begins

        char* encodeBytes(){
            char* result = metric->start_encodeBytes_Metrics(this->currentStates, "encodeBytes");
            return result;
        }

        void grayscale_avgs()  {
            Backups();
            metric->start_grayscale_avgs_metrics(this->currentStates, "grayscale_avg");
        }


        void grayscale_lums() {
            Backups();
            metric->start_grayscale_avgs_metrics(this->currentStates, "grayscale_lums");
              
        }
        

        void colorMasks(float r, float g, float b) {
            Backups();
            metric->start_colorMasks_metrics(r, g, b, this->currentStates, "colorMasks");
        }


        void encodeMessages(const char* message) {
            Backups();
            metric->start_encodeMessages_metrics(message, this->currentStates, "encodeMessages");
        }

        

        void decodeMessages(char* buffer, size_t* messageLength) {
            Backups();
            metric->start_decodeMessage_metrics(buffer, messageLength, this->currentStates, "decodeMessages");

        }

        char* decodeMessagesLib() {
            Backups();
            return metric->start_decodeMessageLib_metrics(this->currentStates, "decodeMessages");
        }

        void std_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, uint32_t cr, uint32_t cc){
            Backups();
            metric->start_std_convolve_clamp_to_0_Metrics(channel, ker_w, ker_h, cr, cc, this->currentStates, "std_convolve_clamp_to_0");
           
        }

        void std_convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, uint32_t cr, uint32_t cc) {
            Backups();
            metric->start_std_convolve_clamp_to_border_Metrics(channel, ker_w, ker_h, cr, cc, this->currentStates, "std_convolve_clamp_to_border");
           
        }

        void flipX()  {
            Backups();
            metric->start_flipX_Metrics(this->currentStates, "flipX");
        }


        void flipY() {
            Backups();
            metric->start_flipY_Metrics(this->currentStates, "flipY");
              
        }

        char* getMetrics() {
            std::string result;
            for (std::string r : metric->getPerformance()){
                result.append(r);
            }
            size_t len = result.length();
            char* buffer = (char*)malloc(sizeof(char) * len);
            strncpy(buffer, result.c_str(), len-1);
            buffer[len-1] = '\0';
            return buffer;
        }

        //---------------------------------------------------------------------------------------------Operations ended

         bool writes(const char* filename)  {
            this->currentStates[0]->write(filename);
            return true;
        }

        void Undos() {
            if (!this->backupStates.size()) {
                std::cout << "Size not good" << std::endl;
                return;
            }
            std::vector<Image *> images = backupStates.back();
            this->backupStates.pop_back();
            try {
                // images[0]->write("../../Data/cPlusOutput/tiger_gray_lum_d.png") ;
                this->restores(images);
            } catch (...) {
                std::cout << "Exception occured" << std::endl;
                this->Undos();
            }
        }
};