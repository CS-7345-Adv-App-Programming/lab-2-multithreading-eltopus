
#include "image.h"
#include <unordered_map>


class Images {
   static Images *instance;
   std::unordered_map<const char *, Image *> images;
 
   // Private constructor so that no objects can be created.
   Images() {
   }

   public:
   static Images *getInstance() {
      if (!instance)
      instance = new Images;
      return instance;
   }

   void put(const char *key, Image *value) {
            images[key] = value;
    }

    Image *get(const char *key) {
            return images[key];
    }

};