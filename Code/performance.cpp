#include <string>
#include <sstream>
#include <string.h>
#include <vector>
#include <iostream>

class Performance {
    private:
        std::vector<const char*> results;
        
       
    public:
        Performance();

        void addResult(const char* result){
            results.push_back(result);
            //  std::cout << result << std::endl;
        }

        void getResult() {
            for (auto r: results){
                std::cout << r << std::endl;
            }
        }

        
        
};