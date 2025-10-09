#pragma once
#include "Filter.h"

class VerticalFlipFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int j = 0; j < image.height / 2; j++){
            for(int i = 0; i < image.width; i++){
                swap(image(i, j, 0), image(i, image.height - j - 1, 0));
                swap(image(i, j, 1), image(i, image.height - j - 1, 1));
                swap(image(i, j, 2), image(i, image.height - j - 1, 2));   
            }
        }
    }      
};