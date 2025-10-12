#pragma once
#include "Filter.h"

class DarkenFilter : public Filter {
private:
float percent;
public:
    DarkenFilter(float p) : percent(p) {}
    void apply(Image &image) override {
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                for(int k = 0; k < 3; k++) {
                    int newC = image(i, j, k) * percent;
                    if(newC > 255) newC = 255;
                    if(newC < 0)newC = 0;

                    image(i, j, k) = newC;
                }
            }
        }
    }      
};