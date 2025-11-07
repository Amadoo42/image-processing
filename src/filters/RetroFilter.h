#pragma once
#include "Filter.h"

class RetroFilter : public Filter {
public:
    void apply(Image &image) override {
        // Blacken every 2-out-of-4 pixel rows to create a retro effect
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                if(j % 4 >= 2) {
                    image(i, j, 0) = image(i, j, 1) = image(i, j, 2) = 0;
                }
            }
        }
    }      
};