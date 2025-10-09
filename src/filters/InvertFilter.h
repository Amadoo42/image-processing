#pragma once
#include "Filter.h"

class InvertFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width; ++i) {
            for(int j = 0; j < image.height; ++j) {
                image(i, j, 0) = 255 - image(i, j, 0);
                image(i, j, 1) = 255 - image(i, j, 1);
                image(i, j, 2) = 255 - image(i, j, 2);
            }
        }
    }      
};