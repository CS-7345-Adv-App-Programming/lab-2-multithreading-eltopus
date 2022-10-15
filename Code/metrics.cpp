#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <string.h>
#include <functional>

#include <iostream>
#include <mutex>
#include <fstream>
#include "image.h"

static double kers[] =  {   1/16.0,  2/16.0,  1/16.0,
                        2/16.0, 4/16.0, 2/16.0,
                        1/16.0, 2/16.0, 1/16.0
                };

static std::mutex logMutex;

class Metrics {

    private:
        
        bool fileExists(std::string& fileName) {
            return static_cast<bool>(std::ifstream(fileName));
        }

        bool writePerformance(std::string filename, std::string column1, std::string column2, std::string column3) {
            std::lock_guard<std::mutex> csvLock(logMutex);
            std::fstream file;
            file.open (filename, std::ios::out | std::ios::app);
            if (file) {
                file << "\"" << column1 << "\",";
                file << "\"" << column2 << "\",";
                file << "\"" << column3 << "\"";
                file <<  std::endl;
                // file.close();
                return true;
            } else {
                return false;
            }
            
        }

    public:
        int n =5;
    
        std::vector<std::string> performance;
        Metrics(){

        };
        ~Metrics();

        void start_std_convolve_clamp_to_0_Metrics(uint8_t channel, uint32_t ker_w, uint32_t ker_h, uint32_t cr, uint32_t cc, std::vector<Image*> states_, std::string func) {
            auto begin = std::chrono::high_resolution_clock::now();
            for (auto i : states_) {
                    i->std_convolve_clamp_to_0(channel, ker_w, ker_h, kers, cr, cc);
                }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            printf("Time measured for %s is: %.3f seconds.\n", func.c_str(), elapsed.count() * 1e-9);
            writePerformance("performance.log", "1", func, std::to_string(elapsed.count() * 1e-9));
            std::string record = "1, " + func + ", " + std::to_string(elapsed.count() * 1e-9) + "\n";
            // performance.push_back(record);

            
            
        }

        void start_std_convolve_clamp_to_border_Metrics(uint8_t channel, uint32_t ker_w, uint32_t ker_h, uint32_t cr, uint32_t cc, std::vector<Image*> states_, std::string func) {
            auto begin = std::chrono::high_resolution_clock::now();
            for (auto i : states_) {
                    i->std_convolve_clamp_to_border(channel, ker_w, ker_h, kers, cr, cc);
                }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            printf("Time measured for %s is: %.3f seconds.\n", func.c_str(), elapsed.count() * 1e-9);
            writePerformance("performance.log", "1", func, std::to_string(elapsed.count() * 1e-9));
            std::string record = "1, " + func + ", " + std::to_string(elapsed.count() * 1e-9) + "\n";
            // performance.push_back(record);

        }

        void start_decodeMessage_metrics(char* buffer, size_t* messageLength, std::vector<Image*> states_, std::string func) {
            auto begin = std::chrono::high_resolution_clock::now();
            for (auto i : states_) {
                i->decodeMessage(buffer, messageLength);
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            printf("Time measured for %s is: %.3f seconds.\n", func.c_str(), elapsed.count() * 1e-9);
            writePerformance("performance.log", "1", func, std::to_string(elapsed.count() * 1e-9));
            std::string record = "1, " + func + ", " + std::to_string(elapsed.count() * 1e-9) + "\n";
            performance.push_back(record);

           
        }

         void start_encodeMessages_metrics(const char* message, std::vector<Image*> states_, std::string func) {
            auto begin = std::chrono::high_resolution_clock::now();
            for (auto i : states_) {
                i->encodeMessage(message);
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            printf("Time measured for %s is: %.3f seconds.\n", func.c_str(), elapsed.count() * 1e-9);
            writePerformance("performance.log", "1", func, std::to_string(elapsed.count() * 1e-9));
            std::string record = "1, " + func + ", " + std::to_string(elapsed.count() * 1e-9) + "\n";
            performance.push_back(record);
        }


        void start_colorMasks_metrics(float r, float g, float b, std::vector<Image*> states_, std::string func) {
            auto begin = std::chrono::high_resolution_clock::now();
            for (auto i : states_) {
                i->colorMask(r, g, b);
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            printf("Time measured for %s is: %.3f seconds.\n", func.c_str(), elapsed.count() * 1e-9);
            writePerformance("performance.log", "1", func, std::to_string(elapsed.count() * 1e-9));
            std::string record = "1, " + func + ", " + std::to_string(elapsed.count() * 1e-9) + "\n";
            performance.push_back(record);

        }

        void start_grayscale_lums_metrics(std::vector<Image*> states_, std::string func) {

            auto begin = std::chrono::high_resolution_clock::now();
            for (auto i : states_) {
                i->grayscale_lum();
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            printf("Time measured for %s is: %.3f seconds.\n", func.c_str(), elapsed.count() * 1e-9);
            writePerformance("performance.log", "1", func, std::to_string(elapsed.count() * 1e-9));
            std::string record = "1, " + func + ", " + std::to_string(elapsed.count() * 1e-9) + "\n";
            performance.push_back(record);
              
        }

        void start_grayscale_avgs_metrics(std::vector<Image*> states_, std::string func) {

            auto begin = std::chrono::high_resolution_clock::now();
            for (auto i : states_) {
                i->grayscale_avg();
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            printf("Time measured for %s is: %.3f seconds.\n", func.c_str(), elapsed.count() * 1e-9);
            writePerformance("performance.log", "1", func, std::to_string(elapsed.count() * 1e-9));
            std::string record = "1, " + func + ", " + std::to_string(elapsed.count() * 1e-9) + "\n";
            performance.push_back(record);
              
        }

        char* start_encodeBytes_Metrics(std::vector<Image*> states_, std::string func){

            auto begin = std::chrono::high_resolution_clock::now();
            std::string base64Images_s;
            for (auto i : states_) {
                base64Images_s.append(i->encodeByteString() + ",");
            }
            size_t len = base64Images_s.length();
            char* buffer = (char*)malloc(sizeof(char) * len);
            strncpy(buffer, base64Images_s.c_str(), len-1);
            buffer[len-1] = '\0';
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            printf("Time measured for %s is: %.3f seconds.\n", func.c_str(), elapsed.count() * 1e-9);
            writePerformance("performance.log", "1", func, std::to_string(elapsed.count() * 1e-9));
            std::string record = "1, " + func + ", " + std::to_string(elapsed.count() * 1e-9) + "\n";
            performance.push_back(record);
            return buffer;

           
            
        }

        std::string getPerformance() {
            std::string result;
            for (auto s : performance){
                result.append(s);
            }
            return result;
        }

};