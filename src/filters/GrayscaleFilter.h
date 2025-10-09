#pragma once
#include "Filter.h"
using namespace std;

class GrayscaleFilter : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                int gray = 0.2126 * image(i, j, 0) + 0.7152 * image(i, j, 1) + 0.0722 * image(i, j, 2);
                for(int c = 0; c < 3; c++) image(i, j, c) = gray;
            }
        }
    }      
};