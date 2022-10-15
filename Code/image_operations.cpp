#include "image.h"
#include "metrics.cpp"


#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>

class ImageOperations {
    private:
        std::vector<std::vector<Image*>> states;
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
                Image* newState = new Image(*i->state_);
                new_states.push_back(newState);
            }
            return new_states;
        }
       

        void restores(std::vector<Image *> oldStates) {
            this->currentStates.clear();
            this->currentStates = oldStates;
            
        }
        
        
        void Backups() {
            this->states.push_back(this->saves());
        }

    public:

        ~ImageOperations() {
            for (auto i : this->currentStates) delete i;
        }


        ImageOperations (){
            // this->currentStates = NULL;
        }

        ImageOperations (Image* state){
            // this->state_= state;
        }

        void createImages(char* buffer) {
            std::string base64(buffer);
            std::vector<std::string> base64Tokens = splitBase64(base64, ',');
            for (auto i : base64Tokens) {
                Image* image = new Image(i.c_str());
                this->currentStates.push_back(image);
            }
            std::cout << this->currentStates.size() << std::endl;
           
        }
      

         //---------------------------------------------------------------------------------------------Operations begins

        char* encodeBytes(){
            Metrics* metric;
            Backups();
            char* result = metric->start_encodeBytes_Metrics(this->currentStates, "encodeBytes");
            performaceResult.push_back(metric->getPerformance());
            return result;
        }

        void grayscale_avgs()  {
            Metrics* metric;
            Backups();
            metric->start_grayscale_avgs_metrics(this->currentStates, "grayscale_avg");
            performaceResult.push_back(metric->getPerformance());
        }


        void grayscale_lums() {
            Metrics* metric;
            Backups();
            metric->start_grayscale_avgs_metrics(this->currentStates, "grayscale_lums");
            performaceResult.push_back(metric->getPerformance());
              
        }
        

        void colorMasks(float r, float g, float b) {
            Metrics* metric;
            Backups();
            metric->start_colorMasks_metrics(r, g, b, this->currentStates, "colorMasks");
            performaceResult.push_back(metric->getPerformance());
        }


        void encodeMessages(const char* message) {
            Metrics* metric;
            Backups();
            metric->start_encodeMessages_metrics(message, this->currentStates, "encodeMessages");
            performaceResult.push_back(metric->getPerformance());
        }

        

        void decodeMessages(char* buffer, size_t* messageLength) {
            Metrics* metric;
            Backups();
            metric->start_decodeMessage_metrics(buffer, messageLength, this->currentStates, "decodeMessages");
            performaceResult.push_back(metric->getPerformance());
        }

        void std_convolve_clamp_to_0(uint8_t channel, uint32_t ker_w, uint32_t ker_h, uint32_t cr, uint32_t cc){
            Metrics* metric;
            Backups();
            metric->start_std_convolve_clamp_to_0_Metrics(channel, ker_w, ker_h, cr, cc, this->currentStates, "std_convolve_clamp_to_0");
            performaceResult.push_back(metric->getPerformance());
        }

        void std_convolve_clamp_to_border(uint8_t channel, uint32_t ker_w, uint32_t ker_h, uint32_t cr, uint32_t cc) {
            Metrics* metric;
            Backups();
            metric->start_std_convolve_clamp_to_border_Metrics(channel, ker_w, ker_h, cr, cc, this->currentStates, "std_convolve_clamp_to_border");
            performaceResult.push_back(metric->getPerformance());
        }

        std::string getMetrics() {
            std::string result;
            for (auto r : performaceResult){
                result.append(r);
            }
            return result;
        }

        //---------------------------------------------------------------------------------------------Operations ended

         bool writes(const char* filename)  {
            this->currentStates[0]->write(filename);
            return true;
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
};