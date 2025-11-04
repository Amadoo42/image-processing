#pragma once
#include "Filter.h"

class BlackAndWhiteFilter : public Filter {
public:    
    void apply(Image &image) override {
        int sum = 0;
        for(int i = 0; i < image.width; i++) {
            for(int j = 0; j < image.height; j++) {
                int gray = 0.2126 * image(i, j, 0) + 0.7152 * image(i, j, 1) + 0.0722 * image(i, j, 2);

                for(int c = 0; c < 3; c++) image(i, j, c) = gray;

                sum += gray;
            }
        }

        // Calculate the average brightness to use as threshold
        int threshold = sum / (image.width * image.height);

        for(int i = 0; i < image.width; i++){
            for(int j = 0; j < image.height; j++){
                if(image(i, j, 0) >= threshold) {
                    image(i, j, 0) = 255;
                    image(i, j, 1) = 255;
                    image(i, j, 2) = 255;
                }
                else {
                    image(i, j, 0) = 0;
                    image(i, j, 1) = 0;
                    image(i, j, 2) = 0;
                }
            }
        }
    }      
};