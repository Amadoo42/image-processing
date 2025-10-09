
#pragma once
#include "Filter.h"
using namespace std;

class retro : public Filter {
public:
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
        for(int j = 0; j < image.height; j++) {
            if(j % 4 > 1) {
                image(i, j, 0) = image(i, j, 1) = image(i, j, 2) = 0;
            }
        }
    }
    }      
};