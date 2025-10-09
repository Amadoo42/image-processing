#pragma once
#include "Filter.h"
using namespace std;

class HorizontalFlipFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width / 2; i++) {
            for(int j = 0; j < image.height; j++) {
                swap(image(i, j, 0), image(image.width - i - 1, j, 0));
                swap(image(i, j, 1), image(image.width - i - 1, j, 1));
                swap(image(i, j, 2), image(image.width - i - 1, j, 2));   
            }
        }
    }      
};