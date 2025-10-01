#pragma once
#include "Filter.h"
using namespace std;

class DarkenFilter : public Filter {
public:
    DarkenFilter() : Filter("Darken") {}

    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                for(int k = 0; k < 3; k++) {
                    image(i, j, k) = (image(i, j, k) + 0) / 2;
                }
            }
        }
    }      
};