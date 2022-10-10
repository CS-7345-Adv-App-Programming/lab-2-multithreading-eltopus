#include "concrete_image.cpp"
#include "image_operations.cpp"
#include <vector>

class Images {
    private:
     
     ImageOperations* ops_;

     public:

     std::vector<Image *> images_;
     
        Images(ImageOperations* img_ops) {
            this->ops_ = img_ops;
        }

        ~Images() {
            for (auto i : images_) delete i;
        }

        void Backup() {
            this->images_.push_back(this->ops_->save());
        }

        void undo() {
            if (!this->images_.size()) {
            return;
        }
            Image *image = this->images_.back();
            this->images_.pop_back();
            try {
                this->ops_->restore(image);
            } catch (...) {
                this->undo();
            }
        };

};