#include <string>
#include <sstream>
#include <string.h>

class Performance {
    private:
        std::string functionName;
        std::string timeTaken;
        std::string index;
       
    public:
        Performance();
        void setFuntionName(std::string functionName_){
            this->functionName = functionName_;
        }

        void setTimeTaken(std::string timeTaken_){
            this->timeTaken = timeTaken_;
        }

        void setIndex(std::string index_){
            this->index = index_;
        }

        std::string getFuntionName(){
            return this->functionName;
        }
         std::string getTimeTaken(){
            return this->timeTaken;
        }
         std::string getIndex(){
            return this->index;
        }

        
        
};