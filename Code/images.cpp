
#include "image.h"
#include <unordered_map>
#include <string.h>
#include <random>
#include <iostream>



class Images {
   static Images *instance;
   std::unordered_map<std::string, Image *> images;
   std::unordered_map<std::string, std::string> mapper;
 
   // Private constructor so that no objects can be created.
   Images() {
   }

   public:
   static Images *getInstance() {
      if (!instance)
      instance = new Images;
      return instance;
   }

   void put(const char *filepath, Image *image) {
        std::string uuid = get_uuid();
        // cout << "Put using key: " << uuid << endl;
        mapper.insert({uuid, filepath});
        images.insert({uuid, image});
    }

    Image *get(const char *filePath) {
        std::string key = getKey(filePath);
        if (!key.empty()) {
                std::cout << "Attempting to get key: " << key << std::endl;
             return images[key];   
        }
        std::cout << "Key is NULL: " << key << std::endl;
        return NULL;
    }

    int getImageVectorSize() {
        return images.size();
    }

    std::vector<std::string> extract_keys() {
        std::vector<std::string> retval;
        for (auto const& element : images) {
                retval.push_back(element.first);
        }
        return retval;
    }

    const char *getNewKey(const char* filepath) {
        char* c = strdup(filepath);
        const char * token, * newKey;
        newKey = token = strtok(c, "/");
        for (;(token = strtok(NULL, "/")) != NULL; newKey = token);
        return newKey;
    }

    std::string getKey(const char* filepath) {
        std::cout << "I am here" << std::endl;
    //    for (const auto & [ key, value ] : mapper) {
    //     // cout << key << ": " << value << endl;
    //     if (value == filepath){
    //             return key;
    //     }
    //    }
       for (auto it: mapper) {
        if (it.second == filepath) {
            return it.first;
        }
       }
       return "";

    }

    std::string get_uuid() {
        static std::random_device dev;
        static std::mt19937 rng(dev());

        std::uniform_int_distribution<int> dist(0, 15);

        const char *v = "0123456789abcdef";
        const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

        std::string res;
        for (int i = 0; i < 16; i++) {
                if (dash[i]) res += "-";
                res += v[dist(rng)];
                res += v[dist(rng)];
        }
        // std::cout << res << std::endl;
        return res;
}

};